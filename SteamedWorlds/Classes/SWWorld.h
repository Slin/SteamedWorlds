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
#include <RALAudioWorld.h>

#if RN_PLATFORM_MAC_OS
	#include <ROCamera.h>
#endif

#include "SWFullscreenEffects.h"
#include "SWStaticEntity.h"
#include "SWPlayer.h"
#include "SWVehicle.h"
#include "SWTurm.h"

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
		
#if RN_PLATFORM_MAC_OS
		void SetHMD(RO::HMD *hmd);
#endif
		
		RN::openal::AudioWorld *GetAudioWorld() const { return _audioWorld; }
		
	private:
		RN::SceneNode *_camera;
		RN::bullet::PhysicsWorld *_physicsWorld;
		RN::openal::AudioWorld *_audioWorld;
		
#if RN_PLATFORM_MAC_OS
		RO::HMD *_hmd;
#endif
		
		RNDeclareMeta(World)
	};
}

#endif /* __SWWORLD_H__ */
