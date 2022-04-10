#pragma once
#include <NumberEngine/Minimal.hpp>

namespace Num {

	using KeyCode = UShort;
	using MouseButton = UShort;

	namespace Key {

		enum : KeyCode {
			Escape = 0,
			LShift,
			A,
			B,
			C,
			D,
			E,
			F,
			G,
			H,
			I,
			J,
			K,
			L,
			M,
			N,
			O,
			P,
			Q,
			R,
			S,
			T,
			U,
			V,
			W,
			X,
			Y,
			Z,
			N0,
			N1,
			N2,
			N3,
			N4,
			N5,
			N6,
			N7,
			N8,
			N9,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			RShift,
			LCtrl,
			RCtrl,
			Space,
			Backspace,
			Enter,
			Tab
		};
	}

	namespace Mouse {

		enum : MouseButton {
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,

			ButtonLast = Button7,
			ButtonLeft = Button0,
			ButtonRight = Button2,
			ButtonMiddle = Button1
		};
	}

}
