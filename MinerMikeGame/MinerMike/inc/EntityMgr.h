#ifndef __EntityMgr_h_
#define __EntityMgr_h_

#include <vector>
#include <Mgr.h>
#include <Entity381.h>
#include <FlyingEntity381.h>
#include <Types381.h>
#include <Command.h>
#include <UnitAI.h>
#include <GfxMgr.h>
#include <UiMgr.h>
#include <InputMgr.h>
#include <cstdlib>
#include <ctime>

class EntityMgr: public Mgr {
public:
  EntityMgr(Engine *engine);
  virtual ~EntityMgr();

  //Engine *engine;

  std::vector<Entity381*> entities;
  Entity381* selectedEntity;
  Entity381* interceptEntity;
  FlyingEntity381* selectedFlyingEntity;
  int selectedEntityIndex;
  int interceptEntityIndex;

  Ogre::AnimationState* mAnimationState;

  //Ogre::SceneManager *sceneMgr;

  void CreateEntityOfTypeAtPosition(EntityTypes type, Ogre::Vector3 pos);
  void SelectNextEntity();
  void SetSelectedFlyingEntity();
  void SelectInterceptEntity(int i);
  void Select(int i);

  void DigBlock(int i);

  void Tick(float dt);

protected:

private:
  void CreateEntity(std::string meshfilename, Ogre::Vector3 pos);
  void CreateMiner(Ogre::Vector3 pos);
  void CreateDiggable(Ogre::Vector3 pos);
  void CreateSolid(Ogre::Vector3 pos);
  void CreateSnake(Ogre::Vector3 pos);
  int count;
  bool snakeCmd;
  int treasureBlock;
};

#endif // #ifndef __EntityMgr_h_
