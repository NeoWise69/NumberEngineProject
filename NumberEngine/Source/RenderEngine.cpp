#include <NumberEngine/Rendering/RenderEngine.hpp>
#include <NumberEngine/Rendering/Window.hpp>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <unordered_map>

namespace Num {

	RenderEngine* RenderEngine::engine = nullptr;

	class RenderEngine::RenderEngineImpl {
	public:
		Bool InitEngine();
		void KillEngine();

		Bool InitSwapchain(HWND hWnd, UInt width, UInt height);
		void KillSwapchain();
		void PresentSwapchain();

		ID3D11Device* m_Device = nullptr;
		IDXGIDevice* m_DXGIDevice = nullptr;
		IDXGIFactory* m_DXGIFactory = nullptr;
		IDXGIAdapter* m_DXGIAdapter = nullptr;
		IDXGISwapChain* m_DXGISwapchain = nullptr;
		D3D_FEATURE_LEVEL m_FeatureLevel = {};
		ID3D11DeviceContext* m_DeviceContext = nullptr;
		ID3D11RenderTargetView* m_RenderTargetView = nullptr;

		// CB
		struct ConstantBufferData {
			ID3D11Buffer* m_Buffer = nullptr;
			D3D11_BUFFER_DESC m_BufferDesc = {};
			D3D11_SUBRESOURCE_DATA m_BufferData = {};

			ConstantBufferData() : m_Buffer(0), m_BufferDesc{ 0 }, m_BufferData{ 0 } {}
		};

		ConstantBufferData* LoadConstantBuffer(void* buffer, UInt memsize);

		std::unordered_map<ConstantBuffer, ConstantBufferData*> m_ConstantBufferRegistry;

		// shader
		struct ShaderData {
			ID3DBlob* m_VSMem = nullptr;
			ID3DBlob* m_PSMem = nullptr;
			ID3D11VertexShader* m_VS = nullptr;
			ID3D11PixelShader* m_PS = nullptr;

			ShaderData() : 
				m_VSMem(0),
				m_PSMem(0),
				m_VS(0),
				m_PS(0) {}

		};

		ShaderData* CreateAndCompileShader(CString shader_filename);

		std::unordered_map<Shader, ShaderData*> m_ShaderRegistry;


		// VB
		struct VertexBufferData {
			ID3D11Buffer* m_Buffer = nullptr;
			D3D11_BUFFER_DESC m_BufferDesc = {};
			D3D11_SUBRESOURCE_DATA m_BufferData = {};
			ID3D11InputLayout* m_InputLayout = nullptr;
			UInt m_VertexSize = {}, m_ListSize = {};

			VertexBufferData() : m_BufferDesc{ 0 },
				m_BufferData{ 0 },
				m_VertexSize(0),
				m_ListSize(0),
				m_Buffer(0),
				m_InputLayout(0) {}

		};

		VertexBufferData* LoadBuffer(const VertexBuffer vb, void* vertices, UInt vertsize, UInt listsize, void* shaderbytecode, UInt sizeshader);
		
		std::unordered_map<VertexBuffer, VertexBufferData*> m_VertexBufferRegistry;


		// IB
		struct IndexBufferData {
			ID3D11Buffer* m_Buffer = nullptr;
			D3D11_BUFFER_DESC m_BufferDesc = {};
			D3D11_SUBRESOURCE_DATA m_BufferData = {};
			UInt m_ListSize = {};

			IndexBufferData() : m_BufferDesc{ 0 },
				m_BufferData{ 0 },
				m_ListSize(0),
				m_Buffer(0) {}

		};

		IndexBufferData* LoadBuffer(const IndexBuffer ib, void* indices, UInt listsize);
		
		std::unordered_map<IndexBuffer, IndexBufferData*> m_IndexBufferRegistry;

	};

	RenderEngine::RenderEngineImpl::VertexBufferData* RenderEngine::RenderEngineImpl::LoadBuffer(const VertexBuffer vb, void* vertices, UInt vertsize, UInt listsize, void* shaderbytecode, UInt sizeshader) {
		VertexBufferData* data = new VertexBufferData();

		if (data->m_Buffer) data->m_Buffer->Release();
		if (data->m_InputLayout) data->m_InputLayout->Release();

		data->m_BufferDesc = { 0 };
		data->m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		data->m_BufferDesc.ByteWidth = vertsize * listsize;
		data->m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		data->m_BufferDesc.CPUAccessFlags = 0;
		data->m_BufferDesc.MiscFlags = 0;

		data->m_BufferData = { 0 };
		data->m_BufferData.pSysMem = vertices;

		data->m_VertexSize = vertsize;
		data->m_ListSize = listsize;

		if (FAILED(m_Device->CreateBuffer(&data->m_BufferDesc, &data->m_BufferData, &data->m_Buffer))) {
			return nullptr;
		}

		D3D11_INPUT_ELEMENT_DESC layout[] = {
			/* SEMNAME - SEMINDEX - FORMAT - INPUTSLOT - ALIGNEDOFFSET - INPUTSLOTCLASS - INSTDATASTEPRATE */
			{"POSITION", 0,			DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0,			DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,	 D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 1,			DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,	 D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		UInt layout_size = ARRAYSIZE(layout);

		if (FAILED(m_Device->CreateInputLayout(layout, layout_size, shaderbytecode, sizeshader, &data->m_InputLayout))) {
			return nullptr;
		}

		return data;
	}

	RenderEngine::RenderEngineImpl::IndexBufferData* RenderEngine::RenderEngineImpl::LoadBuffer(const IndexBuffer ib, void* indices, UInt listsize) {
		IndexBufferData* data = new IndexBufferData();

		if (data->m_Buffer) data->m_Buffer->Release();

		data->m_BufferDesc = { 0 };
		data->m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		data->m_BufferDesc.ByteWidth = 4 * listsize;
		data->m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		data->m_BufferDesc.CPUAccessFlags = 0;
		data->m_BufferDesc.MiscFlags = 0;

		data->m_BufferData = { 0 };
		data->m_BufferData.pSysMem = indices;

		data->m_ListSize = listsize;

		if (FAILED(m_Device->CreateBuffer(&data->m_BufferDesc, &data->m_BufferData, &data->m_Buffer))) {
			return nullptr;
		}

		return data;
	}


	RenderEngine::RenderEngine(const Window& wnd) {
		pimpl = new RenderEngineImpl;
		if (pimpl) {
			pimpl->InitEngine();
			pimpl->InitSwapchain((HWND)(wnd.GetHandle()), wnd.GetWidth(), wnd.GetHeight());
			engine = this;
		}
	}

	RenderEngine::~RenderEngine() {
		pimpl->KillSwapchain();
		pimpl->KillEngine();
		delete pimpl;
	}

	void RenderEngine::Viewport(UInt width, UInt height) {
		D3D11_VIEWPORT vp = { 0 };
		vp.Width = (Float)(width);
		vp.Height = (Float)(height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		pimpl->m_DeviceContext->RSSetViewports(1, &vp);
	}

	void RenderEngine::Present(Bool VSYNC) {
		pimpl->m_DXGISwapchain->Present(VSYNC, NULL);
	}

	void RenderEngine::Clear(Float red, Float green, Float blue, Float alpha) {
		Float color[] = { red, green, blue, alpha };
		pimpl->m_DeviceContext->ClearRenderTargetView(pimpl->m_RenderTargetView, color);
		pimpl->m_DeviceContext->OMSetRenderTargets(1, &pimpl->m_RenderTargetView, NULL);
	}

	void RenderEngine::DrawTriangleList(UInt vertexcount, UInt startindex) {
		pimpl->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pimpl->m_DeviceContext->Draw(vertexcount, startindex);
	}

	void RenderEngine::DrawTriangleListIndexed(UInt indexcount, UInt startvertex, UInt startindexloc) {
		pimpl->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pimpl->m_DeviceContext->DrawIndexed(indexcount, startindexloc, startvertex);
	}

	void RenderEngine::DrawTriangleList(VertexBuffer vb, UInt startvertexindex) {
		RenderEngineImpl::VertexBufferData* data = pimpl->m_VertexBufferRegistry.at(vb);
		if (data) {
			pimpl->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pimpl->m_DeviceContext->Draw(data->m_ListSize, startvertexindex);
		}
	}

	void RenderEngine::DrawTriangleListIndexed(IndexBuffer ib, UInt startvertex, UInt startindexloc) {
		RenderEngineImpl::IndexBufferData* data = pimpl->m_IndexBufferRegistry.at(ib);
		if (data) {
			pimpl->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pimpl->m_DeviceContext->DrawIndexed(data->m_ListSize, startindexloc, startvertex);
		}
	}

	void RenderEngine::DrawTriangleStrip(UInt vertexcount, UInt startindex) {
		pimpl->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		pimpl->m_DeviceContext->Draw(vertexcount, startindex);
	}

	void RenderEngine::DrawTriangleStrip(VertexBuffer vb, UInt startvertexindex) {
		RenderEngineImpl::VertexBufferData* data = pimpl->m_VertexBufferRegistry.at(vb);
		if (data) {
			pimpl->m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			pimpl->m_DeviceContext->Draw(data->m_ListSize, startvertexindex);
		}
	}

	ConstantBuffer RenderEngine::CreateConstantBuffer(void* buffer, UInt memsize) {
		static ULLong CBCounter = 0;
		pimpl->m_ConstantBufferRegistry[++CBCounter] = pimpl->LoadConstantBuffer(buffer, memsize);
		return CBCounter;
	}

	void RenderEngine::SetConstatntBuffer(Shader s, ConstantBuffer cb){
		RenderEngineImpl::ShaderData* shader = pimpl->m_ShaderRegistry.at(s);
		RenderEngineImpl::ConstantBufferData* constant_buffer = pimpl->m_ConstantBufferRegistry.at(cb);
		if (shader && constant_buffer) {
			pimpl->m_DeviceContext->VSSetConstantBuffers(0, 1, &constant_buffer->m_Buffer);
			pimpl->m_DeviceContext->PSSetConstantBuffers(0, 1, &constant_buffer->m_Buffer);
		}
	}

	void RenderEngine::UpdateConstantBuffer(ConstantBuffer cb, void* buffer) {
		RenderEngineImpl::ConstantBufferData* data = pimpl->m_ConstantBufferRegistry.at(cb);
		if (data) {
			pimpl->m_DeviceContext->UpdateSubresource(data->m_Buffer, NULL, NULL, buffer, NULL, NULL);
		}
	}

	void RenderEngine::ReleaseConstantBuffer(ConstantBuffer cb) {
		RenderEngineImpl::ConstantBufferData* data = pimpl->m_ConstantBufferRegistry.at(cb);
		if (data) {
			data->m_Buffer->Release();

			pimpl->m_ConstantBufferRegistry.erase(cb);
			delete data;
		}
	}

	Shader RenderEngine::CreateShader(CString shader_filename) {
		static ULLong ShaderCounter = 0;
		pimpl->m_ShaderRegistry[++ShaderCounter] = pimpl->CreateAndCompileShader(shader_filename);
		return ShaderCounter;
	}

	void RenderEngine::GetShaderBytecode(Shader s, void** bytecode, UInt* size) {
		*bytecode = pimpl->m_ShaderRegistry.at(s)->m_VSMem->GetBufferPointer();
		*size = (UInt)(pimpl->m_ShaderRegistry.at(s)->m_VSMem->GetBufferSize());
	}

	void RenderEngine::SetShader(Shader s) {
		pimpl->m_DeviceContext->VSSetShader(pimpl->m_ShaderRegistry.at(s)->m_VS, nullptr, 0);
		pimpl->m_DeviceContext->PSSetShader(pimpl->m_ShaderRegistry.at(s)->m_PS, nullptr, 0);
	}

	void RenderEngine::SetVertexBuffer(VertexBuffer vb) {
		RenderEngineImpl::VertexBufferData* data = pimpl->m_VertexBufferRegistry.at(vb);
		if (data) {
			UInt stride = data->m_VertexSize;
			UInt offset = 0;
			pimpl->m_DeviceContext->IASetVertexBuffers(0, 1, &data->m_Buffer, &stride, &offset);
			pimpl->m_DeviceContext->IASetInputLayout(data->m_InputLayout);
		}
	}


	RenderEngine& RenderEngine::Get() {
		return *engine;
	}

	VertexBuffer RenderEngine::CreateVertexBuffer(void* vertices, UInt vertsize, UInt listsize, void* shaderbytecode, UInt sizeshader) {
		static ULLong VBCounter = 0;
		pimpl->m_VertexBufferRegistry[++VBCounter] = pimpl->LoadBuffer(VBCounter, vertices, vertsize, listsize, shaderbytecode, sizeshader);
		return VBCounter;
	}

	UInt RenderEngine::GetNumVertices(VertexBuffer vb) {
		RenderEngineImpl::VertexBufferData* data = pimpl->m_VertexBufferRegistry.at(vb);
		if (data) return data->m_ListSize;
		return 0;
	}

	void RenderEngine::ReleaseVertexBuffer(VertexBuffer vb) {
		RenderEngineImpl::VertexBufferData* data = pimpl->m_VertexBufferRegistry.at(vb);
		if (data) {
			data->m_InputLayout->Release();
			data->m_Buffer->Release();

			pimpl->m_VertexBufferRegistry.erase(vb);
			delete data;
		}
	}

	IndexBuffer RenderEngine::CreateIndexBuffer(void* indices, UInt listsize) {
		static ULLong IBCounter = 0;
		pimpl->m_IndexBufferRegistry[++IBCounter] = pimpl->LoadBuffer(IBCounter, indices, listsize);
		return IBCounter;
	}

	void RenderEngine::SetIndexBuffer(IndexBuffer ib) {
		RenderEngineImpl::IndexBufferData* data = pimpl->m_IndexBufferRegistry.at(ib);
		if (data) {
			pimpl->m_DeviceContext->IASetIndexBuffer(data->m_Buffer, DXGI_FORMAT_R32_UINT, 0);
		}
	}

	UInt RenderEngine::GetNumIndices(IndexBuffer ib) {
		RenderEngineImpl::IndexBufferData* data = pimpl->m_IndexBufferRegistry.at(ib);
		if (data) return data->m_ListSize;
		return 0;
	}

	void RenderEngine::ReleaseIndexBuffer(IndexBuffer ib) {
		RenderEngineImpl::IndexBufferData* data = pimpl->m_IndexBufferRegistry.at(ib);
		if (data) {
			data->m_Buffer->Release();
			pimpl->m_IndexBufferRegistry.erase(ib);
			delete data;
		}
	}

	Bool RenderEngine::RenderEngineImpl::InitEngine() {
		D3D_DRIVER_TYPE driver_type_list[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UInt driver_type_list_size = ARRAYSIZE(driver_type_list);
		D3D_FEATURE_LEVEL feature_level_list[] = {
			D3D_FEATURE_LEVEL_11_0
		};
		UInt feature_level_list_size = ARRAYSIZE(feature_level_list);
		Long result;
		for (UInt driver_type_index = 0; driver_type_index < driver_type_list_size;) {
			result = D3D11CreateDevice(NULL, driver_type_list[driver_type_index], NULL, NULL, feature_level_list, feature_level_list_size, D3D11_SDK_VERSION, &m_Device, &m_FeatureLevel, &m_DeviceContext);
			if (SUCCEEDED(result)) break;
			++driver_type_index;
		}
		if (FAILED(result)) {
			return false;
		}

		m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_DXGIDevice);
		m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_DXGIAdapter);
		m_DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);

		return true;
	}

	void RenderEngine::RenderEngineImpl::KillEngine() {
		m_DXGIFactory->Release();
		m_DXGIAdapter->Release();
		m_DXGIDevice->Release();
		m_DeviceContext->Release();
		m_Device->Release();
	}

	Bool RenderEngine::RenderEngineImpl::InitSwapchain(HWND hWnd, UInt width, UInt height) {
		DXGI_SWAP_CHAIN_DESC desc = { 0 };
		desc.BufferCount = 1;
		desc.BufferDesc.Width = width;
		desc.BufferDesc.Height = height;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.OutputWindow = hWnd;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		desc.Windowed = TRUE;

		Long result = m_DXGIFactory->CreateSwapChain(m_Device, &desc, &m_DXGISwapchain);
		if (FAILED(result)) return false;

		ID3D11Texture2D* backbuffer = NULL;
		result = m_DXGISwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer);
		if (FAILED(result)) return false;

		result = m_Device->CreateRenderTargetView(backbuffer, NULL, &m_RenderTargetView);
		backbuffer->Release();
		if (FAILED(result)) return false;


		return true;
	}

	void RenderEngine::RenderEngineImpl::KillSwapchain() {
		m_DXGISwapchain->Release();
	}

	void RenderEngine::RenderEngineImpl::PresentSwapchain() {
		m_DXGISwapchain->Present(1, 0);
	}

	RenderEngine::RenderEngineImpl::ConstantBufferData* RenderEngine::RenderEngineImpl::LoadConstantBuffer(void* buffer, UInt memsize) {
		ConstantBufferData* data = new ConstantBufferData();

		if (data->m_Buffer) data->m_Buffer->Release();

		data->m_BufferDesc = { 0 };
		data->m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		data->m_BufferDesc.ByteWidth = memsize;
		data->m_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		data->m_BufferDesc.CPUAccessFlags = 0;
		data->m_BufferDesc.MiscFlags = 0;

		data->m_BufferData = { 0 };
		data->m_BufferData.pSysMem = buffer;

		if (FAILED(m_Device->CreateBuffer(&data->m_BufferDesc, &data->m_BufferData, &data->m_Buffer))) {
			return nullptr;
		}

		return data;
	}

	RenderEngine::RenderEngineImpl::ShaderData* RenderEngine::RenderEngineImpl::CreateAndCompileShader(CString shader_filename) {
		ShaderData* shader = new ShaderData();

		ID3DBlob* errblob = nullptr;

		std::string normal_shader_filename = shader_filename;
		std::wstring wide_shader_filename = std::wstring(normal_shader_filename.begin(), normal_shader_filename.end());
		LPCWSTR shader_filename_ = wide_shader_filename.c_str();
		if (FAILED(D3DCompileFromFile(shader_filename_, nullptr, nullptr, "VertexShaderMain", "vs_5_0", NULL, NULL, &shader->m_VSMem, &errblob))) {
			OutputDebugStringA((char*)errblob->GetBufferPointer());
			return nullptr;
		}
		if (FAILED(D3DCompileFromFile(shader_filename_, nullptr, nullptr, "PixelShaderMain", "ps_5_0", NULL, NULL, &shader->m_PSMem, &errblob))) {
			return nullptr;
		}

		if (!shader->m_VSMem || !shader->m_PSMem) return nullptr;
	
		m_Device->CreateVertexShader(shader->m_VSMem->GetBufferPointer(), shader->m_VSMem->GetBufferSize(), nullptr, &shader->m_VS);
		m_Device->CreatePixelShader(shader->m_PSMem->GetBufferPointer(), shader->m_PSMem->GetBufferSize(), nullptr, &shader->m_PS);

		return shader;
	}

}
