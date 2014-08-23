//
//  SWVehicle.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWVehicle.h"

namespace SW
{
	RNDefineMeta(Vehicle, RN::Entity)
	
	Vehicle::Vehicle(const std::string filename)
	{
		RN::Model *model = RN::Model::WithFile(filename);
		SetModel(model);
		
		RN::bullet::TriangleMeshShape *shape = new RN::bullet::TriangleMeshShape(model);
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 0.0f);
			AddAttachment(body);
	}
}
