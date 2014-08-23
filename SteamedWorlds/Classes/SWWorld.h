//
//  SWWorld.h
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#ifndef __SWWORLD_H__
#define __SWWORLD_H__

#include <Rayne/Rayne.h>

namespace SW
{
	class World : public RN::World
	{
	public:
		World();
		~World() override;
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void Update(float delta) override;
		
	private:
		RN::Camera *_camera;
	};
}

#endif /* __SWWORLD_H__ */
