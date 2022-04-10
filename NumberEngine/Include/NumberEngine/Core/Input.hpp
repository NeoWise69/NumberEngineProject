#pragma once
#include <NumberEngine/Minimal.hpp>
#include <NumberEngine/Core/Keys.hpp>

namespace Num {

	class APIEXPORT Input {
	public:
		Input();
		~Input();

		void OnEvent(Event& e);
		Bool OnKeyPress(KeyPressedEvent& e);
		Bool OnKeyRelease(KeyReleasedEvent& e);

		Bool IsKeyDown(const KeyCode key);
		Bool IsMouseButtonDown(const MouseButton key);
		Bool IsKeyUp(const KeyCode key);
		Bool IsMouseButtonUp(const MouseButton key);
		Float GetMouseWheelDelta();
		Long GetCursorDeltaPositionX();
		Long GetCursorDeltaPositionY();
		Point GetCursorDeltaPosition();
		Long GetCursorPositionX();
		Long GetCursorPositionY();
		Point GetCursorPosition();
		Bool ClipCursorInArea(const Int left, const Int right, const Int top, const Int bottom);
		void ShowCursor(const Bool visible);

		Input& Get();
	};

}
