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
    std::string texturePath; 
    GLuint textureID;        
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

GLuint loadTexture(const std::string& path)
{
    sf::Image image;
    if (!image.loadFromFile(path))
    {
        std::cerr << "ERROR: The texture could not be loaded from the path: " << path << std::endl;
        return 0;
    }

    image.flipVertically(); 

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
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

            float u = (float)s * S;
            float v = (float)r * R;

            
            vertices.insert(vertices.end(), {
                x * radius, y * radius, z * radius, 
                x, y, z,                            
                u, v                                
            });
        }
    }

    for (unsigned int r = 0; r < rings - 1; ++r) 
    {
        for (unsigned int s = 0; s < sectors - 1; ++s) 
        {
            indices.insert(indices.end(), {r * sectors + s, (r + 1) * sectors + s, (r + 1) * sectors + (s + 1),
                                           r * sectors + s, (r + 1) * sectors + (s + 1), r * sectors + (s + 1)});
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
    window.create(sf::VideoMode({1280, 720}), "Star Wars Planetary System - Tappa 09", sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction))) 
    {
        std::cerr << "Critical error: GLAD could not initialize the graphics context" << std::endl;
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    std::cout << "========================================================\n";
    std::cout << "  Star Wars Planetary System - Tappa 09  \n";
    std::cout << "========================================================\n";

    
    std::string vertSrc = loadShaderSource("shaders/texture_phong.vert");
    std::string fragSrc = loadShaderSource("shaders/texture_phong.frag");
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
    createSphere(1.0f, 32, 32, vertices, indices); 

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    
    GLuint sunTexture = loadTexture("textures/sun.jpg");

    std::vector<PlanetProxy3D> planets = 
{
    
    {"Mustafar",  3.5f, 4.0f, 0.45f, "textures/mustafar.jpg", 0, 0.0f, 1.2f, 0.0f}, 
    {"Tatooine",  6.0f, 2.5f, 0.60f, "textures/tatooine.jpg", 0, 0.7f, 0.8f, 1.5f}, 
    {"Hoth",      8.5f, 3.0f, 0.55f, "textures/hoth.jpg",     0, 1.4f, 0.6f, 3.0f}, 
    {"Endor",    11.5f, 1.8f, 0.70f, "textures/endor.png",    0, 2.1f, 0.4f, 4.5f}  
};

    for(auto& planet : planets) {
        planet.textureID = loadTexture(planet.texturePath);
    }

    float camRadius = 16.0f;       
    float camYaw = -90.0f;         
    float camPitch = 20.0f;         
    float camSpeed = 60.0f;        
    float zoomSpeed = 10.0f;       
    int currentTarget = -1; 

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
                
                if (keyPressed->code == sf::Keyboard::Key::Num0) currentTarget = -1; //sun
                if (keyPressed->code == sf::Keyboard::Key::Num1) currentTarget = 0;  // mustafar
                if (keyPressed->code == sf::Keyboard::Key::Num2) currentTarget = 1;  // tatooine
                if (keyPressed->code == sf::Keyboard::Key::Num3) currentTarget = 2;  // hoth
                if (keyPressed->code == sf::Keyboard::Key::Num4) currentTarget = 3;  // endor
            }
        }

        float deltaTime = deltaClock.restart().asSeconds();

        for (auto& planet : planets) 
        {
            planet.rotationAngle += deltaTime * planet.rotationSpeed;
            planet.orbitAngle    += deltaTime * planet.orbitSpeed;
        }

        // --- keyboard input system, interactive camera ---
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  camYaw -= camSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) camYaw += camSpeed * deltaTime;

        // vertical rotation control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))   camPitch += camSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) camPitch -= camSpeed * deltaTime;


        if (camPitch > 85.0f)  camPitch = 85.0f;
        if (camPitch < -85.0f) camPitch = -85.0f;

        // zoom control
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) camRadius -= zoomSpeed * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) camRadius += zoomSpeed * deltaTime;

        float minZoom = (currentTarget == -1) ? 3.0f : 1.5f;
        if (camRadius < minZoom)  camRadius = minZoom;
        if (camRadius > 40.0f) camRadius = 40.0f;

        
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f); 
        if (currentTarget >= 0 && currentTarget < static_cast<int>(planets.size())) 
        {
            const auto& targetPlanet = planets[currentTarget];
            cameraTarget.x = targetPlanet.posX * std::cos(targetPlanet.orbitAngle);
            cameraTarget.z = -targetPlanet.posX * std::sin(targetPlanet.orbitAngle);
            cameraTarget.y = 0.0f;
        }


        // --- CAMERA POSITION CALCULATION ---
        float pitchRad = glm::radians(camPitch);
        float yawRad = glm::radians(camYaw);

        glm::vec3 cameraOffset;
        cameraOffset.x = camRadius * std::cos(pitchRad) * std::cos(yawRad);
        cameraOffset.y = camRadius * std::sin(pitchRad);
        cameraOffset.z = camRadius * std::cos(pitchRad) * std::sin(yawRad);

        glm::vec3 cameraPos = cameraTarget + cameraOffset;
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), CANVAS_WIDTH / CANVAS_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp); 

        glClearColor(0.019f, 0.019f, 0.039f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "vp"), 1, GL_FALSE, glm::value_ptr(proj * view));
        glUniform1i(glGetUniformLocation(shaderProgram, "texture_diffuse"), 0);

        
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));

        // --- RENDER SOL ---
        glm::mat4 sunModel = glm::mat4(1.0f);
        sunModel = glm::scale(sunModel, glm::vec3(1.3f)); 
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(sunModel));
        
        
        glUniform1i(glGetUniformLocation(shaderProgram, "isSun"), 1); 
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sunTexture);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

        // --- RENDER PLANETAS  ---
        for (auto& planet : planets) 
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, planet.orbitAngle, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::translate(model, glm::vec3(planet.posX, 0.0f, 0.0f)); 
            model = glm::rotate(model, planet.rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f)); 
            model = glm::scale(model, glm::vec3(planet.radius)); 

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            
            
            glUniform1i(glGetUniformLocation(shaderProgram, "isSun"), 0); 

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, planet.textureID);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        }

        window.display();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1, &sunTexture);
    for(auto& planet : planets) {
        glDeleteTextures(1, &planet.textureID);
    }
    glDeleteProgram(shaderProgram);

    return 0;
}