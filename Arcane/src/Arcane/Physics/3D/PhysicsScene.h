#pragma once

#include <Arcane/Physics/3D/PhysicsSettings.h>
#include <Arcane/Scene/Scene.h>
#include <btBulletDynamicsCommon.h>

#include "entt.hpp"

namespace Arcane
{
	class PhysicsScene
	{
	public:
		PhysicsScene(const PhysicsSettings &settings, Scene *scene);
		~PhysicsScene();

		void Simulate(float deltatime);
	private:
		void OnRigidBodyComponentCreate(entt::registry &registry, entt::entity entityID);
		void OnColliderComponentCreate(entt::registry &registry, entt::entity entityID);
	private:
		PhysicsSettings m_Settings;
		Scene *m_Scene;

		const u32 m_MaxSubSteps = 4;

		// Bullet instances
		btDefaultCollisionConfiguration *m_CollisionConfig;
		btCollisionDispatcher *m_CollisionDispatcher;
		btBroadphaseInterface *m_BroadPhaseInterface;
		btSequentialImpulseConstraintSolver *m_ImpulseConstraintSolver; // TODO: Options for a multi-threaded solver in Extras/BulletMultiThreaded
		btDiscreteDynamicsWorld *m_DiscreteDyanmicsWorld;
	};
}
