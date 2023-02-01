#include "arcpch.h"
#include "PhysicsScene.h"

#include <Arcane/Scene/Components.h>

namespace Arcane
{
	PhysicsScene::PhysicsScene(const PhysicsSettings &settings, Scene *scene) : m_Settings(settings)
	{
		m_CollisionConfig = new btDefaultCollisionConfiguration();
		m_CollisionDispatcher = new btCollisionDispatcher(m_CollisionConfig);
		m_BroadPhaseInterface = new btDbvtBroadphase();
		m_ImpulseConstraintSolver = new btSequentialImpulseConstraintSolver();
		m_DiscreteDyanmicsWorld = new btDiscreteDynamicsWorld(m_CollisionDispatcher, m_BroadPhaseInterface, m_ImpulseConstraintSolver, m_CollisionConfig);
		m_DiscreteDyanmicsWorld->setGravity(btVector3(m_Settings.Gravity.x, m_Settings.Gravity.y, m_Settings.Gravity.z));

		scene->m_Registry.on_construct<RigidBodyComponent>().connect<&PhysicsScene::OnRigidBodyComponentCreate>(this);
	}

	PhysicsScene::~PhysicsScene()
	{

	}

	void PhysicsScene::Simulate()
	{

	}

	void PhysicsScene::OnRigidBodyComponentCreate()
	{

	}
}
