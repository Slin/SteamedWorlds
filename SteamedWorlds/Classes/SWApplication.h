//
//  SWApplication.h
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#ifndef __SWAPPLICATION_H__
#define __SWAPPLICATION_H__

#include <Rayne/Rayne.h>
#include "SWWorld.h"

namespace SW
{
	class Application : public RN::Application
	{
	public:
		Application();
		~Application() override;
		
		void Start() override;
		void WillExit() override;
		void KeyDown(RN::Event *event) override;
	};
}

#endif /* __SWAPPLICATION_H__ */
