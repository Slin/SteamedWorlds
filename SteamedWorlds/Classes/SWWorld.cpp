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
#if RN_PLATFORM_MAC_OS
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
#endif
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
#ifndef RN_PLATFORM_MAC_OS
			sun->ActivateShadows(RN::ShadowParameter(static_cast<RN::Camera*>(_camera), 2048));
#endif
			sun->SetRotation(RN::Vector3(60.0f, -60.0f, 0.0f));
			
			//Load environment
			StaticEntity *islandStart = new StaticEntity("Models/insel.sgm");
			RN::Material * islandMaterial = islandStart->GetModel()->GetMaterialAtIndex(0, 0);
			islandMaterial->RemoveTextures();
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/bodeeen.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/booodeeen_rgb.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/grass.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/forestground.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/rock.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/sand.png"));
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/dirt3.png"));
			islandMaterial->SetShader(RN::Shader::WithFile("Shaders/Island"));
			
			islandMaterial = islandStart->GetModel()->GetMaterialAtIndex(0, 1);
			islandMaterial->RemoveTextures();
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/stone1.png"));
			islandMaterial->SetShader(RN::Shader::WithFile("Shaders/DiffuseAO"));
			islandMaterial->Define("TILE_FACTOR", 30.0f);
			
			StaticEntity *stein = new StaticEntity("Models/stein.sgm");
			stein->GetModel()->GetMaterialAtIndex(0, 0)->SetShader(RN::Shader::WithFile("Shaders/DiffuseAO"));
			stein->GetModel()->GetMaterialAtIndex(0, 0)->Define("TILE_FACTOR", 10.0f);
			
			StaticEntity *farn = new StaticEntity("Models/farn.sgm", false);
			farn->GetModel()->GetMaterialAtIndex(0, 0)->SetDiscard(true);
			farn->GetModel()->GetMaterialAtIndex(0, 1)->SetDiscard(true);
			
			new StaticEntity("Models/haus1.sgm");
			StaticEntity *tree = new StaticEntity("Models/trees.sgm", false);
			tree->GetModel()->GetMaterialAtIndex(0, 1)->SetDiscard(true);
			tree->GetModel()->GetMaterialAtIndex(0, 1)->SetDiscardThreshold(0.9f);
			tree->GetModel()->GetMaterialAtIndex(0, 1)->SetCullMode(RN::Material::CullMode::None);
			tree->GetModel()->GetMaterialAtIndex(0, 3)->SetDiscard(true);
			tree->GetModel()->GetMaterialAtIndex(0, 3)->SetDiscardThreshold(0.9f);
			tree->GetModel()->GetMaterialAtIndex(0, 3)->SetCullMode(RN::Material::CullMode::None);
			tree->GetModel()->GetMaterialAtIndex(0, 5)->SetDiscard(true);
			tree->GetModel()->GetMaterialAtIndex(0, 5)->SetDiscardThreshold(0.9f);
			tree->GetModel()->GetMaterialAtIndex(0, 5)->SetCullMode(RN::Material::CullMode::None);
			tree->GetModel()->GetMaterialAtIndex(0, 7)->SetDiscard(true);
			tree->GetModel()->GetMaterialAtIndex(0, 7)->SetDiscardThreshold(0.9f);
			tree->GetModel()->GetMaterialAtIndex(0, 7)->SetCullMode(RN::Material::CullMode::None);
			tree->GetModel()->GetMaterialAtIndex(0, 9)->SetDiscard(true);
			tree->GetModel()->GetMaterialAtIndex(0, 9)->SetDiscardThreshold(0.9f);
			tree->GetModel()->GetMaterialAtIndex(0, 9)->SetCullMode(RN::Material::CullMode::None);
			tree->GetModel()->GetMaterialAtIndex(0, 11)->SetDiscard(true);
			tree->GetModel()->GetMaterialAtIndex(0, 11)->SetDiscardThreshold(0.9f);
			tree->GetModel()->GetMaterialAtIndex(0, 11)->SetCullMode(RN::Material::CullMode::None);

			StaticEntity *jetty = new StaticEntity("Models/jetty.sgm");
			jetty->SetPosition(RN::Vector3(-6.815f, -9.343f, 55.183f));
			new StaticEntity("Models/home.sgm");
			
			Vehicle *ship = new Vehicle("Models/airboat.sgm", "Models/airboat_wheel.sgm");
			ship->SetWorldPosition(ship->GetWorldPosition()+RN::Vector3(0.0f, ship->GetBoundingBox().maxExtend.y, 8.0f));
			ship->GetModel()->GetMaterialAtIndex(0, 2)->Define("RN_SPECULARITY");
			ship->GetModel()->GetMaterialAtIndex(0, 2)->SetSpecularColor(RN::Color(0.1f, 0.08f, 0.05f, 250.0f));
			ship->GetModel()->GetMaterialAtIndex(0, 3)->Define("RN_SPECULARITY");
			ship->GetModel()->GetMaterialAtIndex(0, 3)->SetSpecularColor(RN::Color(0.1f, 0.08f, 0.05f, 250.0f));
			
			_turm = new Turm();
			_turm->GetModel()->GetMaterialAtIndex(0, 0)->SetCullMode(RN::Material::CullMode::None);
			
			_crashingShip = new CrashingShip();
			
			StaticEntity *turmplatform = new StaticEntity("Models/geruest_platform.sgm");
			turmplatform->GetModel()->GetMaterialAtIndex(0, 0)->SetCullMode(RN::Material::CullMode::None);
			
			StaticEntity *kristall = new StaticEntity("Models/diamond.sgm");
			kristall->GetModel()->GetMaterialAtIndex(0, 0)->SetCullMode(RN::Material::CullMode::None);
			
			StaticEntity *zaun = new StaticEntity("Models/zaunacker.sgm");
			zaun->GetModel()->GetMaterialAtIndex(0, 0)->SetCullMode(RN::Material::CullMode::None);
		}
		
		StartIntro();
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
	
	void World::StartIntro()
	{
		_crashingShip->StartAnimation();
		_turm->StartAnimation();
	}
}
