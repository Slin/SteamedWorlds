//
//  SWPickupEntity.h
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#ifndef __SWPICKUPENTITY_H__
#define __SWPICKUPENTITY_H__

#include <Rayne/Rayne.h>

namespace SW
{
	enum ITEM_TYPE { BOTTLE, STEERINGWHEEL };

	class PickupEntity : public RN::Entity
	{
	public:
		PickupEntity(const std::string filename, ITEM_TYPE type);
		PickupEntity(RN::Deserializer *deserializer);
		
		void Initialize();
		void Serialize(RN::Serializer *serializer) override;
		
		void Update(float delta) override;
		
	private:
		ITEM_TYPE _type;
		
		RNDeclareMeta(PickupEntity)
	};
}

#endif /* __SWPICKUPENTITY_H__ */
