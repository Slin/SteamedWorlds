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
		RN::Model *model = RN::Model::WithFile("Models/airboat_ani.sgm");
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
		_isStarted = false;
		_startCounter = 0.0f;
		SetRenderGroup(5);
		GetSkeleton()->SetAnimation("idle");
	}
	
	void CrashingShip::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
	}
	
	void CrashingShip::StartAnimation()
	{
		_isStarted = true;
	}
	
	void CrashingShip::Update(float delta)
	{
		if(_isStarted)
		{
			_startCounter += delta;
			if(_startCounter > 1.9f)
			{
				SetRenderGroup(0);
				GetSkeleton()->Update(delta*24.0f, false);
			}
		}
	}
}
