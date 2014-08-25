//
//  SWStaticEntity.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWCrashingShip.h"

namespace SW
{
	RNDefineMeta(CrashingShip, RN::Entity)
	
	CrashingShip::CrashingShip()
	{
		RN::Model *model = RN::Model::WithFile("Models/airship.sgm");
		SetModel(model);
		
		Initialize();
	}
	
	CrashingShip::CrashingShip(RN::Deserializer *deserializer)
	: RN::Entity(deserializer)
	{
		Initialize();
	}
	
	void CrashingShip::Initialize()
	{
		GetSkeleton()->SetAnimation("Armature.002Action");
	}
	
	void CrashingShip::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
	}
	
	void CrashingShip::Update(float delta)
	{
		GetSkeleton()->Update(delta*24.0f, false);
	}
}
