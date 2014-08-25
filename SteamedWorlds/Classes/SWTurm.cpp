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
		GetSkeleton()->SetAnimation("Armature.002Action");
	}
	
	void Turm::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
	}
	
	void Turm::Update(float delta)
	{
		GetSkeleton()->Update(delta*24.0f, false);
	}
}
