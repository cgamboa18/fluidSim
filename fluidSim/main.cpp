#include<iostream>
#include<random>

#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include<SFML\OpenGl.hpp>
#include<SFML\Main.hpp>
#include<SFML\System.hpp>

#include "Physics.h"
#include "Yaml.hpp"

void updateParticles(sf::Time& elapsedTime, sf::RenderWindow& screen, Particle& testParticle, Bound& boundaries, Particle list[], int index, int numElements);

int main()
{
    Yaml::Node root;
    Yaml::Parse(root, "config.txt");
    double particleFlowControl = root["particleFlowControl"].As<double>();
    //---------------------------------------------------

    sf::RenderWindow window(sf::VideoMode(800, 800), "FluidSim", sf::Style::Close);
    sf::Clock clock;

    Bound b1 = Bound(0, 800, 0, 800);

    const int maxElements = 999;
    int elements = 0;
    Particle particleList[maxElements];

    int control = 0;
    while (window.isOpen())
    {
        sf::Event event;

        static bool lock_click; // Create a bool variable for locking the click.

        control++;
        if (lock_click && control > particleFlowControl)
        {
            control = 0;
            elements++;
            particleList[elements - 1].setParticleAttributes(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 0, 0);
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
    Yaml::Node root;//parse config
    Yaml::Parse(root, "config.txt");
    bool showCenter = root["showCenter"].As<bool>();
    double gradDivisions = root["gradDivisions"].As<double>();
    double particleDisplaySize = root["particleDisplaySize"].As<float>();
    //---------------------------------

    sf::CircleShape particleDot(particleDisplaySize);
    sf::CircleShape gradient(0);
    particleDot.setFillColor(sf::Color::Color(0, 0, 255, 255));

    calculatePosition(testParticle, elapsedTime, boundaries, list, index, numElements);

    particleDot.setPosition(testParticle.xPos - 2, testParticle.yPos - 2);
    
    for (int gradLayer = 1; gradLayer <= gradDivisions; gradLayer++)
    {
        double gradRadius = (30 / gradDivisions) * gradLayer;

        gradient.setRadius(gradRadius);
        gradient.setPosition(testParticle.xPos - (gradRadius), testParticle.yPos - (gradRadius));
        gradient.setFillColor(sf::Color::Color(0, 255, 0, 60 - (30 * (gradLayer / gradDivisions))));

        screen.draw(gradient);
    }
    
    if (showCenter)
    {
        screen.draw(particleDot);
    }
}