#pragma once
#include <NumberEngine/Minimal.hpp>
#include <NumberEngine/Events/Event.hpp>
#include <functional>

namespace Num {

	// used for message catching... (similar struct to MSG type)
	struct WindowContext {
		void* Handle;
		UInt Message;
		ULLong Param1;
		LLong Param2;
		ULong Time;
		Point pt;
	};

	enum WindowStartState {
		StartState_Unknown		= (1 << 0),
		StartState_Normal		= (1 << 1),
		StartState_Maximized	= (1 << 2),
		StartState_Minimized	= (1 << 3),
		StartState_Fullscreen	= (1 << 4),
	};

	struct WindowCreateInfo {
		CString Title;
		UInt Width, Height;
		UInt X, Y;
		WindowStartState StartState;
	};

	class APIEXPORT Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() = default;
		Window(const WindowCreateInfo& info);
		~Window();

		void* GetHandle() const;

		UInt GetWidth() const;
		UInt GetHeight() const;
		//UInt GetClientWidth() const;
		//UInt GetClientHeight() const;
		CString GetTitle() const;

		void Update(WindowContext& msg);
		Bool IsAlive() const;

		void SetEventCallback(const EventCallbackFn& fn);

		Window& GetWindow();

	private:
		class APIEXPORT WindowImpl;
		WindowImpl* pimpl = nullptr;
	};

}
