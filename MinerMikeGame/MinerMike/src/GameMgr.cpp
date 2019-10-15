/*
 * GameMgr.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */

#include <Engine.h>

#include <EntityMgr.h>
#include <GameMgr.h>
#include <GfxMgr.h>

#include <iostream>
#include <Types381.h>

#include <OgreOverlay.h>
#include <OgreSceneNode.h>


GameMgr::GameMgr(Engine *engine): Mgr(engine) {
	cameraNode = 0;
}

GameMgr::~GameMgr() {
	// TODO Auto-generated destructor stub
}

void GameMgr::Init(){

}

void GameMgr::LoadLevel(){


	  //engine->gfxMgr->destroySplash();

	  engine->gfxMgr->mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 1));

	  engine->gfxMgr->mCamera->lookAt(Ogre::Vector3(0, -180, 0));
	  Ogre::Light* light = engine->gfxMgr->mSceneMgr->createLight("MainLight");
	  light->setPosition(20.0, 10000.0, 50.0);

	  // A node to attach the camera to so we can move the camera node instead of the camera.
	  cameraNode = engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode();
	  cameraNode->setPosition(0, 1000, 200);
	  cameraNode->attachObject(engine->gfxMgr->mCamera);

	  engine->gfxMgr->MakeSky();
	  engine->gfxMgr->MakeGround();
	  MakeEntities();
}

void GameMgr::MakeEntities(){
	Ogre::Vector3 pos = Ogre::Vector3(0, 0, 0);
	engine->entityMgr->CreateEntityOfTypeAtPosition(MinerType, pos);
	engine->entityMgr->entities[0]->sceneNode->scale(7,7,7);

	engine->entityMgr->SelectNextEntity(); //sets selection

	Ogre::Vector3 cubePos = Ogre::Vector3(-200, -49, 100);

	for (int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			engine->entityMgr->CreateEntityOfTypeAtPosition(DiggableType, cubePos);
			cubePos.x += 100;
		}
		cubePos.z -= 100;
		cubePos.x -= 500;
	}

	cubePos = Ogre::Vector3(1800, -49, 100);
	for (int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			engine->entityMgr->CreateEntityOfTypeAtPosition(DiggableType, cubePos);
			cubePos.x += 100;
		}
		cubePos.z -= 100;
		cubePos.x -= 500;
	}

	cubePos = Ogre::Vector3(3800, -49, 100);

	for (int i = 0; i < 6; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			engine->entityMgr->CreateEntityOfTypeAtPosition(DiggableType, cubePos);
			cubePos.x += 100;
		}
		cubePos.z -= 100;
		cubePos.x -= 600;
	}


	Ogre::Vector3 AIPos = Ogre::Vector3(0, 10, 100);
	for (int i = 0; i < 2; i++)
	{
		engine->entityMgr->CreateEntityOfTypeAtPosition(SnakeType, AIPos);
		engine->entityMgr->entities[87 + i]->originalPos = AIPos;
		AIPos.z -= 400;
	}

	AIPos = Ogre::Vector3(2000, 10, 100);
	for (int i = 0; i < 3; i++)
	{
		engine->entityMgr->CreateEntityOfTypeAtPosition(SnakeType, AIPos);
		engine->entityMgr->entities[89 + i]->originalPos = AIPos;
		AIPos.z -= 200;
	}

	AIPos = Ogre::Vector3(4000, 10, 100);
	engine->entityMgr->CreateEntityOfTypeAtPosition(SnakeType, AIPos);
	engine->entityMgr->entities[92]->originalPos = AIPos;
	AIPos.z -= 200;
	engine->entityMgr->CreateEntityOfTypeAtPosition(SnakeType, AIPos);
	engine->entityMgr->entities[93]->originalPos = AIPos;
	AIPos.z -= 100;
	engine->entityMgr->CreateEntityOfTypeAtPosition(SnakeType, AIPos);
	engine->entityMgr->entities[94]->originalPos = AIPos;
	AIPos.z -= 200;
	engine->entityMgr->CreateEntityOfTypeAtPosition(SnakeType, AIPos);
	engine->entityMgr->entities[95]->originalPos = AIPos;
}

