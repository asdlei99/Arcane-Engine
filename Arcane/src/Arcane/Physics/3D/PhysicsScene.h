#pragma once

#include <Arcane/Physics/3D/PhysicsSettings.h>
#include <btBulletDynamicsCommon.h>

namespace Arcane
{
	class PhysicsScene
	{
	public:
		PhysicsScene(const PhysicsSettings &settings);
		~PhysicsScene();

		void Simulate();
	private:
		PhysicsSettings m_Settings;

		float m_Accumulator = 0.0f;
		u32 m_NumSubSteps = 0;
		const u32 m_MaxSubSteps = 4;

		// Bullet instances
		btDefaultCollisionConfiguration *m_CollisionConfig;
		btCollisionDispatcher *m_CollisionDispatcher;
		btBroadphaseInterface *m_BroadPhaseInterface;
		btSequentialImpulseConstraintSolver *m_ImpulseConstraintSolver; // TODO: Options for a multi-threaded solver in Extras/BulletMultiThreaded
		btDiscreteDynamicsWorld *m_DiscreteDyanmicsWorld;
	};
}
