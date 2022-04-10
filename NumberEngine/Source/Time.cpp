#include <NumberEngine/Core/Time.hpp>

#include <Windows.h>
#include <stdio.h>

namespace Num {

    static Float old_time;
    static Float new_time;
    static Float delta_time;

    UInt Time::GetTickCount() {
        return ::GetTickCount();
    }

    ULLong Time::GetTickCount64()
    {
        return ::GetTickCount64();
    }

    void Time::UpdateDelta(Float& dt) {
        old_time = new_time;
        new_time = (Float)(GetTickCount());
        dt = (Float)((new_time - old_time) / 1000.0f);
    }

}
