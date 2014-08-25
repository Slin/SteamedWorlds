//
//  SWStaticEntity.h
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#ifndef __SWCrashingShip_H__
#define __SWCrashingShip_H__

#include <Rayne/Rayne.h>
#include <RBRigidBody.h>

namespace SW
{
	class CrashingShip : public RN::Entity
	{
	public:
		CrashingShip();
		CrashingShip(RN::Deserializer *deserializer);
		
		void Initialize();
		void Serialize(RN::Serializer *serializer) override;
		
		void StartAnimation();
		void Update(float delta) override;
		
	private:
		float _startCounter;
		bool _isStarted;
		RNDeclareMeta(CrashingShip)
	};
}

#endif /* __SWCrashingShip_H__ */
