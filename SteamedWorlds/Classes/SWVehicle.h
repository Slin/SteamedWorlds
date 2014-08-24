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
		Vehicle(RN::Deserializer *deserializer);
		
		void Initialize();
		void Serialize(RN::Serializer *serializer) override;
		
		void Update(float delta) override;
		
	private:
		bool _isActive;
		bool _hasToggled;
		
		class RN::bullet::RigidBody *_body;
		RN::SceneNode *_camera;
		RN::Vector3 _oldCameraPosition;
		
		RNDeclareMeta(Vehicle)
	};
}

#endif /* __SWVEHICLE_H__ */
