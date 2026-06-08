#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

int main() 
{
    
    const unsigned int WINDOW_WIDTH = 1280;
    const unsigned int WINDOW_HEIGHT = 720;
    
    
    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Star Wars Planetary System - Tappa 01");
    window.setFramerateLimit(60);

    std::cout << "========================================================\n";
    std::cout << "  Star Wars Planetary System - Tappa 01 Initialized\n";
    std::cout << "  Target Environment: Multiplatform (Win/Mac/Linux)\n";
    std::cout << "========================================================\n";

    
    while (window.isOpen()) 
    {
        
        while (const std::optional<sf::Event> event = window.pollEvent()) 
        {
            
            if (event->is<sf::Event::Closed>()) 
            {
                std::cout << "Window close event detected.\n";
                window.close();
            }
            
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) 
            {
                if (keyPressed->code == sf::Keyboard::Key::Escape) 
                {
                    std::cout << "Escape key pressed. Closing simulation window.\n";
                    window.close();
                }
            }
        }

        
        window.clear(sf::Color(5, 5, 10));

        
        window.display();
    }

    std::cout << "Application finished execution safely.\n";
    return 0;
}