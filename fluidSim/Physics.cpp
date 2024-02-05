#include <math.h>
#include <iostream>

#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include<SFML\OpenGl.hpp>
#include<SFML\Main.hpp>
#include<SFML\System.hpp>

#include "Physics.h"
#include "ConfigReader.h"
ConfigAttributes physicsConfig = ConfigAttributes();

double friction = 0.999;
double lossMultiplier = -0.30;
double gravityAcceleration = 0.981;
double influenceRadius = physicsConfig.influenceRadiusConfig;

void calculatePosition(Particle& particle, sf::Time& elapsedTime, Bound& boundaries, Particle list[], int index, int numElements)
{
	double secondsElapsed = elapsedTime.asSeconds();
	double distanceTraveledY;
	double distanceTraveledX;	

	//Y movement ---------------------------------------
	distanceTraveledY = particle.yVelocity * secondsElapsed;
	particle.yPos = particle.yPos + distanceTraveledY;

	particle.yVelocity += gravityAcceleration;//gravity

	if (particle.yPos + influenceRadius/2 > boundaries.lower)
	{
		particle.yVelocity = particle.yVelocity * lossMultiplier;
		particle.yPos -= distanceTraveledY;

		particle.xVelocity = particle.xVelocity * friction;

	}
	else if (particle.yPos - influenceRadius/2 < boundaries.upper)
	{
		particle.yVelocity = particle.yVelocity * lossMultiplier;
		particle.yPos -= distanceTraveledY;

		particle.xVelocity = particle.xVelocity * friction;

	}


	//X movement ---------------------------------------
	distanceTraveledX = particle.xVelocity * secondsElapsed;
	particle.xPos = particle.xPos + distanceTraveledX;

	if (particle.xPos + influenceRadius/2 > boundaries.right)
	{
		particle.xVelocity = particle.xVelocity * lossMultiplier;
		particle.xPos -= distanceTraveledX;
	}
	else if (particle.xPos - influenceRadius/2 < boundaries.left)
	{
		particle.xVelocity = particle.xVelocity * lossMultiplier;
		particle.xPos -= distanceTraveledX;
	}

	//particle collision
	for (int i = 0; i < numElements; i++)
	{
		if (list[i].yPos != particle.yPos || list[i].xPos != particle.xPos) // checks if current particle is the list particle
		{

			if ((particle.yPos >= list[i].yPos - influenceRadius && particle.yPos <= list[i].yPos + influenceRadius)
				&& (particle.xPos >= list[i].xPos - influenceRadius && particle.xPos <= list[i].xPos + influenceRadius)) // checks intersection
			{

				double xDistanceFromCollider = particle.xPos - list[i].xPos;
				double yDistanceFromCollider = particle.yPos - list[i].yPos;

				double DistanceFromCollider = sqrt(pow(xDistanceFromCollider, 2) + pow(yDistanceFromCollider, 2));

				if (DistanceFromCollider != 0)
				{
					particle.xVelocity = (particle.xVelocity * 0.995) + ((50 / DistanceFromCollider) * (xDistanceFromCollider / abs(xDistanceFromCollider))); //Division by zero needs to be fixed

					particle.yVelocity = (particle.yVelocity * 0.995) + ((50 / DistanceFromCollider) * (yDistanceFromCollider / abs(yDistanceFromCollider)));
				}
				
			}
		}
	}

	
	//std::cout << particle.yPos << "\n";
}