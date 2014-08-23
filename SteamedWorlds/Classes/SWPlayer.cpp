//
//  SWPlayer.cpp
//  SteamedWorlds
//
//  Copyright 2014 by Überpixel. All rights reserved.
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "SWPlayer.h"
#include <RBPhysicsWorld.h>

namespace SW
{
	RNDefineMeta(Player, RN::Entity)
	RNDefineSingleton(Player)
	
	Player::Player(RN::SceneNode *camera) :
		_camera(camera)
	{
		MakeShared();
		
		_controller = new RN::bullet::KinematicController(RN::bullet::CapsuleShape::WithRadius(0.5f, 1.0f), 0.7f);
		_controller->SetJumpSpeed(3.4f);
		
		AddAttachment(_controller);
		AddChild(_camera);
		
		_camera->SetPosition(RN::Vector3(0.0f, 0.5f, 0.0f));
	}
	
	Player::~Player()
	{
		ResignShared();
		_controller->Release();
	}
	
	void Player::Update(float delta)
	{
		RN::Entity::Update(delta);
		
		RN::Input *input = RN::Input::GetSharedInstance();
		
		RN::Vector3 direction(input->IsKeyPressed('d')-input->IsKeyPressed('a'), 0.0f, input->IsKeyPressed('s')-input->IsKeyPressed('w'));
		
		RN::Vector3 rotationX(input->GetMouseDelta().x, 0.0f, 0.0f);
		
		Rotate(rotationX);
		
		direction = GetRotation().GetRotatedVector(direction);
		direction *= 0.1f+0.1f*(input->GetModifierKeys() & RN::KeyModifier::KeyShift);
		
		_controller->SetWalkDirection(direction);

		if(input->IsKeyPressed(' ') && _controller->IsOnGround())
		{
			_controller->Jump();
		}
	}
}
