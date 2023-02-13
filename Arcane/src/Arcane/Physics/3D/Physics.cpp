#include "arcpch.h"
#include "Physics.h"

namespace Arcane
{
	Scene* Physics::s_Scene = nullptr;
	PhysicsScene* Physics::s_PhysicsScene = nullptr;
	PhysicsSettings Physics::s_Settings;

	void Physics::Init(Scene *scene)
	{
		s_Scene = scene;
	}

	void Physics::Shutdown()
	{
		DestroyScene();
	}

	void Physics::CreateScene()
	{
		s_PhysicsScene = new PhysicsScene(s_Settings, s_Scene);
	}

	void Physics::DestroyScene()
	{
		if (s_PhysicsScene)
			delete s_PhysicsScene;
	}

	void Physics::Update(float deltatime)
	{
		s_PhysicsScene->Simulate(deltatime);
	}
}
