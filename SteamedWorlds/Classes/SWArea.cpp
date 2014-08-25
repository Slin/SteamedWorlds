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
	
	Area::Area(float radius, AREA_TYPE type)
	: _type(type), _radius(radius)
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
		if(GetWorldPosition().GetDistance(Player::GetSharedInstance()->GetWorldPosition()) < _radius)
		{
			Player::GetSharedInstance()->EnterArea(_type);
		}
	}
}
