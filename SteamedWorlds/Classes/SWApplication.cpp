//
//  SWApplication.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWApplication.h"

namespace SW
{
	Application::Application()
	{}
	Application::~Application()
	{}
	
	
	void Application::Start()
	{
		SetTitle("Steamed Worlds");

		RN::World *world = new World();
		RN::WorldCoordinator::GetSharedInstance()->LoadWorld(world->Autorelease());
	}
	
	void Application::WillExit()
	{}
	
	void Application::KeyDown(RN::Event *event)
	{
		if(event->GetType() == RN::Event::Type::KeyDown)
		{
			char character = event->GetCharacter();
			
			if(character == '0')
			{
				RN::MessageCenter::GetSharedInstance()->PostMessage(RNCSTR("DPToggle"), nullptr, nullptr);
			}
		}
	}
}
