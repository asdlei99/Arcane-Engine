#pragma once

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
		static PhysicsScene s_Scene;
		static PhysicsSettings s_Settings;
	};
}
