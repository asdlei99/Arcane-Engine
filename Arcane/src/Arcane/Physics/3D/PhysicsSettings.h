#pragma once

#include <glm/glm.hpp>

namespace Arcane
{
	struct PhysicsSettings
	{
		float FixedTimestep = (1.0f / 30.0f);
		glm::vec3 Gravity = { 0.0f, -9.81f, 0.0f };
	};
}
