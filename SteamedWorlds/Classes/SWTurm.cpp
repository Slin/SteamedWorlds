//
//  SWStaticEntity.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWTurm.h"

namespace SW
{
	RNDefineMeta(Turm, RN::Entity)
	
	Turm::Turm()
	{
		RN::Model *model = RN::Model::WithFile("Models/geruest.sgm");
		SetModel(model);
		
		Initialize();
	}
	
	Turm::Turm(RN::Deserializer *deserializer)
	: RN::Entity(deserializer)
	{
		Initialize();
	}
	
	void Turm::Initialize()
	{
		_isStarted = false;
		GetSkeleton()->SetAnimation("kill");
	}
	
	void Turm::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
	}
	
	void Turm::StartAnimation()
	{
		_isStarted = true;
	}
	
	void Turm::Update(float delta)
	{
		if(_isStarted)
			GetSkeleton()->Update(delta*24.0f, false);
	}
}
