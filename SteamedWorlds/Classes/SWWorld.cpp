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
		RN::World("GenericSceneManager"), _hasPlayedIntro(false), _hmd(nullptr)
	{
		_physicsWorld = new RN::bullet::PhysicsWorld();
		AddAttachment(_physicsWorld);
		
		_audioWorld = new RN::openal::AudioWorld();
		AddAttachment(_audioWorld);
	}
	
	World::~World()
	{}
	
	void World::SetHMD(RO::HMD *hmd)
	{
		if(_hmd)
			return;
		
		_hmd = hmd;
	}
	
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
		
		RN::openal::AudioListener *audioListener = new RN::openal::AudioListener();
		
		//Create camera with effects and stuff
		if(_hmd)
		{
			RO::Camera *tempCamera = new RO::Camera(RN::Texture::Format::RGB16F);
			tempCamera->SetHMD(_hmd);
			tempCamera->SetAmbientColor(RN::Color::WithHSV(0.0f, 0.0f, 1.5f));
			tempCamera->GetLeftCamera()->SetSky(sky);
			tempCamera->GetRightCamera()->SetSky(sky);
			
//			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera->GetLeftCamera());
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetLeftCamera());
//			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera->GetRightCamera());
			FullscreenEffects::GetSharedInstance()->CreateGammaPipeline(tempCamera->GetRightCamera());
			
			_camera = tempCamera;
			tempCamera->GetHead()->AddAttachment(audioListener);
		}
		else
		{
			RN::Camera *tempCamera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F);
			tempCamera->SetBlitShader(RN::Shader::WithFile("shader/rn_DrawFramebufferTonemap"));
			tempCamera->SetAmbientColor(RN::Color::WithHSV(0.0f, 0.0f, 1.5f));
			tempCamera->SetSky(sky);
			tempCamera->SetClipFar(5000.0f);
			tempCamera->SetFogFar(5000.0f);
			tempCamera->SetFogNear(100.0f);
			tempCamera->SetFogColor(RN::Color(2.0f, 2.0f, 3.0f));
			
			FullscreenEffects::GetSharedInstance()->CreateBloomPipeline(tempCamera);
			
			_camera = tempCamera;
			_camera->AddAttachment(audioListener);
		}
		_camera->SetFlags(_camera->GetFlags()|RN::SceneNode::Flags::NoSave);
		
		RN::Renderer::GetSharedInstance()->SetHDRExposure(1.0f);
		RN::Renderer::GetSharedInstance()->SetHDRWhitePoint(8.0f);
		
		// Background ambient
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
#if RN_PLATFORM_MAC_OS == 0
			if(!_hmd)
			{
				RN::ShadowParameter shadowParameter(static_cast<RN::Camera*>(_camera), 2048);
				shadowParameter.distanceBlendFactor = 0.004f;
				sun->ActivateShadows(shadowParameter);
			}
#endif
			sun->SetRotation(RN::Vector3(60.0f, -20.0f, 0.0f));
			
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
			islandMaterial->AddTexture(RN::Texture::WithFile("Models/dirt.png"));
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
			ship->SetWorldPosition(RN::Vector3(-15.21, -13.62, 113.27));
			ship->GetModel()->GetMaterialAtIndex(0, 2)->Define("RN_SPECULARITY");
			ship->GetModel()->GetMaterialAtIndex(0, 2)->SetSpecularColor(RN::Color(0.1f, 0.08f, 0.05f, 250.0f));
			ship->GetModel()->GetMaterialAtIndex(0, 3)->Define("RN_SPECULARITY");
			ship->GetModel()->GetMaterialAtIndex(0, 3)->SetSpecularColor(RN::Color(0.1f, 0.08f, 0.05f, 250.0f));
			
			_turm = new Turm();
			_turm->GetModel()->GetMaterialAtIndex(0, 0)->SetCullMode(RN::Material::CullMode::None);
			
			_crashingShip = new CrashingShip();
			
			StaticEntity *turmplatform = new StaticEntity("Models/geruest_platform.sgm");
			turmplatform->GetModel()->GetMaterialAtIndex(0, 0)->SetCullMode(RN::Material::CullMode::None);
			
			StaticEntity *zaun = new StaticEntity("Models/zaunacker.sgm");
			zaun->GetModel()->GetMaterialAtIndex(0, 0)->SetCullMode(RN::Material::CullMode::None);
			
			StaticEntity *wueste = new StaticEntity("Models/wueste.sgm");
			wueste->SetPosition(RN::Vector3(1000.0f, -200.0f, 100.0f));
			wueste->GetModel()->GetMaterialAtIndex(0, 0)->AddTexture(RN::Texture::WithFile("Models/dirt3.png"));
			wueste->GetModel()->GetMaterialAtIndex(0, 0)->SetShader(RN::Shader::WithFile("Shaders/Island"));
			wueste->GetModel()->GetMaterialAtIndex(0, 0)->Define("TILE_FACTOR", 100.0f);
			
			StaticEntity *wuesteObjekte = new StaticEntity("Models/wuesteobjekte.sgm");
			wuesteObjekte->SetPosition(RN::Vector3(1000.0f, -200.0f, 100.0f));
			
			StaticEntity *desertHouse = new StaticEntity("Models/huette_fixed.sgm");
			desertHouse->SetPosition(RN::Vector3(920.262f, -165.618f, -850.0f));

			StaticEntity *desertHouseRuin = new StaticEntity("Models/huette2.sgm");
			desertHouseRuin->SetPosition(RN::Vector3(912.768f, -166.144f, -843.665f));
			desertHouseRuin->SetRotation(RN::Vector3(8.184f, 0.0f, 0.0f));

			StaticEntity *desertHouseBig = new StaticEntity("Models/huette3.sgm");
			desertHouseBig->SetPosition(RN::Vector3(894.02f, -166.528f, -845.12f));
			desertHouseBig->SetRotation(RN::Vector3(-29.24f, 0.0f, 0.0f));
			
			StaticEntity *vulkan = new StaticEntity("Models/vulcano.sgm");
			vulkan->SetPosition(RN::Vector3(500.0f, 200.0f, 1000.0f));
			RN::Material * vulkanMaterial = vulkan->GetModel()->GetMaterialAtIndex(0, 0);
			vulkanMaterial->RemoveTextures();
			vulkanMaterial->AddTexture(RN::Texture::WithFile("Models/vulcano_ao.png"));
			vulkanMaterial->AddTexture(RN::Texture::WithFile("Models/vulcano_splat.png"));
			vulkanMaterial->AddTexture(RN::Texture::WithFile("Models/dirt.png"));
			vulkanMaterial->AddTexture(RN::Texture::WithFile("Models/dirt3.png"));
			vulkanMaterial->AddTexture(RN::Texture::WithFile("Models/dirt2.png"));
			vulkanMaterial->AddTexture(RN::Texture::WithFile("Models/dirt4.png"));
			vulkanMaterial->AddTexture(RN::Texture::WithFile("Models/dirt.png"));
			vulkanMaterial->SetShader(RN::Shader::WithFile("Shaders/Island"));
			vulkanMaterial->Define("SPLAT_NO_ALPHA");
			
			PickupEntity *steeringWheel = new PickupEntity("Models/steeringwheel.sgm", ITEM_TYPE::STEERINGWHEEL);
			steeringWheel->SetPosition(RN::Vector3(23.757f, -1.779f, 33.857f));
			steeringWheel->SetWorldRotation(RN::Vector3(3.24f, -15.16f, -28.641f));

			PickupEntity *bottle = new PickupEntity("Models/flasche.sgm", ITEM_TYPE::BOTTLE);
			bottle->SetPosition(RN::Vector3(910.741f, -165.248f, -851.592f));
			bottle->SetScale(RN::Vector3(4.0f, 4.0f, 4.0f));

			Area *dustArea = new Area(25.0f, AREA_TYPE::DESERT_DUST);
			dustArea->SetPosition(RN::Vector3(1123.616f, -247.31f, 387.43f));

			Area *vulcano = new Area(10.0f, AREA_TYPE::VULCANO);
			vulcano->SetPosition(RN::Vector3(489.34f, 206.0f, 958.28f));

			_vulcanoEmitter = new RN::GenericParticleEmitter();
			_vulcanoEmitter->SetRenderGroup(0);
			_vulcanoEmitter->SetSpawnRate(0.02f);
			_vulcanoEmitter->SetLifeSpan(RN::Vector2(6.0f, 10.0f));
			_vulcanoEmitter->SetStartColor(RN::Color(200, 20, 20));
			_vulcanoEmitter->SetEndColor(RN::Color(255, 255, 40));
			_vulcanoEmitter->SetStartSize(RN::Vector2(1.0f, 2.0f));
			_vulcanoEmitter->SetEndSize(RN::Vector2(0.5f, 1.0f));
			_vulcanoEmitter->SetGravity(RN::Vector3(0.0f, 1.1f, 0.0f));
			_vulcanoEmitter->SetVelocityRandomizeMax(RN::Vector3(1.01f, 1.01f, 1.01f));
			_vulcanoEmitter->SetVelocityRandomizeMin(RN::Vector3(-1.01f, 0.0f, -1.01f));
			_vulcanoEmitter->SetPositionRandomizeMax(RN::Vector3());
			_vulcanoEmitter->SetPositionRandomizeMin(RN::Vector3());
			_vulcanoEmitter->SetPosition(RN::Vector3(490.0f, 198.0f, 958.0f));
			_vulcanoEmitter->SetRotation(RN::Vector3(0.0f, 0.0f, 0.0f));
		}
/*
		// Cactus
		RN::Model *cactuse[] = {RN::Model::WithFile("Models/cactus.sgm"), RN::Model::WithFile("Models/cactus1.sgm"), RN::Model::WithFile("Models/cactus2.sgm")};
		
		RN::InstancingNode *cactusNode = new RN::InstancingNode();
		cactusNode->SetModels(RN::Array::WithObjects(cactuse[0], cactuse[1], cactuse[2]));
		cactusNode->SetFlags(cactusNode->GetFlags() | RN::SceneNode::Flags::NoSave);
		
#if RN_PLATFORM_MAC_OS
		if(_hmd)
		{
			cactusNode->SetPivot(_camera->Downcast<RO::Camera>()->GetLeftCamera());
		}
		else
#endif
		{
			cactusNode->SetPivot(_camera->Downcast<RN::Camera>());
		}
		cactusNode->SetMode(RN::InstancingNode::Mode::Thinning | RN::InstancingNode::Mode::Clipping);
		cactusNode->SetCellSize(128.0f);
		cactusNode->SetClippingRange(256.0f);
		cactusNode->SetThinningRange(256.0f);
		cactusNode->Release();
		
		RN::Random::MersenneTwister random;
		random.Seed(0x1024);
		for(int i = 0; i < 7000; i ++)
		{
			RN::Vector3 pos(random.GetRandomFloatRange(-800.0f, 1200.0f), 500.0f, random.GetRandomFloatRange(-1200.0f, 1200.0f));
			pos += RN::Vector3(1000.0f, -200.0f, 100.0f);
			
			RN::Hit hit = _physicsWorld->CastRay(pos, RN::Vector3(pos.x, pos.y-1000.0f, pos.z));
			if(hit.distance <= 0.0f)
				continue;
			
			pos.y -= hit.distance;
			
			int32 index = random.GetRandomInt32Range(0, 2);
			
			RN::Entity *entity = new RN::Entity(cactuse[index], pos);
			entity->SetFlags(entity->GetFlags() | RN::SceneNode::Flags::Static | RN::SceneNode::Flags::NoSave);
			entity->SetScale(RN::Vector3(random.GetRandomFloatRange(0.2f, 0.3f)));
			entity->SetRotation(RN::Vector3(random.GetRandomFloatRange(0, 360.0f), 0.0f, 0.0f));
			entity->Release();
			
			cactusNode->AddChild(entity);
		}
		*/

		if(_hmd)
		{
			RN::Timer::ScheduledTimerWithDuration(std::chrono::seconds(6), [this] {
				_hmd->DismissSafetyWarning();
			}, false);
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
		if(!_hasPlayedIntro)
		{
			StartIntro();
		}
	}
	
	void World::StartIntro()
	{
		_crashingShip->StartAnimation();
		_turm->StartAnimation();
		Player::GetSharedInstance()->StartAnimation();
		_hasPlayedIntro = true;
	}
}
