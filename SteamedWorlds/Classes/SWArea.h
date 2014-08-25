//
//  SWArea.h
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#ifndef __SWAREA_H__
#define __SWAREA_H__

#include <Rayne/Rayne.h>

namespace SW
{
	enum AREA_TYPE { DESERT_DUST, VULCANO };

	class Area : public RN::SceneNode
	{
	public:
		Area(float radius, AREA_TYPE type);
		Area(RN::Deserializer *deserializer);
		
		void Initialize();
		void Serialize(RN::Serializer *serializer) override;

		void Update(float delta) override;
		
	private:
		AREA_TYPE _type;
		float _radius;
		
		RNDeclareMeta(Area)
	};
}

#endif /* __SWAREA_H__ */
