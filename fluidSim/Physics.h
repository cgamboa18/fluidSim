//All physics related functions will be declared here
#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include<SFML\OpenGl.hpp>
#include<SFML\Main.hpp>
#include<SFML\System.hpp>

class Particle
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

    void setParticleAttributes(double x, double y, double vx, double vy)
    {
        xPos = x;
        yPos = y;
        xVelocity = vx;
        yVelocity = vy;
    }

};

class Bound
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

void calculatePosition(Particle& particle, sf::Time& elapsedTime, Bound& boundaries, Particle list[], int index, int numElements);