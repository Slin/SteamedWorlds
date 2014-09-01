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
#include <ROCamera.h>

#include "SWFullscreenEffects.h"
#include "SWStaticEntity.h"
#include "SWPlayer.h"
#include "SWVehicle.h"
#include "SWTurm.h"
#include "SWCrashingShip.h"

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
		
		void StartIntro();

		void SetHMD(RO::HMD *hmd);
		
		RN::openal::AudioWorld *GetAudioWorld() const { return _audioWorld; }
		
	private:
		RN::SceneNode *_camera;
		RN::bullet::PhysicsWorld *_physicsWorld;
		RN::openal::AudioWorld *_audioWorld;
		
		bool _hasPlayedIntro;
		
		CrashingShip *_crashingShip;
		Turm *_turm;
		
		RN::GenericParticleEmitter *_vulcanoEmitter;
		RO::HMD *_hmd;
		
		RNDeclareMeta(World)
	};
}

#endif /* __SWWORLD_H__ */
