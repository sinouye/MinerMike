#ifndef __Entity381_h_
#define __Entity381_h_

#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <UnitAI.h>
#include <Aspect.h>
#include <Types381.h>

#include <Engine.h>

class Entity381
{
public:
  Entity381(Engine *engine, Ogre::Vector3 pos, int identity);
  virtual ~Entity381();

  Engine *engine;

  //static data
  int identity;
  std::string name;
  std::string meshfilename;
  Ogre::SceneNode* sceneNode;
  Ogre::Entity*    ogreEntity;
  float acceleration, turnRate;
  float minSpeed, maxSpeed;

  //dynamic data
  Ogre::Vector3 position;
  Ogre::Vector3 originalPos;
  Ogre::Vector3 velocity;
  bool isSelected;
  bool isDug;
  float desiredHeading, desiredSpeed;
  float heading, speed;

  int cmdType;

  long long money;
  long long finalMoney;

  bool didSelectSoundPlay;
  std::string soundFile;
  bool playSound;
  unsigned int auioId;

  unsigned int entityId;

  EntityTypes entityType;

  UnitAI * ai;

  std::vector<Aspect* > aspects;

  void Init();

  void Tick(float dt);

  static unsigned int nextId;

  void moneyBronze();
  void moneySilver();
  void moneyGold();
  void moneyTreasure();

protected:


};

class Miner: public Entity381 {
public:
	Miner(Engine *engine, Ogre::Vector3 pos, int identity);
	virtual ~Miner();
};

class Diggable: public Entity381 {
public:
	Diggable(Engine *engine, Ogre::Vector3 pos, int identity);
	virtual ~Diggable();
};

class Solid: public Entity381 {
public:
	Solid(Engine *engine, Ogre::Vector3 pos, int identity);
	virtual ~Solid();
};
class Snake : public Entity381 {
public:
	Snake(Engine *engine, Ogre::Vector3 pos, int identity);
	virtual ~Snake();
};

#endif // #ifndef __Entity381_h_
