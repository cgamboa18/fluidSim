#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include<SFML\OpenGl.hpp>
#include<SFML\Main.hpp>
#include<SFML\System.hpp>

#include "Yaml.hpp"

class Particle // holds all attributes of a particle
{
public:
    double xPos;
    double yPos;

    double xVelocity;
    double yVelocity;

    Particle(double x = 0, double y = 0, double vx = 0, double vy = 0)
    {
        xPos = x;
        yPos = y;
        xVelocity = vx;
        yVelocity = vy;
    }

    void setParticleAttributes(double x, double y, double vx, double vy) // allow for particle attributes to be set customly
    {
        xPos = x;
        yPos = y;
        xVelocity = vx;
        yVelocity = vy;
    }

};

class Bound // holds the boudaries of the window
{
public:
    double upper;
    double lower;
    double left;
    double right;


    Bound(double upperY, double lowerY, double leftX, double rightX)
    {
        upper = upperY;
        lower = lowerY;
        left = leftX;
        right = rightX;
    }


};

class ConfigurationData // holds all data parsed from config file
{
public:
    double friction;
    double lossMultiplier;
    double gravityAcceleration;
    double influenceRadius;
    int particleFlowControl;
    bool showCenter;
    int gradDivisions;
    float particleDisplaySize;
    double viscosityMultiplier;

    ConfigurationData()
    {
        Yaml::Node root;
        Yaml::Parse(root, "config.txt");

        friction = root["friction"].As<double>();
        lossMultiplier = root["lossMultiplier"].As<double>();
        gravityAcceleration = root["gravityAcceleration"].As<double>();
        influenceRadius = root["influenceRadius"].As<double>();
        particleFlowControl = root["particleFlowControl"].As<double>();
        showCenter = root["showCenter"].As<bool>();
        gradDivisions = root["gradDivisions"].As<int>();
        particleDisplaySize = root["particleDisplaySize"].As<float>();
        viscosityMultiplier = root["viscosityMultiplier"].As<double>();
    }
};

void calculatePosition(Particle& particle, sf::Time& elapsedTime, Bound& boundaries, Particle list[], int index, int numElements);

int distanceSign(double distance);