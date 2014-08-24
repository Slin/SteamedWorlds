//
//  SWVehicle.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWVehicle.h"
#include "SWPlayer.h"
#include <ROCamera.h>

namespace SW
{
	RNDefineMeta(Vehicle, RN::Entity)
	
	Vehicle::Vehicle(const std::string filename, const std::string wheelfile, RN::Vector3 bounds, RN::Vector3 wheelpos)
	{
		RN::Model *model = RN::Model::WithFile(filename);
		SetModel(model);
		
		_wheel = new RN::Entity(RN::Model::WithFile(wheelfile));
		AddChild(_wheel);
		_wheel->SetPosition(wheelpos);
		
		_bounds = bounds;
		
		Initialize();
	}
	
	Vehicle::Vehicle(RN::Deserializer *deserializer)
	: RN::Entity(deserializer)
	{
		_bounds = deserializer->DecodeVector3();
		Initialize();
	}
	
	void Vehicle::Initialize()
	{
		_isActive = false;
		AddDependency(Player::GetSharedInstance());
		
		RN::bullet::BoxShape *shape = new RN::bullet::BoxShape(_bounds);
//		RN::bullet::CylinderShape *shape = new RN::bullet::CylinderShape();
		RN::bullet::RigidBody *body = new RN::bullet::RigidBody(shape, 10.0f);
		AddAttachment(body);
		
		body->SetGravity(RN::Vector3(0.0f));
		body->SetDamping(0.6, 0.4);
		body->GetBulletRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
		body->GetBulletCollisionObject()->setActivationState(DISABLE_DEACTIVATION);
		
		_body = body;
	}
	
	void Vehicle::Serialize(RN::Serializer *serializer)
	{
		Entity::Serialize(serializer);
		
		serializer->EncodeVector3(_bounds);
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
					if(player->GetWorldPosition().GetDistance(GetWorldPosition()) < GetBoundingBox().maxExtend.GetLength()*1.2f)
					{
						player->SetPassable(true);
						
						_camera = player->GetCamera();
						_oldCameraPosition = _camera->GetPosition();
						player->RemoveChild(_camera);
						AddChild(_camera);
						_camera->SetPosition(_oldCameraPosition + RN::Vector3(-1.5f, -3.5f, 0.0f));
						
						RemoveDependency(player);
						AddChild(player);
						player->SetPosition(RN::Vector3(-1.5f, -3.5f, 0.0f));
						_isActive = true;
					}
				}
				else
				{
					RemoveChild(player);
					AddDependency(player);
					player->SetWorldPosition(GetWorldPosition()+GetBoundingBox().maxExtend*1.2f);
					
					RemoveChild(_camera);
					player->AddChild(_camera);
					_camera->SetPosition(_oldCameraPosition);
					_camera->SetRotation(RN::Vector3(0.0f, 0.0f, 0.0f));
					
					player->SetPassable(false);
					_isActive = false;
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
			
			RN::Vector3 direction(input->IsKeyPressed('s')-input->IsKeyPressed('w'), input->IsKeyPressed('e')-input->IsKeyPressed('q'), 0.0f);
			direction = GetWorldRotation().GetRotatedVector(direction);
			_body->ApplyImpulse(direction*1.5f);
			
			float rotation = input->IsKeyPressed('a')-input->IsKeyPressed('d');
			_body->ApplyTorqueImpulse(RN::Vector3(0.0f, rotation*0.5f, 0.0f));
			
			if(rotation != 0.0f)
				_wheelRotation += rotation*delta*80.0f;
			else
				_wheelRotation -= _wheelRotation*delta*0.9f;
				
			_wheel->SetRotation(RN::Vector3(0.0f, _wheelRotation, 0.0f));
		}
	}
}
