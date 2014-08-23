//
//  SWStaticEntity.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWStaticEntity.h"

namespace SW
{
	RNDefineMeta(StaticEntity, RN::Entity)
	
	StaticEntity::StaticEntity(const std::string filename, bool collider)
	: _collider(collider), _fileName(filename)
	{
		Initialize();
	}
	
	StaticEntity::StaticEntity(RN::Deserializer *deserializer)
	: RN::Entity(deserializer)
	{
		_fileName = deserializer->DecodeString();
		_collider = deserializer->DecodeBool();
		
		Initialize();
	}
	
	void StaticEntity::Initialize()
	{
		RN::Model *model = RN::Model::WithFile(_fileName);
		SetModel(model);
		
		if(_collider)
		{
			RN::bullet::TriangleMeshShape *shape = new RN::bullet::TriangleMeshShape(model);
			RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 0.0f);
			AddAttachment(body);
		}
	}
	
	void StaticEntity::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
		serializer->EncodeString(_fileName);
		serializer->EncodeBool(_collider);
	}
}
