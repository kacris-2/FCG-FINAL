#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Context.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

struct PlanetProxy3D 
{
    std::string name;
    float posX;            
    float rotationSpeed;   
    float radius;          
    glm::vec3 color;       
    float rotationAngle;   
    
    float orbitSpeed;
    float orbitAngle;
};

std::string loadShaderSource(const std::string& filepath) 
{
    std::ifstream file(filepath);
    if (!file.is_open()) 
    {
        std::cerr << "ERROR: The shader is not found at: " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void createSphere(float radius, unsigned int rings, unsigned int sectors, std::vector<float>& vertices, std::vector<unsigned int>& indices) 
{
    const float R = 1.0f / (float)(rings - 1);
    const float S = 1.0f / (float)(sectors - 1);
    const float PI = 3.14159265f;

    for (unsigned int r = 0; r < rings; ++r) 
    {
        for (unsigned int s = 0; s < sectors; ++s) 
        {
            float y = std::sin(-PI / 2.0f + PI * r * R);
            float x = std::cos(2.0f * PI * s * S) * std::sin(PI * r * R);
            float z = std::sin(2.0f * PI * s * S) * std::sin(PI * r * R);
            vertices.insert(vertices.end(), {x * radius, y * radius, z * radius, x, y, z});
        }
    }

    for (unsigned int r = 0; r < rings - 1; ++r) 
    {
        for (unsigned int s = 0; s < sectors - 1; ++s) 
        {
            indices.insert(indices.end(), {r * sectors + s, r * sectors + (s + 1), (r + 1) * sectors + (s + 1),
                                           r * sectors + s, (r + 1) * sectors + (s + 1), (r + 1) * sectors + r});
        }
    }
}

int main() 
{
    const float CANVAS_WIDTH = 1280.0f;
    const float CANVAS_HEIGHT = 720.0f;
    
    float currentWindowWidth = CANVAS_WIDTH;
    float currentWindowHeight = CANVAS_HEIGHT;

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 4;
    settings.minorVersion = 1;
    settings.attributeFlags = sf::ContextSettings::Attribute::Core;

    sf::RenderWindow window;
    window.create(sf::VideoMode({1280, 720}), "Star Wars Planetary System - Tappa 06", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) 
    {
        std::cerr << "Critical error: GLAD could not initialize the graphics context" << std::endl;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::cout << "========================================================\n";
    std::cout << "  Star Wars Planetary System - Tappa 06 \n";
    std::cout << "========================================================\n";

    std::string vertSrc = loadShaderSource("shaders/phong.vert");
    std::string fragSrc = loadShaderSource("shaders/phong.frag");
    const char* vCode = vertSrc.c_str();
    const char* fCode = fragSrc.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vCode, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fCode, NULL);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    createSphere(1.0f, 26, 26, vertices, indices); 

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::vector<PlanetProxy3D> planets = 
    {
        {"Mustafar", -6.5f, 4.0f, 0.70f, glm::vec3(0.82f, 0.15f, 0.15f), 0.0f, 0.6f, 0.0f}, 
        {"Tatooine", -3.2f, 2.5f, 0.60f, glm::vec3(0.92f, 0.82f, 0.58f), 0.7f, 1.0f, 1.5f}, 
        {"Hoth",      3.2f, 3.0f, 0.55f, glm::vec3(0.70f, 0.86f, 0.94f), 1.4f, 0.9f, 3.0f}, 
        {"Endor",     6.5f, 1.8f, 0.75f, glm::vec3(0.13f, 0.54f, 0.13f), 2.1f, 0.5f, 4.5f}  
    };

    glm::vec3 sunColor = glm::vec3(1.0f, 0.54f, 0.0f); 
    float sunRotationAngle = 0.0f;
    float sunSpeed = 1.2f; 

    // --- ORBITING CAMERA ---
    float camRadius = 16.0f;       
    float camYaw = -90.0f;         
    float camPitch = 0.0f;         
    float camSpeed = 60.0f;        
    float zoomSpeed = 10.0f;       

    sf::Clock deltaClock;
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* resizeEvent = event->getIf<sf::Event::Resized>()) 
            {
                currentWindowWidth = static_cast<float>(resizeEvent->size.x);
                currentWindowHeight = static_cast<float>(resizeEvent->size.y);
                
                float minScale = std::min(currentWindowWidth / CANVAS_WIDTH, currentWindowHeight / CANVAS_HEIGHT);
                float viewportWidth = CANVAS_WIDTH * minScale;
                float viewportHeight = CANVAS_HEIGHT * minScale;
                float viewportX = (currentWindowWidth - viewportWidth) / 2.0f;
                float viewportY = (currentWindowHeight - viewportHeight) / 2.0f;

                glViewport(static_cast<GLint>(viewportX), static_cast<GLint>(viewportY), 
                           static_cast<GLsizei>(viewportWidth), static_cast<GLsizei>(viewportHeight));
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) window.close();
            }
        }

        float deltaTime = deltaClock.restart().asSeconds();

        // --- keyboard input system, interactive camera ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  camYaw -= camSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) camYaw += camSpeed * deltaTime;
        
        // vertical rotation control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) camPitch += camSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) camPitch -= camSpeed * deltaTime;

        
        if (camPitch > 85.0f)  camPitch = 85.0f;
        if (camPitch < -85.0f) camPitch = -85.0f;

        // zoom control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))   camRadius -= zoomSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) camRadius += zoomSpeed * deltaTime;

        
        if (camRadius < 2.0f)  camRadius = 2.0f;
        if (camRadius > 40.0f) camRadius = 40.0f;

        // --- CAMERA POSITION CALCULATION ---
        float pitchRad = glm::radians(camPitch);
        float yawRad = glm::radians(camYaw);

        glm::vec3 cameraPos;
        cameraPos.x = camRadius * std::cos(pitchRad) * std::cos(yawRad);
        cameraPos.y = camRadius * std::sin(pitchRad);
        cameraPos.z = camRadius * std::cos(pitchRad) * std::sin(yawRad);

        
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), CANVAS_WIDTH / CANVAS_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp); 

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "vp"), 1, GL_FALSE, glm::value_ptr(proj * view));

        glClearColor(0.019f, 0.019f, 0.039f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // --- RENDER SOL ---
        sunRotationAngle += deltaTime * sunSpeed;
        glm::mat4 sunModel = glm::mat4(1.0f);
        sunModel = glm::rotate(sunModel, sunRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f)); 
        sunModel = glm::scale(sunModel, glm::vec3(1.3f)); 

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(sunModel));
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(sunColor));
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

        // --- RENDER PLANETAS ---
        for (auto& planet : planets) 
        {
            planet.rotationAngle += deltaTime * planet.rotationSpeed;
            planet.orbitAngle    += deltaTime * planet.orbitSpeed;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, planet.orbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(planet.posX, 0.0f, 0.0f)); 
            model = glm::rotate(model, planet.rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f)); 
            model = glm::scale(model, glm::vec3(planet.radius)); 

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(planet.color));

            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        }

        window.display();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shaderProgram);

    return 0;
}