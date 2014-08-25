//
//  SWStaticEntity.h
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#ifndef __SWTURM_H__
#define __SWTURM_H__

#include <Rayne/Rayne.h>
#include <RBRigidBody.h>

namespace SW
{
	class Turm : public RN::Entity
	{
	public:
		Turm();
		Turm(RN::Deserializer *deserializer);
		
		void Initialize();
		void Serialize(RN::Serializer *serializer) override;
		
		void StartAnimation();
		void Update(float delta) override;
		
	private:
		bool _isStarted;
		RNDeclareMeta(Turm)
	};
}

#endif /* __SWTURM_H__ */
