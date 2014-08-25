//
//  SWPickupEntity.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWPickupEntity.h"
#include "SWPlayer.h"

namespace SW
{
	RNDefineMeta(PickupEntity, RN::Entity)
	
	PickupEntity::PickupEntity(const std::string filename, ITEM_TYPE type)
	: _type(type)
	{
		RN::Model *model = RN::Model::WithFile(filename);
		SetModel(model);
		
		Initialize();
	}
	
	PickupEntity::PickupEntity(RN::Deserializer *deserializer)
	: RN::Entity(deserializer)
	{
		
		Initialize();
	}
	
	void PickupEntity::Initialize()
	{
		
	}
	
	void PickupEntity::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
	}
	
	void PickupEntity::Update(float delta)
	{
		if(GetWorldPosition().GetDistance(Player::GetSharedInstance()->GetWorldPosition()) < 1.0f)
		{
			// Pick up
			RemoveFromWorld();
			Player::GetSharedInstance()->PickUpItem(_type);
		}
	}
}
