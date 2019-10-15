# Miner Mike
## A game created by Sidney Inouye and Benjamin Estela

## Game Description 
The game was created in a game engine that was created for CS381 Game Engine Architecture at the University of Nevada, Reno. The game engine was developed with OGRE and is written in C++.  
This game features Mike, a snowman who is looking for some gold. Navigate the snowman through the world and mine for precious metals, while also avoiding the evil penguins, which end your game. There are three levels to play through, with the last level having a randomly generated final treasure.  

## Fair Use
This game features music from [Ketsa Music](https://ketsa.uk/) with the Creative Commons License. This program was created for a class and is to demonstrate knowledge of putting background music into games.

## Dependencies
### Libraries
Libraries that are required to run this are OgreOverlay, audio, openal, OgreMain, OIS, and boost_system. Make sure these are stored in `/usr/local/lib/` and `/usr/lib/x86_64-linux-gnu`, as that is where Eclipse is set to look for these libraries.

### Includes
Other include paths are required to match are as follows:  
`/usr/include/AL`  
`/usr/include/audio`  
`/usr/local/include/OGRE/`  
`/usr/local/include/OGRE/Overlay/`  
`/usr/include/ois/`  

## How to Run
This game was developed on and only runs on Eclipse. As Eclipse is quite picky, make sure that the game is imported on as a C++ project so it is able to build and run. This game might not be able to run outside of the ECC though due to the Eclipse dependencies and pickiness.