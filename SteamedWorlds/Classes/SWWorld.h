//
//  SWWorld.h
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#ifndef __SWWORLD_H__
#define __SWWORLD_H__

#include <Rayne/Rayne.h>
#include <RBPhysicsWorld.h>

#include "SWFullscreenEffects.h"
#include "SWStaticEntity.h"
#include "SWPlayer.h"

namespace SW
{
	class World : public RN::World
	{
	public:
		World();
		~World() override;
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void SaveOnThread(RN::Thread *thread, RN::Serializer *serializer) override;
		
		void Update(float delta) override;
		
	private:
		RN::Camera *_camera;
		RN::bullet::PhysicsWorld *_physicsWorld;
		
		RNDeclareMeta(World)
	};
}

#endif /* __SWWORLD_H__ */
