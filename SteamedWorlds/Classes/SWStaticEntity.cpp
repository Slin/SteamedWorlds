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
	{
		SetFlags(GetFlags()|RN::SceneNode::Flags::NoSave);
		
		RN::Model *model = RN::Model::WithFile(filename);
		SetModel(model);
		
		if(collider)
		{
			RN::bullet::TriangleMeshShape *shape = new RN::bullet::TriangleMeshShape(model);
			RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 0.0f);
			AddAttachment(body);
		}
	}
}
