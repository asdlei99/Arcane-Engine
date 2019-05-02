/*
	Author: Mohamed Kazma
	Descriptions: Holds joystick data and handles events sent from the joystick manager via glfw
	and updates the corresponding data it is holding
*/

#pragma once

#define MAX_JOYSTICK_BUTTONS 18

#define ARCANE_GAMEPAD_A 0
#define ARCANE_GAMEPAD_B 1
#define ARCANE_GAMEPAD_X 2
#define ARCANE_GAMEPAD_Y 3
#define ARCANE_GAMEPAD_LEFT_BUMPER 4
#define ARCANE_GAMEPAD_RIGHT_BUMPER 5
#define ARCANE_GAMEPAD_BACK 6
#define ARCANE_GAMEPAD_START 7
#define ARCANE_GAMEPAD_GUIDE 8
#define ARCANE_GAMEPAD_LEFT_THUMB 9
#define ARCANE_GAMEPAD_RIGHT_THUMB 10
#define ARCANE_GAMEPAD_DPAD_UP 11
#define ARCANE_GAMEPAD_DPAD_RIGHT 12
#define ARCANE_GAMEPAD_DPAD_DOWN 13
#define ARCANE_GAMEPAD_DPAD_LEFT 14
#define ARCANE_GAMEPAD_LAST   ARCANE_GAMEPAD_DPAD_LEFT
#define ARCANE_GAMEPAD_CROSS ARCANE_GAMEPAD_A
#define ARCANE_GAMEPAD_CIRCLE   ARCANE_GAMEPAD_B
#define ARCANE_GAMEPAD_SQUARE   ARCANE_GAMEPAD_X
#define ARCANE_GAMEPAD_TRIANGLE   ARCANE_GAMEPAD_Y

namespace arcane
{
	class JoystickInputData
	{
	public:
		// Store the state of each joytick button to be queried
		unsigned char m_ButtonStates[MAX_JOYSTICK_BUTTONS];

		JoystickInputData(char id = 0);
		~JoystickInputData();

		// Update the joystick's connection status
		inline void SetConnection(bool isConnected) { m_Connected = isConnected; }
		// Set the joystick id that we want to query
		inline void SetId(char id) { m_Id = id; }
		// Set the deadzone of the joystick
		inline void SetDeadzone(float dead) { m_Deadzone = dead; }
		// Get the number of buttons that this joystick has
		inline char GetNumButtons() { return m_NumButtons; }
		// Get the left stick values
		inline glm::vec2& GetLeftStick() { return m_LeftStick; }
		// Get Right stick values
		inline glm::vec2& GetRightStick() { return m_RightStick; }
		// Get Triggers
		inline glm::vec2& GetTriggers() { return m_Triggers; }
		// Check if the joystick is connected
		inline bool IsConnected() { return m_Connected; }

		// Update the joystick values by querying for button states
		void Update();

	private:
		// Is the joystick connected
		bool m_Connected; 
		// The id that we will use to query for button states
		char m_Id; 
		// The number of buttons is different between controllers
		int m_NumButtons; 
		// DeadZone value for the analog buttons (range 0-1)
		float m_Deadzone;
		// Axis positions of the left and right analog sticks
		glm::vec2 m_LeftStick;
		glm::vec2 m_RightStick;
		// Values of the left and right triggers
		glm::vec2 m_Triggers;
	};
}
