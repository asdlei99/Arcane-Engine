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
	private:
		static PhysicsScene *s_Scene;
		static PhysicsSettings s_Settings;
	};
}
