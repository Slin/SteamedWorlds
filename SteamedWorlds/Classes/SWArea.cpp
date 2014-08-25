//
//  SWPickupEntity.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWArea.h"
#include "SWPlayer.h"

namespace SW
{
	RNDefineMeta(Area, RN::SceneNode)
	
	Area::Area(AREA_TYPE type)
	: _type(type)
	{
		Initialize();
	}
	
	Area::Area(RN::Deserializer *deserializer)
	: RN::SceneNode(deserializer)
	{
		
		Initialize();
	}
	
	void Area::Initialize()
	{
		
	}
	
	void Area::Serialize(RN::Serializer *serializer)
	{
		SceneNode::Serialize(serializer);
	}
	
	void Area::Update(float delta)
	{
		if(GetWorldPosition().GetDistance(Player::GetSharedInstance()->GetWorldPosition()) > 2.0f)
		{
			Player::GetSharedInstance()->EnterArea(_type);
		}
	}
}
