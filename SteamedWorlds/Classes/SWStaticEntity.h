//
//  SWStaticEntity.h
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#ifndef __SWSTATICENTITY_H__
#define __SWSTATICENTITY_H__

#include <Rayne/Rayne.h>
#include <RBRigidBody.h>

namespace SW
{
	class StaticEntity : public RN::Entity
	{
	public:
		StaticEntity(const std::string filename, bool collider=true);
		StaticEntity(RN::Deserializer *deserializer);
		
		void Initialize();
		void Serialize(RN::Serializer *serializer) override;
		
		void Update(float delta) override;
		
	private:
		bool _collider;
		
		RNDeclareMeta(StaticEntity)
	};
}

#endif /* __SWSTATICENTITY_H__ */
