//
//  SWVehicle.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWVehicle.h"
#include "SWPlayer.h"

namespace SW
{
	RNDefineMeta(Vehicle, RN::Entity)
	
	Vehicle::Vehicle(const std::string filename)
	{
		RN::Model *model = RN::Model::WithFile(filename);
		SetModel(model);
		
		Initialize();
	}
	
	Vehicle::Vehicle(RN::Deserializer *deserializer)
	: RN::Entity(deserializer)
	{
		Initialize();
	}
	
	void Vehicle::Initialize()
	{
		_isActive = false;
		
		RN::bullet::BoxShape *shape = new RN::bullet::BoxShape(GetBoundingBox().maxExtend);
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 10.0f);
		AddAttachment(body);
	}
	
	void Vehicle::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
	}
	
	void Vehicle::Update(float delta)
	{
		RN::Input *input = RN::Input::GetSharedInstance();
		Player *player = Player::GetSharedInstance();
		
		if(input->IsKeyPressed('f'))
		{
			if(!_hasToggled)
			{
				if(!_isActive)
				{
					if(player->GetWorldPosition().GetDistance(GetWorldPosition()) < GetBoundingBox().maxExtend.GetLength()*1.2f)
					{
						player->SetPassable(true);
						player->SetWorldPosition(GetWorldPosition());
						AddChild(player);
						_isActive = true;
					}
				}
				else
				{
					RemoveChild(player);
					player->SetWorldPosition(GetWorldPosition()+GetBoundingBox().maxExtend*1.2f);
					player->SetPassable(false);
					_isActive = false;
				}
				
				_hasToggled = true;
			}
		}
		else
		{
			_hasToggled = false;
		}
	}
}
