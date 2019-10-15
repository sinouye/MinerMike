/*
 * UiMgr.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: chad
 */
#include <cfloat>
#include <UiMgr.h>
#include <Engine.h>
#include <GfxMgr.h>
#include <InputMgr.h>
#include <EntityMgr.h>
#include <Types381.h>
#include <GameMgr.h>
#include <GfxMgr.h>
#include <iostream>

UiMgr::UiMgr(Engine* eng): Mgr(eng){
	// Initialize the OverlaySystem (changed for Ogre 1.9)
	mOverlaySystem = new Ogre::OverlaySystem();
	engine->gfxMgr->mSceneMgr->addRenderQueueListener(mOverlaySystem);
	//Ogre::WindowEventUtilities::addWindowEventListener(engine->gfxMgr->ogreRenderWindow, this);
	isSplashOpen = true;
	this->selectionDistanceSquaredThreshold = 10000;
	endGame = false;
}

UiMgr::~UiMgr(){ // before gfxMgr destructor

}

void UiMgr::Init(){
	//init sdktrays

    mInputContext.mKeyboard = engine->inputMgr->mKeyboard;
    mInputContext.mMouse = engine->inputMgr->mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", engine->gfxMgr->mWindow, mInputContext, this);
    //mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    //mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    //mTrayMgr->hideCursor();

    level = 0;

    //mTrayMgr->showBackdrop("Ogre/Compositor/GlassPass");
}

void UiMgr::Stop(){

}

void UiMgr::LoadLevel(){
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "Title", "Miner Mike", Ogre::Real(200));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "Instruction1", "Press P to play!", Ogre::Real(200));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "Instruction2", "Right click to move", Ogre::Real(200));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "Instruction3", "Left click to dig", Ogre::Real(200));

	//mTrayMgr->showBackdrop("ECGameSLENT/UI");
}

void UiMgr::Tick(float dt){
	mTrayMgr->refreshCursor();

	if(!isSplashOpen)
	{
		infoLabel->setCaption("Money: $" + std::to_string(engine->entityMgr->selectedEntity->money));

		if(level == 0 && engine->entityMgr->selectedEntity->money >= 600)
		{
			nextLevelButton();
		}

		if(level == 1 && engine->entityMgr->selectedEntity->money >= 1300)
		{
			nextLevelButton();
		}
		if(level == 2 && engine->entityMgr->selectedEntity->money >= 1000000)
		{
			nextLevelButton();
		}
	}
}

void UiMgr::windowResized(Ogre::RenderWindow* rw){
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = engine->inputMgr->mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

void UiMgr::windowClosed(Ogre::RenderWindow* rw){

}

bool UiMgr::keyPressed(const OIS::KeyEvent &arg) {
	std::cout << "Key Pressed: " << arg.key << std::endl;
	return true;
}
bool UiMgr::keyReleased(const OIS::KeyEvent &arg){
	return true;
}
bool UiMgr::mouseMoved(const OIS::MouseEvent &arg){
    if (mTrayMgr->injectMouseMove(arg)) return true;
	return false;
}
bool UiMgr::mousePressed(const OIS::MouseEvent &me, OIS::MouseButtonID mid) {
	std::cout << "mouse clicked" << std::endl;
	if (mTrayMgr->injectMouseDown(me, mid)) return true;
	return false;
}
bool UiMgr::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
    if (mTrayMgr->injectMouseUp(arg, id)) return true;
    /* normal mouse processing here... */
	return false;
}

void UiMgr::buttonHit(OgreBites::Button *b){
	if(b->getName() == "nextLevel")
	{
		if(level == 0 && engine->entityMgr->entities[0]->money >=300)
		{
			infoLabel3->setCaption("You cannot go to next level");
			engine->entityMgr->entities[0]->desiredSpeed = 0;
			engine->entityMgr->entities[0]->speed = 0;
			engine->entityMgr->entities[0]->velocity = Ogre::Vector3::ZERO;
			engine->entityMgr->entities[0]->position = Ogre::Vector3(2000, 0, 0);
			engine->gameMgr->cameraNode->setPosition(2000, 1000, 200);
			//engine->gfxMgr->mCamera->lookAt(2000, -180, 0);
			level++;
		}
		else if (level == 1 && engine->entityMgr->entities[0]->money >=700)
		{
			infoLabel3->setCaption("You cannot end the game");
			nextLevel->setCaption("End Game!");
			engine->entityMgr->entities[0]->desiredSpeed = 0;
			engine->entityMgr->entities[0]->speed = 0;
			engine->entityMgr->entities[0]->velocity = Ogre::Vector3::ZERO;
			engine->entityMgr->entities[0]->position = Ogre::Vector3(4000, 0, 0);
			engine->gameMgr->cameraNode->setPosition(4000, 1000, 200);
			//engine->gfxMgr->mCamera->lookAt(2000, -180, 0);
			level++;
		}
		else if (level == 2 && endGame == true)
		{
			engine->entityMgr->entities[0]->velocity = Ogre::Vector3::ZERO;
			isSplashOpen = true;
			engine->inputMgr->alreadyDeleted = false;
			loadGameWin();
		}
	}

	else if(b->getName() == "digTile")
	{
		int index;
		float minDistanceSquared = FLT_MAX;
		float distanceSquared; //because squareroot is expensive
		for(unsigned int i = 1; i < engine->entityMgr->entities.size(); i++){
			distanceSquared = engine->entityMgr->entities[0]->position.squaredDistance(engine->entityMgr->entities[i]->position);
			if (distanceSquared < selectionDistanceSquaredThreshold){
				if (distanceSquared < minDistanceSquared){
					index = i;
					minDistanceSquared = distanceSquared;
				}
			}
		}
		if(engine->entityMgr->entities[index])
			engine->entityMgr->DigBlock(index);
	}
}

void UiMgr::itemSelected(OgreBites::SelectMenu *m){

}

void UiMgr::loadGameUI()
{
	mTrayMgr->destroyAllWidgets();
	digTile = mTrayMgr->createButton(OgreBites::TL_RIGHT, "digTile", "DIG HERE!", 400);
	infoLabel = mTrayMgr->createLabel(OgreBites::TL_RIGHT, "infoLabel", "Money: $0", 400);
	infoLabel2 = mTrayMgr->createLabel(OgreBites::TL_RIGHT, "infoLabel2", "Nothing dug yet!", 400);
	nextLevel = mTrayMgr->createButton(OgreBites::TL_LEFT, "nextLevel", "Go to next level!", 400);
	infoLabel3 = mTrayMgr->createLabel(OgreBites::TL_LEFT, "infoLabel3", "You cannot go to next level", 500);
}

void UiMgr::nextLevelButton()
{
	if( engine->entityMgr->selectedEntity->money >= 1000000 )
		infoLabel3->setCaption("You won! You may end the game!");
	else
		infoLabel3->setCaption("You may go to next level!");

}

void UiMgr::loadGameOver(){
	mTrayMgr->destroyAllWidgets();
	engine->entityMgr->entities[0]->finalMoney = engine->entityMgr->entities[0]->money;
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "Title2", "You died poor...", Ogre::Real(400));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "FinalScore", "You earned a total of: $" + std::to_string(engine->entityMgr->entities[0]->finalMoney), Ogre::Real(400));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "Instruction4", "Press Esc to quit!", Ogre::Real(400));

	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_BOTTOMRIGHT, "Credits1", "Credits:", Ogre::Real(400));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_BOTTOMRIGHT, "Credits2", "Producer: Benjamin Estela", Ogre::Real(400));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_BOTTOMRIGHT, "Credits3", "Programmer: Sidney Inouye", Ogre::Real(400));

	//mTrayMgr->showBackdrop("ECSLENT/UI");
}

void UiMgr::loadGameWin(){
	mTrayMgr->destroyAllWidgets();
	engine->entityMgr->entities[0]->finalMoney = engine->entityMgr->entities[0]->money;
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "Title2", "CONGRATULATIONS!", Ogre::Real(400));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "FinalScore", "You earned a total of: $" + std::to_string(engine->entityMgr->entities[0]->finalMoney), Ogre::Real(400));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_CENTER, "Instruction4", "Press Esc to quit!", Ogre::Real(400));

	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_BOTTOMRIGHT, "Credits1", "Credits:", Ogre::Real(400));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_BOTTOMRIGHT, "Credits2", "Producer: Benjamin Estela", Ogre::Real(400));
	mTrayMgr->createLabel(OgreBites::TrayLocation::TL_BOTTOMRIGHT, "Credits3", "Programmer: Sidney Inouye", Ogre::Real(400));


	//mTrayMgr->showBackdrop("ECSLENT/UI");
}
