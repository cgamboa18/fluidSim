#include<iostream> //this is a test
#include<random>

#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include<SFML\OpenGl.hpp>
#include<SFML\Main.hpp>
#include<SFML\System.hpp>

#include "Physics.h"
#include "ConfigReader.h"
ConfigAttributes mainConfig = ConfigAttributes();


void updateParticles(sf::Time& elapsedTime, sf::RenderWindow& screen, Particle& testParticle, Bound& boundaries, Particle list[], int index, int numElements);

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 800), "FluidSim", sf::Style::Close);
    sf::Clock clock;

    Bound b1 = Bound(0, 800, 0, 800);

    const int maxElements = 999;
    int elements = 0;
    Particle particleList[maxElements];

    while (window.isOpen())
    {
        sf::Event event;

        static bool lock_click; // Create a bool variable for locking the click.
        if (lock_click)
        {
            elements++;
            particleList[elements - 1].setParticleAttributes(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, rand() % 200 - 100, rand() % 200 - 100);
        }

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            
            if (event.type == sf::Event::MouseButtonPressed) //Mouse button Pressed
            {
                if (event.mouseButton.button == sf::Mouse::Left && lock_click != true) //specifies
                {
                    lock_click = true; 
                }
            }

            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    lock_click = false;
                }
            }


        }
        window.clear();

        sf::Time elapsed = clock.restart();

        for (int i = 0; i < elements; i++)
        {
            updateParticles(elapsed, window, particleList[i], b1, particleList, i, elements);
        }
        window.display();
    }

    return 0;
}

void updateParticles(sf::Time& elapsedTime, sf::RenderWindow& screen, Particle& testParticle, Bound& boundaries, Particle list[], int index, int numElements)
{
    sf::CircleShape particleDot(2.f);
    sf::CircleShape gradient(0);//new
    particleDot.setFillColor(sf::Color::Color(0, 0, 255, 255));

    calculatePosition(testParticle, elapsedTime, boundaries, list, index, numElements);

    particleDot.setPosition(testParticle.xPos - 2, testParticle.yPos - 2);
    
    double gradDivisions = 10;
    for (int gradLayer = 1; gradLayer <= gradDivisions; gradLayer++)
    {
        double gradRadius = (30 / gradDivisions) * gradLayer;

        gradient.setRadius(gradRadius);
        gradient.setPosition(testParticle.xPos - (gradRadius), testParticle.yPos - (gradRadius));
        gradient.setFillColor(sf::Color::Color(0, 255, 0, 60 - (30 * (gradLayer / gradDivisions))));

        screen.draw(gradient);
    }
    
    screen.draw(particleDot);
}