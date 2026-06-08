#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct PlanetProxy 
{
    std::string name;
    sf::CircleShape shape;
};

int main() 
{
    
    const float CANVAS_WIDTH = 1280.0f;
    const float CANVAS_HEIGHT = 720.0f;
    
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Star Wars Planetary System - Tappa 02");
    window.setFramerateLimit(60);

    
    sf::View view(sf::FloatRect({0.f, 0.f}, {CANVAS_WIDTH, CANVAS_HEIGHT}));
    window.setView(view);

    std::cout << "========================================================\n";
    std::cout << "  Star Wars Planetary System - Tappa 02 Giant Scale\n";
    std::cout << "  Layout: Centered Sun, 2 Giant Planets Left, 2 Right\n";
    std::cout << "========================================================\n";

    // Center of the universe
    sf::Vector2f center(CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f);

    // 1. Central Star 
    float sunRadius = 90.0f; 
    sf::CircleShape sunShape(sunRadius);
    sunShape.setFillColor(sf::Color(255, 140, 0)); 
    sunShape.setOrigin({sunRadius, sunRadius});     
    sunShape.setPosition(center);                   

    
    std::vector<PlanetProxy> planets;

    // --- LEFT SIDE PLANETS 
    
    // Planet 1: Mustafar
    float mRadius = 45.0f; 
    sf::CircleShape mShape(mRadius);
    mShape.setFillColor(sf::Color(210, 40, 40));
    mShape.setOrigin({mRadius, mRadius});
    mShape.setPosition(center + sf::Vector2f(-480.0f, 0.0f)); 
    planets.push_back({"Mustafar", mShape});

    // Planet 2: Tatooine 
    float tRadius = 40.0f;
    sf::CircleShape tShape(tRadius);
    tShape.setFillColor(sf::Color(235, 210, 150));
    tShape.setOrigin({tRadius, tRadius});
    tShape.setPosition(center + sf::Vector2f(-240.0f, 0.0f)); 
    planets.push_back({"Tatooine", tShape});

    // --- RIGHT SIDE PLANETS 

    // Planet 3: Hoth 
    float hRadius = 38.0f;
    sf::CircleShape hShape(hRadius);
    hShape.setFillColor(sf::Color(180, 220, 240));
    hShape.setOrigin({hRadius, hRadius});
    hShape.setPosition(center + sf::Vector2f(240.0f, 0.0f));  
    planets.push_back({"Hoth", hShape});

    // Planet 4: Endor  
    float eRadius = 48.0f;
    sf::CircleShape eShape(eRadius);
    eShape.setFillColor(sf::Color(34, 139, 34));
    eShape.setOrigin({eRadius, eRadius});
    eShape.setPosition(center + sf::Vector2f(480.0f, 0.0f));  
    planets.push_back({"Endor", eShape});


    while (window.isOpen()) 
    {
        while (const std::optional<sf::Event> event = window.pollEvent()) 
        {
            if (event->is<sf::Event::Closed>()) 
            {
                window.close();
            }
            
            
            if (const auto* resizeEvent = event->getIf<sf::Event::Resized>())
            {
                float windowWidth = static_cast<float>(resizeEvent->size.x);
                float windowHeight = static_cast<float>(resizeEvent->size.y);
                
                float minScale = std::min(windowWidth / CANVAS_WIDTH, windowHeight / CANVAS_HEIGHT);
                float viewportWidth = (CANVAS_WIDTH * minScale) / windowWidth;
                float viewportHeight = (CANVAS_HEIGHT * minScale) / windowHeight;
                float viewportX = (1.0f - viewportWidth) / 2.0f;
                float viewportY = (1.0f - viewportHeight) / 2.0f;

                view.setViewport(sf::FloatRect({viewportX, viewportY}, {viewportWidth, viewportHeight}));
                window.setView(view);
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) 
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape) 
                {
                    window.close();
                }
            }
        }

        window.clear(sf::Color(5, 5, 10));

        
        window.draw(sunShape);
        for (const auto& planet : planets) 
        {
            window.draw(planet.shape);
        }

        window.display();
    }

    return 0;
}