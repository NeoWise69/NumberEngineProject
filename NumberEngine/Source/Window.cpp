#include <NumberEngine/Rendering/Window.hpp>
#include <NumberEngine/Core/Input.hpp>

#include <NumberEngine/Events/Event.hpp>
#include <NumberEngine/Events/MouseEvent.hpp>
#include <NumberEngine/Events/WindowEvent.hpp>
#include <NumberEngine/Events/KeyboardEvent.hpp>

#include <Windows.h>
#include <assert.h>
#include <string>

namespace Num {

	class Window::WindowImpl {
	public:
		WindowImpl() = default;
		~WindowImpl() = default;

		bool InitWindow();
		void KillWindow();

		static Int ExtractShowCmd(WindowCreateInfo info);
		static LLong WindowProcedure(HWND hWnd, UInt msg, WPARAM wParam, LPARAM lParam);

		WindowCreateInfo m_Info;
		void* Handle;
		Bool m_IsAlive;
		EventCallbackFn EventCallback;

		WNDCLASSEX wc;
	};

	static Short m_KeysStates[256] = {};
	static Short m_OldKeysStates[256] = {};
	static Short m_KeysStatesRes[256] = {};
	static Point m_OldMousePos = {};
	static Point m_ResultDeltaMousePos = {};
	static Point m_ResultMousePos = {};
	static Float m_MouseWheelDelta = {};
	static Bool m_FirstTime = true;

	Bool Input::IsKeyDown(const KeyCode key) {
		Short keyWin = 0;
		
		if (key >= Key::A && key <= Key::Z)
			keyWin = 65 + (key - 2);
		else if (key >= Key::N0 && key <= Key::N9)
			keyWin = '0' + (key - 28);
		else if (key == Key::LShift)
			keyWin = VK_LSHIFT;
		else if (key == Key::RShift)
			keyWin = VK_RSHIFT;
		else if (key == Key::LCtrl)
			keyWin = VK_LCONTROL;
		else if (key == Key::RCtrl)
			keyWin = VK_RCONTROL;
		else if (key == Key::Enter)
			keyWin = VK_RETURN;
		else if (key == Key::Backspace)
			keyWin = VK_BACK;
		else if (key == Key::Tab)
			keyWin = VK_TAB;
		else if (key == Key::Escape)
			keyWin = VK_ESCAPE;

		return  (m_KeysStatesRes[keyWin] == 0);
	}

	Bool Input::IsMouseButtonDown(const MouseButton button) {
		if (button == Mouse::ButtonLeft)
		{
			auto state = m_KeysStatesRes[VK_LBUTTON];
			return (state == 0);

		}
		else if (button == Mouse::ButtonMiddle)
		{
			auto state = m_KeysStatesRes[VK_MBUTTON];
			return (state == 0);
		}
		else if (button == Mouse::ButtonRight)
		{
			auto state = m_KeysStatesRes[VK_RBUTTON];
			return (state == 0);
		}

		return false;
	}

	Bool Input::IsKeyUp(const KeyCode key) {
		Short keyWin = 0;

		if (key >= Key::A && key <= Key::Z)
			keyWin = 65 + (key - 2);
		else if (key >= Key::N0 && key <= Key::N9)
			keyWin = '0' + (key - 28);
		else if (key == Key::LShift)
			keyWin = VK_LSHIFT;
		else if (key == Key::RShift)
			keyWin = VK_RSHIFT;
		else if (key == Key::LCtrl)
			keyWin = VK_LCONTROL;
		else if (key == Key::RCtrl)
			keyWin = VK_RCONTROL;
		else if (key == Key::Enter)
			keyWin = VK_RETURN;
		else if (key == Key::Backspace)
			keyWin = VK_BACK;
		else if (key == Key::Tab)
			keyWin = VK_TAB;
		else if (key == Key::Escape)
			keyWin = VK_ESCAPE;

		return  (m_KeysStatesRes[keyWin] == 1);
	}

	Input& Input::Get() {
		return *this;
	}

	void Input::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Input::OnKeyPress));
		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Input::OnKeyRelease));
	}

	Bool Input::OnKeyPress(KeyPressedEvent& e) {
		
	}

	Bool Input::OnKeyRelease(KeyReleasedEvent& e) {

	}

	Bool Input::IsMouseButtonUp(const MouseButton button) {
		if (button == Mouse::ButtonLeft)
		{
			auto state = m_KeysStatesRes[VK_LBUTTON];
			return (state == 1);

		}
		else if (button == Mouse::ButtonMiddle)
		{
			auto state = m_KeysStatesRes[VK_MBUTTON];
			return (state == 1);
		}
		else if (button == Mouse::ButtonRight)
		{
			auto state = m_KeysStatesRes[VK_RBUTTON];
			return (state == 1);
		}

		return false;
	}

	Float Input::GetMouseWheelDelta() {
		return m_MouseWheelDelta;
	}

	Long Input::GetCursorDeltaPositionX() {
		return m_ResultDeltaMousePos.x;
	}

	Long Input::GetCursorDeltaPositionY() {
		return m_ResultDeltaMousePos.y;
	}

	Point Input::GetCursorDeltaPosition() {
		return { GetCursorDeltaPositionX(), GetCursorDeltaPositionY() };
	}

	Long Input::GetCursorPositionX() {
		return m_ResultMousePos.x;
	}

	Long Input::GetCursorPositionY() {
		return m_ResultMousePos.y;
	}

	Point Input::GetCursorPosition() {
		return { GetCursorPositionX(), GetCursorPositionY() };
	}

	Bool Input::ClipCursorInArea(const Int left, const Int right, const Int top, const Int bottom) {
		RECT clipspace = { left, top, right, bottom };
		return ClipCursor(&clipspace);
	}

	void Input::ShowCursor(const Bool visible) {
		::ShowCursor(visible);
	}

	void InputUpdateState(Bool handle) {
		if (handle) {
			if (m_MouseWheelDelta < 0)
				m_MouseWheelDelta += m_MouseWheelDelta * -0.2f;
			else if (m_MouseWheelDelta > 0)
				m_MouseWheelDelta -= m_MouseWheelDelta * 0.2f;

			m_ResultDeltaMousePos = {};
			m_ResultMousePos = {};

			POINT current_mouse_position = {};
			::GetPhysicalCursorPos(&current_mouse_position);

			if (m_FirstTime) {
				m_OldMousePos = { current_mouse_position.x, current_mouse_position.y };
				m_FirstTime = false;
			}

			if (*(Point*)&current_mouse_position != m_OldMousePos) {
				m_ResultDeltaMousePos = { current_mouse_position.x - m_OldMousePos.x, current_mouse_position.y - m_OldMousePos.y };
			}

			m_OldMousePos = { current_mouse_position.x, current_mouse_position.y };
			m_ResultMousePos = { current_mouse_position.x, current_mouse_position.y };

			for (UInt i = 0; i < 256; i++)
			{
				m_KeysStates[i] = ::GetAsyncKeyState(i);

				if (m_KeysStatesRes[i] & 0x8001)
				{
					m_KeysStatesRes[i] = 0;
				}
				else // KEY IS UP
				{
					if (m_KeysStates[i] != m_OldKeysStates[i])
						m_KeysStatesRes[i] = 1;
					else
						m_KeysStatesRes[i] = 2;
				}
			}
			::memcpy(m_OldKeysStates, m_KeysStates, 512);

		}
	}

	static std::string GetLastErrorAsString()
	{
		DWORD errorMessageID = ::GetLastError();
		if (errorMessageID == 0) {
			return std::string();
		}

		LPSTR messageBuffer = nullptr;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);

		LocalFree(messageBuffer);

		return message;
	}

	static void PrintLastError() {
		std::string error = GetLastErrorAsString();
		if (!error.empty()) {
			printf("Error: %s", error.c_str());
		}
	}


	bool Window::WindowImpl::InitWindow() {

#ifdef NDEBUG
		ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

		// Starting boooooring windows window creation
		wc = { 0 };
		wc.cbSize = 80U; // sizeof(WNDCLASSEX);
		wc.lpszClassName = L"NumberEngine::WindowImpl";
		wc.lpfnWndProc = WindowImpl::WindowProcedure;

		// check the errors ?
		RegisterClassExW(&wc);

		std::string normal_title = m_Info.Title;
		std::wstring wide_title = std::wstring(normal_title.begin(), normal_title.end());

		LPCWSTR title = wide_title.c_str();

		// create actual window
		Handle = CreateWindowExW(NULL, wc.lpszClassName, title, WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU, m_Info.X, m_Info.Y, m_Info.Width, m_Info.Height, NULL, NULL, NULL, NULL);
		if (Handle) {
			return true;
		}

		PrintLastError();

		return false;
	}

	void Window::WindowImpl::KillWindow() {
		if (this && Handle) {
			DestroyWindow((HWND)(Handle));
			m_IsAlive = false;
		}
	}

	Int Window::WindowImpl::ExtractShowCmd(WindowCreateInfo info) {
		switch (info.StartState)
		{
			case StartState_Maximized: return 3;
			case StartState_Minimized: return 2;
			case StartState_Fullscreen: return 3;
			case StartState_Normal: return 5;
		default:
			return 5;
			break;
		}
	}



	LLong Window::WindowImpl::WindowProcedure(HWND hWnd, UInt msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
			Window* data = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			if (data) {
				Char c = MapVirtualKeyW((UInt)(wParam), MAPVK_VK_TO_CHAR);
				KeyPressedEvent event(c, 0);
				data->pimpl->EventCallback(event);
			}
			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP: {
			Window* data = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			if (data) {
				Char c = MapVirtualKeyW((UInt)(wParam), MAPVK_VK_TO_CHAR);
				KeyReleasedEvent event(c);
				data->pimpl->EventCallback(event);
			}
			break;
		}
		case WM_CHAR: {
			Window* data = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			if (data) {
				Char c = MapVirtualKeyW((UInt)(wParam), MAPVK_VK_TO_CHAR);
				KeyTypedEvent event(c);
				data->pimpl->EventCallback(event);
			}
			break;
		}
		case WM_MOUSEWHEEL: {
			m_MouseWheelDelta = (Float)(GET_WHEEL_DELTA_WPARAM(wParam));
			break;
		}
		case WM_DESTROY: {
			Window* data = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			PostQuitMessage(0);
			break;
		}
		case WM_CLOSE: {
			Window* data = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			if (data) {
				WindowCloseEvent event;
				data->pimpl->EventCallback(event);
			}
			break;
		}
		case WM_SETFOCUS: {
			Window* data = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			if (data) {
				static Bool ft = true;
				if (!ft) {
					WindowFocusEvent event;
					data->pimpl->EventCallback(event);
				}
				ft = false;
			}
			break;
		}
		case WM_KILLFOCUS: {
			Window* data = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			if (data) {
				WindowLostFocusEvent event;
				data->pimpl->EventCallback(event);
			}
			break;
		}
		case WM_SIZE: {
			Window* data = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
			if (data) {
				static Bool ft = true;
				data->pimpl->m_Info.Width = LOWORD(lParam) + 16;
				data->pimpl->m_Info.Height = HIWORD(lParam) + 39;
				if (!ft) {
					WindowResizeEvent event(data->pimpl->m_Info.Width, data->pimpl->m_Info.Height);
					data->pimpl->EventCallback(event);
				}
				ft = false;
			}
			break;
		}
		}
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	Window::Window(const WindowCreateInfo& info) {
		pimpl = new WindowImpl;
		if (pimpl) {
			pimpl->m_Info = info;
			pimpl->InitWindow();
			// post init
			SetWindowLongPtrW((HWND)(pimpl->Handle), GWLP_USERDATA, (LONG_PTR)(this));
			ShowWindow((HWND)(pimpl->Handle), WindowImpl::ExtractShowCmd(pimpl->m_Info));
			UpdateWindow((HWND)(pimpl->Handle));
			pimpl->m_IsAlive = true;
		}
	}

	Window::~Window() {
		pimpl->KillWindow();
		delete pimpl;
	}

	void* Window::GetHandle() const {
		return pimpl ? pimpl->Handle : nullptr;
	}

	UInt Window::GetWidth() const {
		return pimpl ? pimpl->m_Info.Width : -1;
	}

	UInt Window::GetHeight() const {
		return pimpl ? pimpl->m_Info.Height : -1;
	}

	CString Window::GetTitle() const {
		return pimpl ? pimpl->m_Info.Title : nullptr;
	}

	void Window::Update(WindowContext& msg) {
		if (PeekMessageW((LPMSG)&msg, (HWND)(pimpl->Handle), NULL, NULL, PM_REMOVE)) {
			TranslateMessage((LPMSG)&msg);
			DispatchMessageW((LPMSG)&msg);
		}

	}
	
	Bool Window::IsAlive() const {
		return pimpl ? pimpl->m_IsAlive : false;
	}

	void Window::SetEventCallback(const EventCallbackFn& fn) {
		pimpl->EventCallback = fn;
	}

	Window& Window::GetWindow() {
		return *this;
	}

}

