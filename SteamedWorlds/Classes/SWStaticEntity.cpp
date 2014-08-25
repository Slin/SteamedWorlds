//
//  SWStaticEntity.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWStaticEntity.h"
#include "SWPlayer.h"

namespace SW
{
	RNDefineMeta(StaticEntity, RN::Entity)
	
	StaticEntity::StaticEntity(const std::string filename, bool collider)
	: _collider(collider)
	{
		RN::Model *model = RN::Model::WithFile(filename);
		SetModel(model);
		
		Initialize();
	}
	
	StaticEntity::StaticEntity(RN::Deserializer *deserializer)
	: RN::Entity(deserializer)
	{
		_collider = deserializer->DecodeBool();
		Initialize();
	}
	
	void StaticEntity::Initialize()
	{
		if(_collider)
		{
			RN::bullet::TriangleMeshShape *shape = new RN::bullet::TriangleMeshShape(GetModel());
			RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 0.0f);
			AddAttachment(body);
		}
	}
	
	void StaticEntity::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
		serializer->EncodeBool(_collider);
	}
	
	void StaticEntity::Update(float delta)
	{
		if(_collider)
			return;
		
		if(GetWorldPosition().GetDistance(Player::GetSharedInstance()->GetWorldPosition()) > 500.0f)
		{
			SetRenderGroup(5);
		}
		else
		{
			SetRenderGroup(0);
		}
	}
}
