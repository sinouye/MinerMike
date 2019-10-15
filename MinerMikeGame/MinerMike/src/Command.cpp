/*
 * Command.cpp
 *
 *  Created on: Apr 7, 2019
 *      Author: sinouye
 */
#include <Command.h>
#include <Utils.h>

Command::Command(Entity381* ent, COMMAND_TYPE ct)
{
	entity = ent;
	commandType = ct;
}

Command::~Command()
{

}

void Command::init()
{

}

void Command::tick(float dt)
{

}

bool Command::done()
{
	return true;
}

MoveTo::MoveTo(Entity381* ent, Ogre::Vector3 location) : Command(ent, MOVETO)
{
	targetLocation = location;
	MOVE_DISTANCE_THRESHOLD = 50.0f;
}

MoveTo::~MoveTo()
{

}

void MoveTo::init()
{
	if(!done())
	{
		if(Ogre::Math::ATan2(entity->position.z - targetLocation.z, entity->position.x - targetLocation.x).valueDegrees() + 180 > 360)
			entity->desiredHeading = Ogre::Math::ATan2(entity->position.z - targetLocation.z, entity->position.x - targetLocation.x).valueDegrees() - 180;
		else
			entity->desiredHeading = Ogre::Math::ATan2(entity->position.z - targetLocation.z, entity->position.x - targetLocation.x).valueDegrees() + 180;
	}
}

void MoveTo::tick(float dt)
{
	if(!done())
	{
		if ((entity->position - targetLocation).length() >= MOVE_DISTANCE_THRESHOLD + 150)
		{
			entity->desiredSpeed = entity->maxSpeed;
		}
		else if ((entity->position - targetLocation).length() >= MOVE_DISTANCE_THRESHOLD + 100)
		{
			if(entity->maxSpeed - entity->acceleration / 2 > 0)
				entity->desiredSpeed = entity->maxSpeed - entity->acceleration / 2;
		}
		else if ((entity->position - targetLocation).length() >= MOVE_DISTANCE_THRESHOLD + 75)
		{
			entity->desiredSpeed = entity->acceleration;
		}

		if(entity->desiredSpeed > entity->speed){
			entity->speed += entity->acceleration * dt;
		} else if (entity->desiredSpeed < entity->speed){
			entity->speed -= entity->acceleration * dt;
		}
		entity->speed = Clamp(entity->minSpeed, entity->maxSpeed, entity->speed);

		if(entity->desiredHeading > entity->heading){
			if(entity->desiredHeading - entity->heading > 180)
				entity->heading -= entity->turnRate * dt;
			else
				entity->heading += entity->turnRate * dt;
		} else if (entity->desiredHeading < entity->heading){
			if(entity->desiredHeading - entity->heading < -180)
				entity->heading += entity->turnRate * dt;
			else
				entity->heading -= entity->turnRate * dt;;
		}

		entity->heading = FixAngle(entity->heading);

		entity->velocity.y = 0.0f; // just to be safe, we do not want ships in the air.
		entity->velocity.x = Ogre::Math::Cos(Ogre::Degree(entity->heading)) * entity->speed; //adjacent/hyp
		entity->velocity.z = Ogre::Math::Sin(Ogre::Degree(entity->heading)) * entity->speed; //opposite/hyp

		//This does not change!
		entity->position = entity->position + entity->velocity * dt;
	}
	else
	{
		/*entity->desiredHeading = Ogre::Math::ATan2(entity->position.z - targetLocation.z, entity->position.x - targetLocation.x).valueDegrees() + 180;
		entity->desiredSpeed = 0.0f;

		if(entity->desiredSpeed > entity->speed){
			entity->speed += entity->acceleration * dt;
		} else if (entity->desiredSpeed < entity->speed){
			entity->speed -= entity->acceleration * dt;
		}
		entity->speed = Clamp(entity->minSpeed, entity->maxSpeed, entity->speed);

		entity->velocity.y = 0.0f; // just to be safe, we do not want ships in the air.
		entity->velocity.x = 0.0f;
		entity->velocity.z = 0.0f;

		//This does not change!
		entity->position = entity->position + entity->velocity * dt;*/
		entity->velocity = Ogre::Vector3::ZERO;
		entity->desiredSpeed = entity->speed = 0;
		entity->desiredHeading = entity->heading;
		entity->position = entity->position + entity->velocity * dt;
	}
}

bool MoveTo::done()
{
	if((entity->position - targetLocation).length() <= MOVE_DISTANCE_THRESHOLD)
		return true;
	else
		return false;
}
Intercept::Intercept(Entity381* ent, Entity381* target) : Command(ent, INTERCEPT)
{
	targetEntity = target;
	MOVE_DISTANCE_THRESHOLD = 50.0f;
}
Intercept::~Intercept(){	}
void Intercept::init()
{
	if(!done())
	{
		if(Ogre::Math::ATan2(entity->position.z - targetEntity->position.z, entity->position.x - targetEntity->position.x).valueDegrees() + 180 > 360)
			entity->desiredHeading = Ogre::Math::ATan2(entity->position.z - targetEntity->position.z, entity->position.x - targetEntity->position.x).valueDegrees() - 180;
		else
			entity->desiredHeading = Ogre::Math::ATan2(entity->position.z - targetEntity->position.z, entity->position.x - targetEntity->position.x).valueDegrees() + 180;
	}
}
void Intercept::tick(float dt)
{
	if(!done())
		{
			if ((entity->position - targetEntity->position).length() >= MOVE_DISTANCE_THRESHOLD + 150)
			{
				entity->desiredSpeed = entity->maxSpeed;
			}
			else if ((entity->position - targetEntity->position).length() >= MOVE_DISTANCE_THRESHOLD + 100)
			{
				if(entity->maxSpeed - entity->acceleration / 2 > 0)
					entity->desiredSpeed = entity->maxSpeed - entity->acceleration / 2;
			}
			else if ((entity->position - targetEntity->position).length() >= MOVE_DISTANCE_THRESHOLD + 75)
			{
				entity->desiredSpeed = entity->acceleration;
			}

			if(Ogre::Math::ATan2(entity->position.z - targetEntity->position.z, entity->position.x - targetEntity->position.x).valueDegrees() + 180 > 360)
						entity->desiredHeading = Ogre::Math::ATan2(entity->position.z - targetEntity->position.z, entity->position.x - targetEntity->position.x).valueDegrees() - 180;
					else
						entity->desiredHeading = Ogre::Math::ATan2(entity->position.z - targetEntity->position.z, entity->position.x - targetEntity->position.x).valueDegrees() + 180;

			if(entity->desiredSpeed > entity->speed){
				entity->speed += entity->acceleration * dt;
			} else if (entity->desiredSpeed < entity->speed){
				entity->speed -= entity->acceleration * dt;
			}
			entity->speed = Clamp(entity->minSpeed, entity->maxSpeed, entity->speed);

			if(entity->desiredHeading > entity->heading){
				if(entity->desiredHeading - entity->heading > 180)
					entity->heading -= entity->turnRate * dt;
				else
					entity->heading += entity->turnRate * dt;
			} else if (entity->desiredHeading < entity->heading){
				if(entity->desiredHeading - entity->heading < -180)
					entity->heading += entity->turnRate * dt;
				else
					entity->heading -= entity->turnRate * dt;;
			}

			entity->heading = FixAngle(entity->heading);

			entity->velocity.y = 0.0f; // just to be safe, we do not want ships in the air.
			entity->velocity.x = Ogre::Math::Cos(Ogre::Degree(entity->heading)) * entity->speed; //adjacent/hyp
			entity->velocity.z = Ogre::Math::Sin(Ogre::Degree(entity->heading)) * entity->speed; //opposite/hyp

			//This does not change!
			entity->position = entity->position + entity->velocity * dt;
		}
		else
		{
			/*entity->desiredHeading = Ogre::Math::ATan2(entity->position.z - targetLocation.z, entity->position.x - targetLocation.x).valueDegrees() + 180;
			entity->desiredSpeed = 0.0f;

			if(entity->desiredSpeed > entity->speed){
				entity->speed += entity->acceleration * dt;
			} else if (entity->desiredSpeed < entity->speed){
				entity->speed -= entity->acceleration * dt;
			}
			entity->speed = Clamp(entity->minSpeed, entity->maxSpeed, entity->speed);

			entity->velocity.y = 0.0f; // just to be safe, we do not want ships in the air.
			entity->velocity.x = 0.0f;
			entity->velocity.z = 0.0f;

			//This does not change!
			entity->position = entity->position + entity->velocity * dt;*/
			entity->velocity = Ogre::Vector3::ZERO;
			entity->desiredSpeed = entity->speed = 0;
			entity->desiredHeading = entity->heading;
			entity->position = entity->position + entity->velocity * dt;
		}
}
bool Intercept::done()
{
	if((entity->position - targetEntity->position).length() <= MOVE_DISTANCE_THRESHOLD)
			return true;
		else
			return false;
}

