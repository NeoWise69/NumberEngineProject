#include <NumberEngine/Rendering/Window.hpp>
#include <NumberEngine/Events/WindowEvent.hpp>
#include <NumberEngine/Rendering/RenderEngine.hpp>
#include <NumberEngine/Math/Vertex.hpp>
#include <NumberEngine/Math/CB.hpp>
#include <NumberEngine/Core/Time.hpp>
#include <NumberEngine/Core/Input.hpp>

#include <stdio.h>

using namespace Num;

class Testbed {
private:
	vec3 m_Rotational;
	Bool m_Alive = true;
	Bool m_HandleInputEvents = true;
	Float dt;
public:
	Testbed() {
		WindowCreateInfo info = { 0 };
		info.Title = "Test";
		info.Width = 1200U;
		info.Height = 800U;
		info.X = info.Y = 200U;
		info.StartState = StartState_Normal;

		Window MainWindow = Window(info);
		MainWindow.SetEventCallback(BIND_EVENT_FN(Testbed::OnEvent));

		auto _invisible_render_engine_variable_ = RenderEngine(MainWindow);
		Shader default_shader = RenderEngine::Get().CreateShader("Default.fx");

		PositionColorColor1Vertex vertices[] = {
			{vec3(-0.5f, -0.5f,  -0.5f), vec3(1.f,  0.0f, 0.0f), vec3(0.f,  0.0f, 1.0f)},
			{vec3(-0.5f,  0.5f,  -0.5f), vec3(0.0f, 1.f,  0.0f), vec3(1.0f, 0.f,  0.0f)},
			{vec3( 0.5f,  0.5f,  -0.5f), vec3(0.0f, 0.0f, 1.f),	 vec3(0.0f, 1.0f, 0.f)},
			{vec3( 0.5f, -0.5f,  -0.5f), vec3(0.4f, 0.1f, 0.7f), vec3(0.4f, 0.1f, 0.7f)},
			{vec3( 0.5f, -0.5f,   0.5f), vec3(0.4f, 1.0f, 0.7f), vec3(0.4f, 0.1f, 0.7f)},
			{vec3( 0.5f,  0.5f,   0.5f), vec3(1.0f, 0.1f, 0.7f), vec3(0.4f, 0.1f, 0.7f)},
			{vec3(-0.5f,  0.5f,   0.5f), vec3(0.4f, 0.1f, 0.9f), vec3(0.4f, 0.1f, 0.7f)},
			{vec3(-0.5f, -0.5f,   0.5f), vec3(0.4f, 1.0f, 0.7f), vec3(0.4f, 0.1f, 0.7f)},
		};
		UInt vertices_size = ARRSIZE(vertices);

		UInt indices[] = {
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			1, 6, 5,
			5, 2, 1,
			7, 0, 3,
			3, 4, 7,
			3, 2, 5,
			5, 4, 3,
			7, 6, 1,
			1, 0, 7
		};
		UInt indices_size = ARRSIZE(indices);

		IndexBuffer ib = RenderEngine::Get().CreateIndexBuffer(indices, indices_size);

		void* shaderbytecode = nullptr;
		UInt shadersize = 0;

		RenderEngine::Get().GetShaderBytecode(default_shader, &shaderbytecode, &shadersize);

		VertexBuffer vb = RenderEngine::Get().CreateVertexBuffer(vertices, sizeof(vertices[0]), vertices_size, shaderbytecode, shadersize);

		CB cc;
		cc.m_Time = 0;

		ConstantBuffer cb = RenderEngine::Get().CreateConstantBuffer(&cc, sizeof(CB));

		WindowContext ctx;

		mat4x4 m_WorldCam;
		m_WorldCam.translate({ 0, 0, 0 });

		Float m_Forward = 0.0f;

		while (m_Alive) {
			MainWindow.Update(ctx);
			if (Input::IsKeyDown(Key::Escape)) { printf("OK\n"); m_Alive = false; }
			RenderEngine::Get().Viewport(MainWindow.GetWidth(), MainWindow.GetHeight());
			RenderEngine::Get().Clear(0.8f, 0.2f, 0.3f, 1);

			mat4x4 t;

			CB ncc;
			ncc.m_Time = Time::GetTickCount();
			ncc.m_ProjectionMatrix.identity();

			m_Rotational.y += Input::GetCursorDeltaPositionX() * dt * 0.3f;
			m_Rotational.x += Input::GetCursorDeltaPositionY() * dt * 0.3f;

			printf("(%d, %d)     \n", Input::GetCursorPositionX() - info.X, Input::GetCursorPositionY() - info.Y);

			if (Input::IsKeyDown(Key::W))
				m_Forward = 0.3f;
			else if (Input::IsKeyDown(Key::S))
				m_Forward = -0.3f;
			else
				m_Forward = 0;

			/*
			
			ncc.m_WorldMatrix.scale({ 1, 1, 1 });

			t.identity();
			t.rotateZ(0);
			ncc.m_WorldMatrix *= t;

			t.identity();
			t.rotateY(m_Rotational.y);
			ncc.m_WorldMatrix *= t;

			t.identity();
			t.rotateX(m_Rotational.x);
			ncc.m_WorldMatrix *= t;*/

			ncc.m_WorldMatrix.identity();

			mat4x4 world_cam;
			world_cam.identity();

			t.identity();
			t.rotateX(m_Rotational.x);
			world_cam *= t;

			t.identity();
			t.rotateY(m_Rotational.y);
			world_cam *= t;

			vec3 newpos = m_WorldCam.getTranslation() + m_WorldCam.getZDirection() * (m_Forward * 0.3f);

			world_cam.translate(newpos);

			m_WorldCam = world_cam;
			world_cam.inverse();

			ncc.m_ViewMatrix = world_cam;
			/*ncc.m_ProjectionMatrix.orthographic(
				(Float)(MainWindow.GetWidth()) / 200.0f,
				(Float)(MainWindow.GetHeight()) / 200.0f,
				-2.f, 2.f
			);*/
			ncc.m_ProjectionMatrix.perspective(
				1.57f, (Float)((Float)MainWindow.GetWidth() / (Float)MainWindow.GetHeight()), 0.00001f, 100.0f
			);

			RenderEngine::Get().UpdateConstantBuffer(cb, &ncc);


			RenderEngine::Get().SetConstatntBuffer(default_shader, cb);
			RenderEngine::Get().SetShader(default_shader);
			RenderEngine::Get().SetVertexBuffer(vb);
			RenderEngine::Get().SetIndexBuffer(ib);
			RenderEngine::Get().DrawTriangleListIndexed(ib, 0, 0);

			RenderEngine::Get().Present(true);

			Time::UpdateDelta(dt);
		}

		RenderEngine::Get().ReleaseVertexBuffer(vb);
		RenderEngine::Get().ReleaseIndexBuffer(ib);
		RenderEngine::Get().ReleaseConstantBuffer(cb);


	}

	void OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Testbed::OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Testbed::OnWindowClose));
		dispatcher.Dispatch<WindowFocusEvent>(BIND_EVENT_FN(Testbed::OnWindowFocus));
		dispatcher.Dispatch<WindowLostFocusEvent>(BIND_EVENT_FN(Testbed::OnWindowLostFocus));
	}

	Bool OnWindowResize(WindowResizeEvent& e) {
		RenderEngine::Get().Viewport(e.GetWidth(), e.GetHeight());
		return false;
	}

	Bool OnWindowClose(WindowCloseEvent& e) {
		m_Alive = false;
		return false;
	}

	Bool OnWindowFocus(WindowFocusEvent& e) {
		m_HandleInputEvents = true;
		return false;
	}

	Bool OnWindowLostFocus(WindowLostFocusEvent& e) {
		m_HandleInputEvents = false;
		return false;
	}

};

int main() {

	Testbed tb;

	return 0;
}
