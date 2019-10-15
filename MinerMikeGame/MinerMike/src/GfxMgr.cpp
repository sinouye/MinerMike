/*
 * GfxMgr.cpp
 *
 *  Created on: Mar 4, 2018
 *      Author: sushil
 */


#include <iostream>

#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreException.h>
#include <OgrePlane.h>
#include <OgreMeshManager.h>

#include <Engine.h>
#include <GfxMgr.h>

GfxMgr::GfxMgr(Engine *engine): Mgr(engine) {

	mRoot = 0;
	mResourcesCfg = Ogre::StringUtil::BLANK;
	mPluginsCfg   = Ogre::StringUtil::BLANK;
	mWindow = 0;
	mSceneMgr = 0;
	mCamera = 0;
	//oceanSurface(Ogre::Vector3::UNIT_Y, 0);

#ifdef _DEBUG
  mResourcesCfg = "resources_d.cfg";
  mPluginsCfg = "plugins_d.cfg";
#else
  mResourcesCfg = "resources.cfg";
  mPluginsCfg = "plugins.cfg";
#endif

  mRoot = new Ogre::Root(mPluginsCfg);

  Ogre::ConfigFile cf;
  cf.load(mResourcesCfg);

  Ogre::String name, locType;
  Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();

  while (secIt.hasMoreElements())
  {
    Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator it;

    for (it = settings->begin(); it != settings->end(); ++it)
    {
      locType = it->first;
      name = it->second;

      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
    }
  }

  if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
  	  std::cerr << "Could not find Config File and could not show Config Dialog" << std::endl;

    mWindow = mRoot->initialise(true, "Miner Mike");

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    mCamera = mSceneMgr->createCamera("MainCam");
    mCamera->setPosition(0, 0, 80);
    mCamera->lookAt(0, 0, -300);
    mCamera->setNearClipDistance(5);


    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    mCamera->setAspectRatio(
      Ogre::Real(vp->getActualWidth()) /
      Ogre::Real(vp->getActualHeight()));

    //-----------------------------------------------------------------------
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
}

GfxMgr::~GfxMgr() {

	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
//	delete rect;
	delete mRoot;
}

void GfxMgr::Init(){
  //mRoot->addFrameListener(this);
  //mRoot->startRendering();
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	createSplash();
}


void GfxMgr::MakeSky(){
	engine->gfxMgr->mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");
}

void GfxMgr::MakeGround(){

	//Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	//Ogre::Plane oceanSurface(Ogre::Vector3::UNIT_Y, 0);
	//oceanSurface = plane;
	oceanSurface.normal = Ogre::Vector3::UNIT_Y;
	oceanSurface.d = 0;
	Ogre::MeshManager::getSingleton().createPlane(
	    "ocean",
	    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	    //plane,
		oceanSurface,
	    10000, 10000, 20, 20,
	    true,
	    1, 20, 20,
	    Ogre::Vector3::UNIT_Z);

	  Ogre::Entity* groundEntity = engine->gfxMgr->mSceneMgr->createEntity("ocean");
	  engine->gfxMgr->mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	  groundEntity->setCastShadows(false);
	  //groundEntity->setMaterialName("Ocean2_HLSL_GLSL");
	  //groundEntity->setMaterialName("OceanHLSL_GLSL");
	  groundEntity->setMaterialName("Examples/BeachStones");
	  //groundEntity->setMaterialName("NavyCg");
}


void GfxMgr::windowClosed(Ogre::RenderWindow *rw){
	mRoot->shutdown();
	engine->keepRunning = false;
}

bool GfxMgr::frameRenderingQueued(const Ogre::FrameEvent& fe){
	if(mWindow->isClosed()) return false;
	return true;
}


void GfxMgr::LoadLevel(){

}

void GfxMgr::Stop(){
	mRoot->shutdown();
}

void GfxMgr::Tick(float dt){
//call ogre root render one frame method
	mRoot->renderOneFrame(dt);
	Ogre::WindowEventUtilities::messagePump();
}

void GfxMgr::createSplash()
{
	Ogre::Light* splashLight = engine->gfxMgr->mSceneMgr->createLight("SplashLight");
    splashLight->setPosition(20.0, 40.0, 50.0);

	rect = new Ogre::Rectangle2D(true);
	rect->setCorners(-1.0, 1.0, 1.0, -1.0);
	rect->setMaterial("Examples/BeachStones");

	rect->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND);

	Ogre::AxisAlignedBox aabInf;
	aabInf.setInfinite();
	rect->setBoundingBox(aabInf);

	Ogre::SceneNode* splashNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Background");
	splashNode->attachObject(rect);
}

void GfxMgr::createSplash2()
{

}

void GfxMgr::destroySplash()
{
	delete rect;
}
