/*
 * InputMgr.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */
#include<cfloat>

#include <Engine.h>
#include <GfxMgr.h>
#include <InputMgr.h>
#include <EntityMgr.h>
#include <GameMgr.h>
#include <UiMgr.h>
#include <Utils.h>

InputMgr::InputMgr(Engine *engine) : Mgr(engine), OIS::KeyListener(), OIS::MouseListener() {

	this->mInputMgr = 0;
	this->mKeyboard = 0;
	this->mMouse = 0;
	this->keyboardTimer = keyTime;
	deltaDesiredSpeed = 10.0f;
	deltaDesiredHeading = 10.0f;
	deltaDesiredAltitude = 20;
	this->selectionDistanceSquaredThreshold = 10000;

	alreadyDeleted = false;
}

InputMgr::~InputMgr() {

}

void InputMgr::Init(){

	  OIS::ParamList pl;
	  size_t windowHandle = 0;
	  std::ostringstream windowHandleStr;

	  engine->gfxMgr->mWindow->getCustomAttribute("WINDOW", &windowHandle);
	  windowHandleStr << windowHandle;
	  pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));
	  // insert the following lines right before calling mInputSystem = OIS::InputManager::createInputSystem( paramList );
		#if defined OIS_WIN32_PLATFORM
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
		#elif defined OIS_LINUX_PLATFORM
		pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
		pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
		pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
		#endif

	  mInputMgr = OIS::InputManager::createInputSystem(pl);

	  mKeyboard = static_cast<OIS::Keyboard*>(
	    mInputMgr->createInputObject(OIS::OISKeyboard, true));
	  mMouse = static_cast<OIS::Mouse*>(
	    mInputMgr->createInputObject(OIS::OISMouse, true));

	  int left, top;
	  unsigned int width, height, depth;

	  engine->gfxMgr->mWindow->getMetrics(width, height, depth, left, top);
	  const OIS::MouseState &ms = mMouse->getMouseState();
	  ms.width = width;
	  ms.height = height;
	  mMouse->setEventCallback(this);
	  mKeyboard->setEventCallback(this);

}



void InputMgr::Stop(){
	if(mInputMgr){
		mInputMgr->destroyInputObject(mMouse);
		mInputMgr->destroyInputObject(mKeyboard);

		OIS::InputManager::destroyInputSystem(mInputMgr);
		mInputMgr = 0;
   	}
}

void InputMgr::Tick(float dt){

	mKeyboard->capture();
	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE)){
		engine->keepRunning = false;
	}
	mMouse->capture();
//	mTrayMgr->frameRenderingQueued(fe);

	UpdateCamera(dt);
	UpdateVelocityAndSelection(dt);

}

void InputMgr::UpdateCamera(float dt){
	  if (mKeyboard->isKeyDown(OIS::KC_P))
	  {
		  if(!alreadyDeleted)
		  {
			  engine->gfxMgr->destroySplash();
			  engine->uiMgr->loadGameUI();
			  engine->uiMgr->isSplashOpen = false;
			  alreadyDeleted = true;
		  }
	  }
}

void InputMgr::UpdateVelocityAndSelection(float dt){
	keyboardTimer -= dt;

	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD8)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->desiredSpeed += deltaDesiredSpeed;
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD2)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->desiredSpeed -= deltaDesiredSpeed;
	}


	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD9)){
		keyboardTimer = keyTime;
		if(engine->entityMgr->selectedFlyingEntity != 0){
			engine->entityMgr->selectedFlyingEntity->desiredAltitude += deltaDesiredAltitude;
		}
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD3)){
		keyboardTimer = keyTime;
		if(engine->entityMgr->selectedFlyingEntity != 0)
			engine->entityMgr->selectedFlyingEntity->desiredAltitude -= deltaDesiredAltitude;
	}


	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD4)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->desiredHeading -= deltaDesiredHeading;
	//turn left is decreasing degrees, turn right is increasing degrees because increasing degrees gives us the +ive Z axis
	}
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_NUMPAD6)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->desiredHeading += deltaDesiredHeading;
	}
	engine->entityMgr->selectedEntity->desiredHeading = FixAngle(engine->entityMgr->selectedEntity->desiredHeading);




	//Set velocity to zero....
	if((keyboardTimer < 0) && mKeyboard->isKeyDown(OIS::KC_SPACE)){
		keyboardTimer = keyTime;
		engine->entityMgr->selectedEntity->velocity = Ogre::Vector3::ZERO;
		engine->entityMgr->selectedEntity->desiredSpeed = engine->entityMgr->selectedEntity->speed = 0;
		engine->entityMgr->selectedEntity->desiredHeading = engine->entityMgr->selectedEntity->heading;
	}
}

void InputMgr::LoadLevel(){

}

bool InputMgr::keyPressed(const OIS::KeyEvent& ke){
	return true;
}

bool InputMgr::keyReleased(const OIS::KeyEvent& ke){
	return true;
}

bool InputMgr::mouseMoved(const OIS::MouseEvent& me){
	 if (engine->uiMgr->mTrayMgr->injectMouseMove(me)) return true;
	return true;
}

bool InputMgr::mousePressed(const OIS::MouseEvent& me, OIS::MouseButtonID mid){
	//std::cout << "Mouse pressed" << std::endl;
	if (engine->uiMgr->mTrayMgr->injectMouseDown(me, mid)) return true;

	if(!(engine->uiMgr->isSplashOpen))
	{
		if(OIS::MB_Right == mid) {
			HandleMoveTo(me);
		}

		if(OIS::MB_Left == mid) {
			HandleDig(me);
		}
	}

	return true;
}

bool InputMgr::mouseReleased(const OIS::MouseEvent& me, OIS::MouseButtonID mid){
	if (engine->uiMgr->mTrayMgr->injectMouseUp(me, mid)) return true;
	return true;
}


void InputMgr::HandleDig(const OIS::MouseEvent &me)
{
	int index = -1;
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
	if(engine->entityMgr->entities[index] && index != -1)
		engine->entityMgr->DigBlock(index);
}

void InputMgr::HandleMoveTo(const OIS::MouseEvent &me){
	std::pair<bool, Ogre::Real> mousePoint;
	int index;
	bool InterceptTrue = false;

	Ogre::Ray mouseRay = engine->gfxMgr->mCamera->getCameraToViewportRay(
			me.state.X.abs / (float)engine->gfxMgr->mCamera->getViewport()->getActualWidth(),
			me.state.Y.abs / (float)engine->gfxMgr->mCamera->getViewport()->getActualHeight()
	);

	mousePoint = mouseRay.intersects(engine->gfxMgr->oceanSurface);

	if(mousePoint.first)
	{
		Ogre::Vector3 posUnderMouse = mouseRay.getPoint(mousePoint.second);
		float minDistanceSquared = FLT_MAX;
		float distanceSquared; //because squareroot is expensive
		for(unsigned int i = 0; i < engine->entityMgr->entities.size(); i++){
			distanceSquared = posUnderMouse.squaredDistance(engine->entityMgr->entities[i]->position);
			if (distanceSquared < selectionDistanceSquaredThreshold){
				if (distanceSquared < minDistanceSquared){
					index = i;
					minDistanceSquared = distanceSquared;
				}

				InterceptTrue = true;
			}
		}
		engine->entityMgr->SelectInterceptEntity(index);

		if(!InterceptTrue)
		{
			MoveTo *mT = new MoveTo(engine->entityMgr->selectedEntity, mouseRay.getPoint(mousePoint.second));

			if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
				engine->entityMgr->selectedEntity->ai->AddCommand((Command*) mT);
			else
				engine->entityMgr->selectedEntity->ai->SetCommand((Command*) mT);
		}
		else
		{
			Intercept *inter = new Intercept(engine->entityMgr->selectedEntity, engine->entityMgr->interceptEntity);
			if(mKeyboard->isKeyDown(OIS::KC_LSHIFT))
				engine->entityMgr->selectedEntity->ai->AddCommand((Command*) inter);
			else
				engine->entityMgr->selectedEntity->ai->SetCommand((Command*) inter);
		}
	}
}

