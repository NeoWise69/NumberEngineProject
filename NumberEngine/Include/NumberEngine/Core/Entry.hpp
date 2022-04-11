#pragma once
#include <NumberEngine/Core/Application.hpp>

#ifdef _WIN32

extern Num::NApplication* Num::CreateApplication(const NCommandLineArgs& cmd);

#include <Windows.h>

#ifdef _DEBUG

int main(int argc, char* argv[]) {
	auto app = Num::CreateApplication({ argc, argv });
	app->Run();
	delete app;
	return 0;
}

#else

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	auto app = Num::CreateApplication({__argc, __argv});
	app->Run();
	delete app;
	return 0;
}

#endif

#endif
