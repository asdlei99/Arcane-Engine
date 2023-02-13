#pragma once

#include <Arcane/Physics/3D/PhysicsScene.h>
#include <Arcane/Physics/3D/PhysicsSettings.h>
#include <Arcane/Scene/Scene.h>

namespace Arcane
{
	class Physics
	{
	public:
		static void Init(Scene *scene);
		static void Shutdown();

		static void CreateScene();
		static void DestroyScene();

		static void Update(float deltatime);
	private:
		static Scene *s_Scene;
		static PhysicsScene *s_PhysicsScene;
		static PhysicsSettings s_Settings;
	};
}
