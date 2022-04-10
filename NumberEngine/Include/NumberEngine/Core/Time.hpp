#pragma once
#include <NumberEngine/Minimal.hpp>

namespace Num {

	class APIEXPORT Time {
	public:
		static UInt GetTickCount();
		static ULLong GetTickCount64();

		static void UpdateDelta(Float& dt);

	};

}
