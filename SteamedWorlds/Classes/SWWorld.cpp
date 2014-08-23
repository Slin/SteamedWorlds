//
//  SWWorld.cpp
//  SteamedWorlds
//
//  Created by Nils Daumann on 23.08.14.
//  Copyright (c) 2014 GoodyCow. All rights reserved.
//

#include "SWWorld.h"

namespace SW
{
	RNDefineMeta(World, RN::World)
	
	World::World() :
		RN::World("GenericSceneManager")
	{
		_physicsWorld = new RN::bullet::PhysicsWorld();
		AddAttachment(_physicsWorld);
	}
	
	World::~World()
	{}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		RN::World::LoadOnThread(thread, deserializer);
		
		//Create camera with effects and stuff
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, RN::Camera::Flags::Defaults);
		_camera->SetBlitShader(RN::Shader::WithFile("shader/rn_DrawFramebufferTonemap"));
		_camera->SceneNode::SetFlags(RN::SceneNode::Flags::NoSave);
		
		RN::Model *sky = RN::Model::WithSkyCube();
		for(int i = 0; i < sky->GetMeshCount(0); i++)
		{
			RN::Material *material = sky->GetMaterialAtIndex(0, i);
			material->SetAmbientColor(RN::Color(6.0f, 6.0f, 6.0f, 1.0f));
			material->SetOverride(RN::Material::Override::Shader);
			material->Define("RN_ATMOSPHERE");
		}
		_camera->SetSky(sky);
		
		RN::Renderer::GetSharedInstance()->SetHDRExposure(1.0f);
		RN::Renderer::GetSharedInstance()->SetHDRWhitePoint(2.5f);
		
		FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(_camera);
		
		//Sun
		RN::Light *sun = new RN::Light(RN::Light::Type::DirectionalLight);
		sun->ActivateShadows(RN::ShadowParameter(_camera, 256));
		sun->SetRotation(RN::Vector3(60.0f, -60.0f, 0.0f));
		
		//Load environment
		StaticEntity *islandStart = new StaticEntity("Models/inselj.sgm");
		StaticEntity *blubb = new StaticEntity("Models/schiff.sgm");
		
		//Create player
		new Player(_camera);
	}
	
	void World::SaveOnThread(RN::Thread *thread, RN::Serializer *serializer)
	{
		RN::World::SaveOnThread(thread, serializer);
		
		/*serializer->EncodeObject(_ground);
		
		serializer->EncodeVector3(_camera->GetWorldPosition());
		serializer->EncodeQuarternion(_camera->GetWorldRotation());*/
	}

	void World::Update(float delta)
	{
		// Do something...
	}
}
