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
#include "SWWorld.h"

#include <RBPhysicsWorld.h>

#if RN_PLATFORM_MAC_OS
#include <ROCamera.h>
#endif

namespace SW
{
	RNDefineMeta(Player, RN::Entity)
	RNDefineSingleton(Player)
	
	Player::Player(RN::SceneNode *camera) :
	_camera(camera), _isPassable(true), _controller(nullptr), _footstepSource(nullptr), _hasBottle(false), _hasSteeringwheel(false), _hasDust(false)
	{
		MakeShared();
	
		_random.Seed(0x1024);
		SetFlags(GetFlags()|RN::SceneNode::Flags::NoSave);
		
		SetPassable(false);
		AddChild(_camera);
		
		_camera->SetPosition(RN::Vector3(0.0f, 0.78f, 0.0f));
		
//		_skeleton = RN::Skeleton::WithFile("Models/cameraent.sga");
//		_skeleton->SetAnimation("cameraAction");
//		_skeleton->Retain();
	}
	
	Player::~Player()
	{
		ResignShared();
		RN::SafeRelease(_controller);
		RN::SafeRelease(_skeleton);
	}
	
	void Player::Update(float delta)
	{
		RN::Entity::Update(delta);
		
		/*
		if(_isAnimating > 0.0f)
		{
			_isAnimating -= delta;
			
			if(_isAnimating < 0.0f)
				SetPassable(false);
			
			return;
		}
		*/

		if(_controller)
		{
			RN::Input *input = RN::Input::GetSharedInstance();
			
			RN::Vector3 rotationX(input->GetMouseDelta().x, 0.0f, 0.0f);
			Rotate(rotationX);
			
#if RN_PLATFORM_MAC_OS
			if(!_camera->IsKindOfClass(RO::Camera::GetMetaClass()))
#endif
			{
				RN::Vector3 rotationY(0.0f, input->GetMouseDelta().y, 0.0f);
				rotationY += _camera->GetRotation().GetEulerAngle();
				rotationY.y = std::max(-80.0f, std::min(65.0f, rotationY.y));
				_camera->SetRotation(rotationY);
			}
			
			RN::Vector3 direction(input->IsKeyPressed('d')-input->IsKeyPressed('a'), 0.0f, input->IsKeyPressed('s')-input->IsKeyPressed('w'));
			
			direction = GetRotation().GetRotatedVector(direction);
			direction *= 0.05f+0.05f*(input->GetModifierKeys() & RN::KeyModifier::KeyShift);
			
			_controller->SetWalkDirection(direction);
			
			if(!_footstepSource && direction.GetLength() > 0.0f)
			{
				std::string filename("Audio/footstep_gravel_");
				filename += std::to_string(_random.GetRandomInt32Range(1, 2));
				filename += ".ogg";
				_footstepSource = GetWorld()->Downcast<SW::World>()->GetAudioWorld()->PlaySound(RN::AudioResource::WithFile(filename));
				_footstepSource->SetSelfdestruct(false);
				_footstepSource->SetPitch(_random.GetRandomFloatRange(0.7f, 1.0f)*(input->GetModifierKeys() & RN::KeyModifier::KeyShift)?2.0f:1.0f);
				_footstepSource->SetGain(_random.GetRandomFloatRange(0.8f, 1.0f));
			}
			else
			{
				if(_footstepSource && !_footstepSource->IsPlaying())
				{
					_footstepSource->RemoveFromWorld();
					_footstepSource = nullptr;
				}
			}

			if(input->IsKeyPressed(' ') && _controller->IsOnGround())
			{
				_controller->Jump();
			}
		}
	}
	
	void Player::SetPassable(bool passable)
	{
		if(_isPassable == passable)
		{
			return;
		}
		
		if(!passable && !_controller)
		{
			_controller = new RN::bullet::KinematicController(RN::bullet::CapsuleShape::WithRadius(0.25f, 1.22f), 0.4f, RN::bullet::CapsuleShape::WithRadius(0.22f, 1.22f));
			_controller->SetJumpSpeed(3.4f);
			
			AddAttachment(_controller);
		}
		else
		{
			if(_controller)
			{
				RemoveAttachment(_controller);
				RN::SafeRelease(_controller);
			}
		}
		
		_isPassable = passable;
	}
	
	void Player::StartAnimation()
	{
		_isAnimating = 15.0f;
		//SetPassable(true);
		SetPosition(RN::Vector3(-6.6f, -1.5f, 28.0f));
		SetRotation(RN::Vector3(102.0f, 0.0f, 0.0f));
		static_cast<World*>(GetWorld())->GetAudioWorld()->PlaySound(RN::AudioResource::WithFile("Audio/talking-intro.ogg"));
	}

	void Player::PickUpItem(ITEM_TYPE type) {
		switch (type) {
		case ITEM_TYPE::BOTTLE:
			_hasBottle = true;
			static_cast<World*>(GetWorld())->GetAudioWorld()->PlaySound(RN::AudioResource::WithFile("Audio/talking-willdo.ogg"));
			break;

		case ITEM_TYPE::STEERINGWHEEL:
			_hasSteeringwheel = true;
			// say: what a nice steering wheel

			break;

		default:
			break;
		}
	}

	void Player::EnterArea(AREA_TYPE area) {
		switch (area) {
		case AREA_TYPE::DESERT_DUST:
			if (!_hasBottle) {
				static_cast<World*>(GetWorld())->GetAudioWorld()->PlaySound(RN::AudioResource::WithFile("Audio/talking-missingbottle.ogg"));
			}
			else {
				_hasDust = true;
				static_cast<World*>(GetWorld())->GetAudioWorld()->PlaySound(RN::AudioResource::WithFile("Audio/talking-gotovulcano.ogg"));
			}
			break;
		case AREA_TYPE::VULCANO:
			if (!_hasDust) {
				;// say: I need the special cometal dust to enable the anti-gravity forces of the diamond
			}
			else {
				// victory
				static_cast<World*>(GetWorld())->GetAudioWorld()->PlaySound(RN::AudioResource::WithFile("Audio/talking-end.ogg"));
			}
			break;
		}
	}
}
