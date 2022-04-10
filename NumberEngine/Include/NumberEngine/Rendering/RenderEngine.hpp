#pragma once
#include <NumberEngine/Minimal.hpp>

namespace Num {

	class Window;
	using VertexBuffer = ULLong;
	using IndexBuffer = ULLong;
	using ConstantBuffer = ULLong;
	using Shader = ULLong;

	class APIEXPORT RenderEngine {
	public:
		RenderEngine() = default;
		RenderEngine(const Window& wnd);
		~RenderEngine();

		void Viewport(UInt width, UInt height);
		void Present(Bool VSYNC);
		void Clear(Float red, Float green, Float blue, Float alpha);

		void DrawTriangleList(UInt vertexcount, UInt startindex);
		void DrawTriangleListIndexed(UInt indexcount, UInt startvertex, UInt startindexloc);
		void DrawTriangleStrip(UInt vertexcount, UInt startindex);

		void DrawTriangleList(VertexBuffer vb, UInt startvertexindex);
		void DrawTriangleListIndexed(IndexBuffer ib, UInt startvertex, UInt startindexloc);
		void DrawTriangleStrip(VertexBuffer vb, UInt startvertexindex);

		ConstantBuffer CreateConstantBuffer(void* buffer, UInt memsize);
		void SetConstatntBuffer(Shader s, ConstantBuffer cb);
		void UpdateConstantBuffer(ConstantBuffer cb, void* buffer);
		void ReleaseConstantBuffer(ConstantBuffer cb);

		Shader CreateShader(CString shader_filename);
		void GetShaderBytecode(Shader s, void** bytecode, UInt* size);
		void SetShader(Shader s);

		static RenderEngine& Get();

		VertexBuffer CreateVertexBuffer(void* vertices, UInt vertsize, UInt listsize, void* shaderbytecode, UInt sizeshader);
		void SetVertexBuffer(VertexBuffer vb);
		UInt GetNumVertices(VertexBuffer vb);
		void ReleaseVertexBuffer(VertexBuffer vb);

		IndexBuffer CreateIndexBuffer(void* indices, UInt listsize);
		void SetIndexBuffer(IndexBuffer ib);
		UInt GetNumIndices(IndexBuffer ib);
		void ReleaseIndexBuffer(IndexBuffer ib);
	private:
		static RenderEngine* engine;
		class RenderEngineImpl;
		RenderEngineImpl* pimpl;
	};

}
