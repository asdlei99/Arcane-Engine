#pragma once

#include <Arcane/Physics/3D/PhysicsScene.h>
#include <Arcane/Physics/3D/PhysicsSettings.h>

namespace Arcane
{
	class Physics
	{
	public:
		static void Init();
		static void Shutdown();

		static void CreateScene();
		static void DestroyScene();
	private:
		static PhysicsScene *s_Scene;
		static PhysicsSettings s_Settings;
	};
}
