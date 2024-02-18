#include<iostream>
#include<random>

#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include<SFML\OpenGl.hpp>
#include<SFML\Main.hpp>
#include<SFML\System.hpp>

#include "Physics.h"
#include "Yaml.hpp"

ConfigurationData configMain = ConfigurationData();//parse config file for values

void updateParticles(sf::Time& elapsedTime, sf::RenderWindow& screen, Particle& testParticle, Bound& boundaries, Particle list[], int index, int numElements);

int main()
{

    sf::RenderWindow window(sf::VideoMode(800, 800), "FluidSim", sf::Style::Close);
    sf::Clock clock;

    Bound b1 = Bound(0, 800, 0, 800);//instatiates object to declare the boundaries

    const int maxElements = 999; //set maximum amount of particle that can be on screen
    int elements = 0;
    Particle particleList[maxElements];

    int control = 0;
    while (window.isOpen())
    {
        sf::Event event;

        static bool lock_click; // Create a bool variable for locking the click.

        control++;
        if (lock_click && control > configMain.particleFlowControl && elements != 999)
        {
            if (sf::Mouse::getPosition(window).y + configMain.influenceRadius / 1.75 < b1.lower && // Checks if cursor is in bounds to create a new particle
                sf::Mouse::getPosition(window).y - configMain.influenceRadius / 1.75 > b1.upper &&
                sf::Mouse::getPosition(window).x + configMain.influenceRadius / 1.75 < b1.right &&
                sf::Mouse::getPosition(window).x - configMain.influenceRadius / 1.75 > b1.left)
            {
                control = 0;
                elements++;

                //creates new particle on screen, sets attributes of particle
                particleList[elements - 1].setParticleAttributes(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, rand() % 5 - 5, 0);
            }
        }

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            //detects the mouse button beign pressed
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left && lock_click != true)
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
            updateParticles(elapsed, window, particleList[i], b1, particleList, i, elements); //updates the position of the particles on screen
        }
        window.display();
    }

    return 0;
}

void updateParticles(sf::Time& elapsedTime, sf::RenderWindow& screen, Particle& testParticle, Bound& boundaries, Particle list[], int index, int numElements)
{

    sf::CircleShape particleDot(configMain.particleDisplaySize);
    sf::CircleShape gradient(0);
    particleDot.setFillColor(sf::Color::Color(0, 0, 255, 255));

    calculatePosition(testParticle, elapsedTime, boundaries, list, index, numElements);

    particleDot.setPosition(testParticle.xPos - 2, testParticle.yPos - 2);
    
    //creates visual gradient of the particle
    for (int gradLayer = 1; gradLayer <= configMain.gradDivisions; gradLayer++)
    {
        double gradRadius = (configMain.influenceRadius / configMain.gradDivisions) * gradLayer;

        gradient.setRadius(gradRadius);
        gradient.setPosition(testParticle.xPos - (gradRadius), testParticle.yPos - (gradRadius));
        gradient.setFillColor(sf::Color::Color(0, 255, 0, 60 - (configMain.influenceRadius * (gradLayer / configMain.gradDivisions))));

        screen.draw(gradient);
    }
    
    if (configMain.showCenter)
    {
        screen.draw(particleDot);
    }
}