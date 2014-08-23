//
//  SWWorld.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWWorld.h"

namespace SW
{
	World::World() :
		RN::World("GenericSceneManager")
	{}
	
	World::~World()
	{}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB888, RN::Camera::Flags::Defaults);
		
		RN::Model *islandStartModel = RN::Model::WithFile("Models/insel.sgm");
		/*RN::Entity *islandStart = */new RN::Entity(islandStartModel);
	}

	void World::Update(float delta)
	{
		// Do something...
	}
}
