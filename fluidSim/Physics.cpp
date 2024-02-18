#include <math.h>
#include <iostream>

#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include<SFML\OpenGl.hpp>
#include<SFML\Main.hpp>
#include<SFML\System.hpp>

#include "Physics.h"
#include "Yaml.hpp"

ConfigurationData configPhysics = ConfigurationData();//parse config file for values

void calculatePosition(Particle& particle, sf::Time& elapsedTime, Bound& boundaries, Particle list[], int index, int numElements)
{
	double secondsElapsed = elapsedTime.asSeconds();
	double distanceTraveledY;
	double distanceTraveledX;	

	//Y movement ---------------------------------------
	distanceTraveledY = particle.yVelocity * secondsElapsed;
	particle.yPos = particle.yPos + distanceTraveledY;

	particle.yVelocity += configPhysics.gravityAcceleration;//gravity

	//Boundaries
	if (particle.yPos + configPhysics.influenceRadius/2 > boundaries.lower)
	{
		particle.yVelocity = particle.yVelocity * configPhysics.lossMultiplier;
		particle.yPos -= distanceTraveledY;

		particle.xVelocity = particle.xVelocity * configPhysics.friction;

	}
	else if (particle.yPos - configPhysics.influenceRadius/2 < boundaries.upper)
	{
		particle.yVelocity = particle.yVelocity * configPhysics.lossMultiplier;
		particle.yPos -= distanceTraveledY * 1.25;

		particle.xVelocity = particle.xVelocity * configPhysics.friction;

	}


	//X movement ---------------------------------------
	distanceTraveledX = particle.xVelocity * secondsElapsed;
	particle.xPos = particle.xPos + distanceTraveledX;

	//boundaries
	if (particle.xPos + configPhysics.influenceRadius/2 > boundaries.right)
	{
		particle.xVelocity = particle.xVelocity * configPhysics.lossMultiplier;
		particle.xPos -= distanceTraveledX;
	}
	else if (particle.xPos - configPhysics.influenceRadius/2 < boundaries.left)
	{
		particle.xVelocity = particle.xVelocity * configPhysics.lossMultiplier;
		particle.xPos -= distanceTraveledX;
	}

	//particle collision
	for (int i = 0; i < numElements; i++) //loops through each particle in existance
	{
		if (list[i].yPos != particle.yPos || list[i].xPos != particle.xPos) // checks if current particle is the current list particle
		{

			if ((particle.yPos >= list[i].yPos - configPhysics.influenceRadius && particle.yPos <= list[i].yPos + configPhysics.influenceRadius)
				&& (particle.xPos >= list[i].xPos - configPhysics.influenceRadius && particle.xPos <= list[i].xPos + configPhysics.influenceRadius)) // checks if particle is intersecting another
			{
				double xDistanceFromCollider = particle.xPos - list[i].xPos;
				double yDistanceFromCollider = particle.yPos - list[i].yPos;

				double DistanceFromCollider = sqrt(pow(xDistanceFromCollider, 2) + pow(yDistanceFromCollider, 2)); // gives the distance between current particle and intersecting particle

				if (DistanceFromCollider != 0)
				{
					// main calculation for collision physics between particle
					particle.xVelocity = (particle.xVelocity * 0.995) + ((configPhysics.viscosityMultiplier / DistanceFromCollider) * distanceSign(xDistanceFromCollider));

					particle.yVelocity = (particle.yVelocity * 0.995) + ((configPhysics.viscosityMultiplier / DistanceFromCollider) * distanceSign(yDistanceFromCollider));
				}

			}
		}

	}
}

//determines the sign of a double distance: returns 1, -1, or 0
int distanceSign(double distance)
{
	int sign;

	if (distance != 0)
	{
		sign = distance / abs(distance);
	}
	else
	{
		sign = 0;
	}


	return sign;
}