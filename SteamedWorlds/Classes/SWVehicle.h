//
//  SWVehicle.h
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#ifndef __SWVEHICLE_H__
#define __SWVEHICLE_H__

#include <Rayne/Rayne.h>
#include <RBRigidBody.h>

namespace SW
{
	class Vehicle : public RN::Entity
	{
	public:
		Vehicle(const std::string filename);
		
	private:
		
		RNDeclareMeta(Vehicle)
	};
}

#endif /* __SWVEHICLE_H__ */
