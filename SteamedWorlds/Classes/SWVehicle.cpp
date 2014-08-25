//
//  SWVehicle.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWVehicle.h"
#include "SWPlayer.h"

#if RN_PLATFORM_MAC_OS
#include <ROCamera.h>
#endif

namespace SW
{
	RNDefineMeta(Vehicle, RN::Entity)
	
	Vehicle::Vehicle(const std::string filename, const std::string wheelfile, RN::Vector3 wheelpos)
	{
		RN::Model *model = RN::Model::WithFile(filename);
		SetModel(model);
		
		_wheel = new RN::Entity(RN::Model::WithFile(wheelfile));
		AddChild(_wheel);
		_wheel->SetPosition(wheelpos);
		
		_engineLeft = new RN::openal::AudioSource(RN::AudioResource::WithFile("Audio/engine.ogg"));
		_engineLeft->SetRepeat(true);
		AddChild(_engineLeft);
		_engineLeft->SetPosition(RN::Vector3(-0.02f, -4.27f, 1.64f));
		
		_engineRight = new RN::openal::AudioSource(RN::AudioResource::WithFile("Audio/engine.ogg"));
		_engineRight->SetRepeat(true);
		AddChild(_engineRight);
		_engineRight->SetPosition(RN::Vector3(-0.02f, -4.27f, -1.64f));
		
		Initialize();
	}
	
	Vehicle::Vehicle(RN::Deserializer *deserializer)
	: RN::Entity(deserializer)
	{
		Initialize();
	}
	
	void Vehicle::Initialize()
	{
		_isActive = false;
		_isRepaired = false;
		AddDependency(Player::GetSharedInstance());
		
		RN::bullet::CompoundShape *shape = new RN::bullet::CompoundShape();
		shape->Autorelease();
		
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(1.8f, 0.555f, 1.36f)), RN::Vector3(-0.76f, -5.03f, 0.0f), RN::Vector3(0.0f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(1.15f, 0.555f, 1.36f)), RN::Vector3(2.11f, -4.53f, 0.0f), RN::Vector3(0.0f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(0.595f, 0.853f, 1.0f)), RN::Vector3(-2.81f, -3.76f, 0.0f), RN::Vector3(0.0f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(1.205f, 0.395f, 0.4505f)), RN::Vector3(-4.6f, -3.59f, 0.0f), RN::Vector3(0.0f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(0.77f, 0.555f, 0.452)), RN::Vector3(-0.02f, -4.27f, -1.64f), RN::Vector3(0.0f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(0.77f, 0.555f, 0.452)), RN::Vector3(-0.02f, -4.28f, 1.62f), RN::Vector3(0.0f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(0.11f, 0.555f, 1.36)), RN::Vector3(-0.12f, -4.06f, 1.24f), RN::Vector3(90.0f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(0.11f, 0.555f, 1.36)), RN::Vector3(-0.12f, -4.06f, -1.23f), RN::Vector3(90.0f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(0.11f, 0.555f, 1.36)), RN::Vector3(2.3f, -4.06f, -0.5f), RN::Vector3(56.8f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(0.11f, 0.555f, 1.36)), RN::Vector3(2.3f, -4.06f, 0.55f), RN::Vector3(121.5f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(0.11f, 0.555f, 0.495)), RN::Vector3(-1.88f, -4.06f, 1.05f), RN::Vector3(63.4f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(0.11f, 0.555f, 0.495)), RN::Vector3(-1.88f, -4.06f, -1.03f), RN::Vector3(114.7f, 0.0f, 0.0f));
		shape->AddChild(RN::bullet::BoxShape::WithHalfExtents(RN::Vector3(7.235f, 3.125f, 3.125f)), RN::Vector3(-0.08f, 2.39f, 0.06f), RN::Vector3(0.0f, 0.0f, 0.0f));
		
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 10.0f);
		AddAttachment(body);
		
		body->SetGravity(RN::Vector3(0.0f));
		body->SetDamping(0.6, 0.4);
		body->GetBulletRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
		
		_body = body;
	}
	
	void Vehicle::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
	}
	
	void Vehicle::Update(float delta)
	{
		
		RN::Input *input = RN::Input::GetSharedInstance();
		Player *player = Player::GetSharedInstance();
		
		if(input->IsKeyPressed('f'))
		{
			if(!_hasToggled)
			{
				if(!_isActive)
				{
					if(player->GetWorldPosition().GetDistance(GetWorldPosition()) < GetBoundingBox().maxExtend.GetLength()*0.8f)
					{
						if (player->HasSteeringWheel()) {
							_isRepaired = true;
							// say: that
						}

						if (!_isRepaired) {
							// ich brauch steering wheel
							//static_cast<World*>(GetWorld())->GetAudioWorld()->PlaySound(RN::AudioResource::WithFile(""));

						} else {
							player->SetPassable(true);
							_body->GetBulletCollisionObject()->forceActivationState(DISABLE_DEACTIVATION);

							_camera = player->GetCamera();
							_oldCameraPosition = _camera->GetPosition();
							player->RemoveChild(_camera);
							AddChild(_camera);
							_camera->SetPosition(_oldCameraPosition + RN::Vector3(-1.5f, -3.5f, 0.0f));

							RemoveDependency(player);
							AddChild(player);
							player->SetPosition(RN::Vector3(-1.5f, -3.5f, 0.0f));
							_isActive = true;

							_engineLeft->SetRepeat(true);
							_engineLeft->Play();

							_engineRight->SetRepeat(true);
							_engineRight->Play();
						}
					}
				}
				else
				{
					RemoveChild(player);
					AddDependency(player);
					_body->GetBulletCollisionObject()->forceActivationState(DISABLE_SIMULATION);
					
					player->SetRotation(RN::Vector3(_camera->GetEulerAngle().x, 0.0f, 0.0f));
					RemoveChild(_camera);
					player->AddChild(_camera);
					_camera->SetPosition(_oldCameraPosition);
					_camera->SetRotation(RN::Vector3(0.0f, 0.0f, 0.0f));
					
					player->SetPassable(false);
					_isActive = false;
					
					_engineLeft->SetRepeat(false);
					_engineRight->SetRepeat(false);
				}
				
				_hasToggled = true;
			}
		}
		else
		{
			_hasToggled = false;
		}
		
		if(_isActive)
		{
#if RN_PLATFORM_MAC_OS
			if(!_camera->IsKindOfClass(RO::Camera::GetMetaClass()))
#endif
			{
				RN::Vector3 euler = _camera->GetEulerAngle();
				euler.x += input->GetMouseDelta().x;
				euler.y += input->GetMouseDelta().y;
				euler.y = std::max(-80.0f, std::min(65.0f, euler.y));
				_camera->SetRotation(euler);
			}
			
			RN::Vector3 speed = _body->GetLinearVelocity();
			float gain = std::min(speed.GetLength()*0.1f+0.1f, 1.0f)*0.5f;
			float pitch = 0.2+std::min(speed.GetLength()*0.1f, 1.0f);
			_engineRight->SetGain(gain);
			_engineRight->SetPitch(pitch);
			_engineLeft->SetPitch(pitch);
			_engineLeft->SetGain(gain);
			
			RN::Vector3 direction(input->IsKeyPressed('s')-input->IsKeyPressed('w'), input->IsKeyPressed('e')-input->IsKeyPressed('q'), 0.0f);
			direction = GetWorldRotation().GetRotatedVector(direction);
			_body->ApplyImpulse(direction*(21.5f+(input->GetModifierKeys()&RN::KeyModifier::KeyShift)));
			
			float rotation = input->IsKeyPressed('a')-input->IsKeyPressed('d');
			_body->ApplyTorqueImpulse(RN::Vector3(0.0f, rotation*0.5f, 0.0f));
			
			if(rotation != 0.0f)
				_wheelRotation += rotation*delta*80.0f;
			else
				_wheelRotation -= _wheelRotation*delta*0.9f;
				
			_wheel->SetRotation(RN::Vector3(0.0f, _wheelRotation, 0.0f));

			smokeEmitter->SetLifeSpan(RN::Vector2(2.5f, 1.0f));
			smokeEmitter->SetSpawnRate(0.02f);
		}
	}
}
