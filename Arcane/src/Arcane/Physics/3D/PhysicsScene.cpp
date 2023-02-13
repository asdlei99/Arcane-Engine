#include "arcpch.h"
#include "PhysicsScene.h"

#include <Arcane/Scene/Components.h>
#include <Arcane/Scene/Entity.h>

namespace Arcane
{
	PhysicsScene::PhysicsScene(const PhysicsSettings &settings, Scene *scene) : m_Settings(settings), m_Scene(scene)
	{
		m_CollisionConfig = new btDefaultCollisionConfiguration();
		m_CollisionDispatcher = new btCollisionDispatcher(m_CollisionConfig);
		m_BroadPhaseInterface = new btDbvtBroadphase();
		m_ImpulseConstraintSolver = new btSequentialImpulseConstraintSolver();
		m_DiscreteDyanmicsWorld = new btDiscreteDynamicsWorld(m_CollisionDispatcher, m_BroadPhaseInterface, m_ImpulseConstraintSolver, m_CollisionConfig);
		m_DiscreteDyanmicsWorld->setGravity(btVector3(m_Settings.Gravity.x, m_Settings.Gravity.y, m_Settings.Gravity.z));

		m_Scene->m_Registry.on_construct<RigidBodyComponent>().connect<&PhysicsScene::OnRigidBodyComponentCreate>(this);
		m_Scene->m_Registry.on_construct<BoxColliderComponent>().connect<&PhysicsScene::OnRigidBodyComponentCreate>(this);
	}

	PhysicsScene::~PhysicsScene()
	{

	}

	void PhysicsScene::Simulate(float deltatime)
	{
		m_DiscreteDyanmicsWorld->stepSimulation(deltatime, m_MaxSubSteps, m_Settings.FixedTimestep);
	}

	void PhysicsScene::OnRigidBodyComponentCreate(entt::registry &registry, entt::entity entityID)
	{
		// Create an entity handle
		Entity entity(m_Scene, entityID);
		if (!entity.HasComponent<BoxColliderComponent>())
			return;

		auto &transform = entity.GetComponent<TransformComponent>();
		auto &rigidbody = entity.GetComponent<RigidBodyComponent>();
		auto &collider = entity.GetComponent<BoxColliderComponent>();

		btCollisionShape *shape = new btBoxShape(btVector3()); // TODO: Manage these new calls

		glm::vec3 physicsOrigin = transform.Translation + collider.Center;

		btTransform physicsTransform;
		physicsTransform.setIdentity();
		physicsTransform.setOrigin(btVector3(physicsOrigin.x, physicsOrigin.y, physicsOrigin.z));

		if (rigidbody.IsStatic == true)
		{
			rigidbody.Mass = 0.0f;
		}

		btVector3 localInertia(0.0f, 0.0f, 0.0f);
		if (!rigidbody.IsStatic)
			shape->calculateLocalInertia(rigidbody.Mass, localInertia);

		// Using motionstate is optional, it provides interpolation capabilities and only synchronizes 'active' objects
		btDefaultMotionState *motionState = new btDefaultMotionState(physicsTransform); // TODO: Manage these new calls
		btRigidBody::btRigidBodyConstructionInfo rbInfo(rigidbody.Mass, motionState, shape, localInertia);
		btRigidBody *physicsRigidBody = new btRigidBody(rbInfo); // TODO: Manage these new calls

		// Add the rb to our dynamics world
		m_DiscreteDyanmicsWorld->addRigidBody(physicsRigidBody);
	}

	void PhysicsScene::OnColliderComponentCreate(entt::registry &registry, entt::entity entityID)
	{
		// Create an entity handle
		Entity entity(m_Scene, entityID);

		if (!entity.HasComponent<RigidBodyComponent>())
			return;

		auto &transform = entity.GetComponent<TransformComponent>();
		auto &rigidbody = entity.GetComponent<RigidBodyComponent>();
		auto &collider = entity.GetComponent<BoxColliderComponent>();
	}
}
