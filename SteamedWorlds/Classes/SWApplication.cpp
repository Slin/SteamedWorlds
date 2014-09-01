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

		SW::World *world = new World();
		
		RO::HMD *hmd = RO::System::GetSharedInstance()->GetHMD(0);
		if(hmd)
		{
			hmd->SetAsDisplay(false);
		}
		
		world->SetHMD(hmd);
		
/*		RN::Window *window = RN::Window::GetSharedInstance();
		RN::WindowConfiguration *configuration = new RN::WindowConfiguration(window->GetMainScreen()->GetWidth(), window->GetMainScreen()->GetHeight());
		window->ActivateConfiguration(configuration, RN::Window::Mask::Fullscreen);
		RN::Window::GetSharedInstance()->HideCursor();*/

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
