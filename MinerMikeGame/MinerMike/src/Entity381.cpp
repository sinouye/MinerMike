/*
 * Entity381.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: sushil
 */

#include <GfxMgr.h>
#include <iostream>
#include<Entity381.h>
#include<Physics2D.h>

std::string IntToString(int x){
	char tmp[10000];
	sprintf(tmp, "%i", x);
	return std::string(tmp);
}

Entity381::Entity381(Engine *engine, Ogre::Vector3 pos, int ident){

	this->engine = engine;

	entityType = None;
	meshfilename = std::string();
	position = pos;
	velocity = Ogre::Vector3(0, 0, 0);
	identity = ident;
	isSelected = false;

	name = "Entity381"; //meshfname + IntToString(identity);
/*
	ogreEntity = engine->gfxMgr->mSceneMgr->createEntity(meshfilename);
	sceneNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode(pos);
	sceneNode->attachObject(ogreEntity);*/
	sceneNode = 0;
	ogreEntity = 0;

	Physics2D* phx = new Physics2D(this);
	aspects.push_back((Aspect*) phx);
	Renderable * renderable = new Renderable(this);
	aspects.push_back((Aspect*)renderable);
	ai = new UnitAI(this);
	aspects.push_back((Aspect*)ai);

	this->acceleration = 0;
	this->desiredHeading = this->heading = 0;
	this->turnRate = 0;
	this->desiredSpeed = this->speed = 0;
	this->minSpeed = this->maxSpeed = 0;

	cmdType = 0;

	this->money = 0;
}

Entity381::~Entity381(){

}

void Entity381::Init(){

	name = meshfilename + IntToString(identity);
	ogreEntity = engine->gfxMgr->mSceneMgr->createEntity(meshfilename);
	sceneNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode(position);
	sceneNode->attachObject(ogreEntity);

	if(entityType == DiggableType) {
		ogreEntity->setMaterialName("Examples/GrassFloor");
	}
	else if (entityType == SolidType)
	{
		ogreEntity->setMaterialName("Examples/BeachStones");
	}
}

void Entity381::Tick(float dt){
	for(unsigned int i = 0; i < aspects.size(); i++){
		aspects[i]->Tick(dt);
	}
}

void Entity381::moneyBronze()
{
	money += 25;
}
void Entity381::moneySilver()
{
	money += 50;
}
void Entity381::moneyGold()
{
	money += 100;
}

void Entity381::moneyTreasure()
{
	money += 1000000000000;
}

//-------------------------------------------------------------------------------------------------------------------------------
Miner::Miner(Engine *engine, Ogre::Vector3 pos, int ident):
		Entity381(engine, pos, ident){
	meshfilename = "Snowman.mesh";
	entityType = MinerType;
	this->minSpeed = 0;
	this->maxSpeed = 75.0f;//meters per second...
	this->acceleration = 20.0f; // slow
	this->turnRate = 90.0f;
}

Miner::~Miner(){

}
//-------------------------------------------------------------------------------------------------------------------------------
Diggable::Diggable(Engine *engine, Ogre::Vector3 pos, int ident):
		Entity381(engine, pos, ident){
	meshfilename = "cube.mesh";
	entityType = DiggableType;

	this->minSpeed = 0;
	this->maxSpeed = 0.0f;
	this->acceleration = 0.0f;
	this->turnRate = 0.0f;
}

Diggable::~Diggable(){

}
//-------------------------------------------------------------------------------------------------------------------------------
Solid::Solid(Engine *engine, Ogre::Vector3 pos, int ident):
		Entity381(engine, pos, ident){
	meshfilename = "cube.mesh";
	entityType = SolidType;

	this->minSpeed = 0;
	this->maxSpeed = 0.0f;
	this->acceleration = 0.0f;
	this->turnRate = 0.0f;
}

Solid::~Solid(){

}
//-------------------------------------------------------------------------------------------------------------------------------
Snake::Snake(Engine *engine, Ogre::Vector3 pos, int ident):
		Entity381(engine, pos, ident){
	meshfilename = "penguin.mesh";
	entityType = SnakeType;
	this->minSpeed = 0;
	this->maxSpeed = 70.0f;//meters per second...
	this->acceleration = 20.0f; // slow
	this->turnRate = 90.0f;
}

Snake::~Snake(){

}
