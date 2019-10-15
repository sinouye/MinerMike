/*
 * UnitAI.cpp
 *
 *  Created on: Apr 7, 2019
 *      Author: sinouye
 */
#include <UnitAI.h>
#include <Command.h>

UnitAI::UnitAI(Entity381*ent) : Aspect(ent)
{

}

UnitAI::~UnitAI()
{

}

void UnitAI::Tick(float dt)
{
	if(commands.front())
	{
		commands.front()->tick(dt);
		if(commands.front()->done())
		{
			commands.pop_front();
			if(commands.front())
			{
				commands.front()->init();
			}
		}
	}
}

void UnitAI::SetCommand(Command *c)
{
	if(commands.front()) {
		commands.pop_front();
	}
	commands.push_front(c);
	commands.front()->init();
}

void UnitAI::AddCommand(Command *c)
{
	bool onlyCommand = true;
	if(commands.front())
		onlyCommand = false;
	commands.push_back(c);
	if(onlyCommand)
		commands.back()->init();
}
