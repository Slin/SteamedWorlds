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
	
#if RN_PLATFORM_MAC_OS
	,_hmd(nullptr)
#endif
	{
		_physicsWorld = new RN::bullet::PhysicsWorld();
		AddAttachment(_physicsWorld);
		
		_audioWorld = new RN::openal::AudioWorld();
		AddAttachment(_audioWorld);
	}
	
	World::~World()
	{}
	
#if RN_PLATFORM_MAC_OS
	void World::SetHMD(RO::HMD *hmd)
	{
		if(_hmd)
			return;
		
		_hmd = hmd;
	}
#endif
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		RN::World::LoadOnThread(thread, deserializer);
		
		//Sky
		RN::Model *sky = RN::Model::WithSkyCube();
		for(int i = 0; i < sky->GetMeshCount(0); i++)
		{
			RN::Material *material = sky->GetMaterialAtIndex(0, i);
			material->SetAmbientColor(RN::Color(6.0f, 6.0f, 6.0f, 1.0f));
			material->SetOverride(RN::Material::Override::Shader);
			material->Define("RN_ATMOSPHERE");
		}
		
		//Create camera with effects and stuff
		if(_hmd)
		{
			RO::Camera *tempCamera = new RO::Camera(RN::Texture::Format::RGB16F);
			tempCamera->SetHMD(_hmd);
			tempCamera->SetAmbientColor(RN::Color::WithHSV(0.0f, 0.0f, 1.5f));
			tempCamera->GetLeftCamera()->SetSky(sky);
			tempCamera->GetRightCamera()->SetSky(sky);
			
			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera->GetLeftCamera());
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetLeftCamera());
			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera->GetRightCamera());
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetRightCamera());
			
			_camera = tempCamera;
		}
		else
		{
			RN::Camera *tempCamera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F);
			tempCamera->SetBlitShader(RN::Shader::WithFile("shader/rn_DrawFramebufferTonemap"));
			tempCamera->SetAmbientColor(RN::Color::WithHSV(0.0f, 0.0f, 1.5f));
			tempCamera->SetSky(sky);
			
			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera);
			
			_camera = tempCamera;
		}
		_camera->SetFlags(_camera->GetFlags()|RN::SceneNode::Flags::NoSave);
		
		RN::Renderer::GetSharedInstance()->SetHDRExposure(1.0f);
		RN::Renderer::GetSharedInstance()->SetHDRWhitePoint(8.0f);
		
		// Background ambient
		RN::openal::AudioListener *audioListener = new RN::openal::AudioListener();
		_camera->AddAttachment(audioListener);
		
		RN::AudioResource *audio = RN::AudioResource::WithFile("/Audio/wind_in_trees.ogg");
		RN::openal::AudioSource *audioSource = _audioWorld->PlaySound(audio);
		audioSource->SetRepeat(true);
		audioSource->SetGain(0.5f);
		
		//Create player
		Player *player = new Player(_camera);
		player->SetPosition(RN::Vector3(0.0f, 4.0f, 0.0f));
		
		if(!deserializer)
		{
			//Sun
			RN::Light *sun = new RN::Light(RN::Light::Type::DirectionalLight);
			//sun->ActivateShadows(RN::ShadowParameter(_camera, 2048));
			sun->SetRotation(RN::Vector3(60.0f, -60.0f, 0.0f));
			
			//Load environment
			StaticEntity *islandStart = new StaticEntity("Models/insel.sgm");
			RN::Material * islandMaterial = islandStart->GetModel()->GetMaterialAtIndex(0, 0);
			islandMaterial->RemoveTextures();
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/bodeeen.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/insel_rgba.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/grass.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/dirt.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/sand.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/stone1.png"));
			islandMaterial->SetShader(RN::Shader::WithFile("Shaders/Island"));
			
			StaticEntity *haus = new StaticEntity("Models/wohnhaus.sgm");
			haus->SetWorldPosition(RN::Vector3(20.0f, 1.0f, 0.0f));
			
			Vehicle *ship = new Vehicle("Models/airboat.sgm");
			ship->SetWorldPosition(ship->GetWorldPosition()+RN::Vector3(0.0f, ship->GetBoundingBox().maxExtend.y+5.0f, 0.0f));
			ship->GetModel()->GetMaterialAtIndex(0, 1)->Define("RN_SPECULARITY");
			ship->GetModel()->GetMaterialAtIndex(0, 1)->SetSpecularColor(RN::Color(0.1f, 0.08f, 0.05f, 250.0f));
			ship->GetModel()->GetMaterialAtIndex(0, 2)->Define("RN_SPECULARITY");
			ship->GetModel()->GetMaterialAtIndex(0, 2)->SetSpecularColor(RN::Color(0.1f, 0.08f, 0.05f, 250.0f));
		}
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
