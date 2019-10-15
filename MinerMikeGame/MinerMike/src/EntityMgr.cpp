/*
 * EntityMgr.cpp
 *
 *  Created on: Feb 7, 2018
 *      Author: sushil
 */

#include <EntityMgr.h>
#include <Engine.h>
#include <iostream>
#include <UiMgr.h>

EntityMgr::EntityMgr(Engine *eng): Mgr(eng){
	selectedEntity = 0;
	interceptEntity = 0;
	count = 0;
	selectedEntityIndex = -1;
	selectedFlyingEntity = 0;
	interceptEntityIndex = -1;
	snakeCmd = false;

	srand((unsigned) time(0));
	treasureBlock = rand() % 36 + 1;
}

EntityMgr::~EntityMgr(){

}


void EntityMgr::SelectNextEntity(){
	if(selectedEntityIndex >= count - 1) {
		selectedEntityIndex = 0;
	} else {
		selectedEntityIndex++;
	}
	Select(selectedEntityIndex);
/*	if(selectedEntity != 0)
		selectedEntity->isSelected = false;
	selectedEntity = entities[selectedEntityIndex];
	selectedEntity->isSelected = true;
	SetSelectedFlyingEntity();*/
}

void EntityMgr::SetSelectedFlyingEntity(){
	FlyingEntity381 *tmp = dynamic_cast<FlyingEntity381 *>(selectedEntity);
	if(tmp != 0){
		selectedFlyingEntity = tmp;
	}	else {
		selectedFlyingEntity = 0;
	}
}


void EntityMgr::Select(int i){
	if(i >= 0 && i < count){
		if(selectedEntity != 0)
			selectedEntity->isSelected = false;
		selectedEntityIndex = i;
		selectedEntity = entities[i];
		selectedEntity->isSelected = true;
	}
}

void EntityMgr::DigBlock(int i){
	if(entities[i]->entityType == DiggableType) {
		entities[i]->isDug = true;
		entities[i]->position.y = -100000;

		int moneyType = rand() % 100 + 1;
		if(i == 51 + treasureBlock)
		{
			entities[0]->moneyTreasure();
			engine->uiMgr->infoLabel2->setCaption("YOU FOUND THE TREASURE! +$ONE TRILLION!");
			engine->uiMgr->endGame = true;
		}
		else if(moneyType % 13 == 0)
		{
			entities[0]->moneyGold();
			engine->uiMgr->infoLabel2->setCaption("You found gold! +$100");
		}
		else if (moneyType % 5 == 0)
		{
			entities[0]->moneySilver();
			engine->uiMgr->infoLabel2->setCaption("You found silver! +$50");
		}
		else
		{
			entities[0]->moneyBronze();
			engine->uiMgr->infoLabel2->setCaption("You found bronze! +$25");
		}
	}
}
void EntityMgr::SelectInterceptEntity(int i){
	if(i >= 0 && i < count){
		if(interceptEntity != 0)
			interceptEntity->isSelected = false;
		interceptEntityIndex = i;
		interceptEntity = entities[i];
	}

	/*disd
	interceptEntity = entities[0];
	Ogre::Real minDistance = Ogre::Vector3(interceptEntity->position - mousePos).length();
	Ogre::Real currentDistance;
	int minIndex = 0;

	for(int index = 1; index < count; index++) {
		interceptEntity = entities[index];
		currentDistance = Ogre::Vector3(interceptEntity->position - mousePos).length();
		if (currentDistance < minDistance) {
			minIndex = index;
			minDistance = currentDistance;
		}
	}
	interceptEntityIndex = minIndex;
	interceptEntity = entities[selectedEntityIndex]; */
}


void EntityMgr::CreateEntityOfTypeAtPosition(EntityTypes entType, Ogre::Vector3 pos){

	Entity381 * ent;
	switch(entType){
	case MinerType:
		ent = (Entity381*) (new Miner(engine, pos, count++));
		break;
	case DiggableType:
		ent = (Entity381*) (new Diggable(engine, pos, count++));
		break;
	case SolidType:
		ent = (Entity381*) (new Solid(engine, pos, count++));
		break;
	case SnakeType:
		ent = (Entity381*) (new Snake(engine, pos, count++));
		break;
	default:
		ent = (Entity381*) (new Solid(engine, pos, count++));//CreateEntity("robot.mesh", pos);
		break;
	}
	ent->Init();
	entities.push_back(ent);

}

void EntityMgr::Tick(float dt){
	for(int i = 0; i < count; i++){
		if(entities[i]->entityType == SnakeType){
			if(entities[i]->cmdType == 0) {
				if(snakeCmd){
					MoveTo *mT = new MoveTo(entities[i], Ogre::Vector3(entities[i]->originalPos.x - 300,0,entities[i]->originalPos.z));
					entities[i]->ai->SetCommand((Command*) mT);
				}
				else{
					MoveTo *mT = new MoveTo(entities[i], Ogre::Vector3(entities[i]->originalPos.x + 300,0,entities[i]->originalPos.z));
					entities[i]->ai->SetCommand((Command*) mT);
				}
				if(entities[i]->ai->commands.front()->done()){
					snakeCmd = !snakeCmd;
				} //does not go backwards, fix by tomorrow

				if((entities[i]->position - entities[0]->position).length() <= 50)
				{
					if(!(engine->uiMgr->isSplashOpen))
					{
						engine->uiMgr->isSplashOpen = true;
						engine->inputMgr->alreadyDeleted = false;
						//engine->gfxMgr->createSplash2();
						engine->uiMgr->loadGameOver();
					}
				}
			}
		}
		entities[i]->Tick(dt);
	}
}

