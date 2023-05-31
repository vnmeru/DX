#include "Model.h"

#include <iterator>
#include <sstream>
#include <string>

#include "Engine/Renderer/d3dx12.h"
#include "Engine/Renderer/DirectX12.h"

const std::string ModelDir = "Models\\";

void Model::Init()
{
	const auto& DX = DirectX12::GetInstance();

	// Initialize vertex buffer
	int VertexBufferSize = GetVertexBufferSize();
	// Create a default heap to hold the vertex buffer
	D3D12_HEAP_PROPERTIES defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(VertexBufferSize);
	DX.m_pDevice->CreateCommittedResource(
		&defaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_COMMON, // Start in common state
		nullptr,
		IID_PPV_ARGS(&vertexBuffer));

	// Create an upload heap for the vertex buffer
	D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	DX.m_pDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
		nullptr,
		IID_PPV_ARGS(&vBufferUploadHeap));
	vBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");

	// Initialize index buffer
	int IndexBufferSize = GetIndexBufferSize();
	numModelIndices = IndexBufferSize / sizeof(DWORD);

	{
		auto Buffer = CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize);
		// Create a default heap to hold the index buffer
		DX.m_pDevice->CreateCommittedResource(
			&defaultHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&Buffer,
			D3D12_RESOURCE_STATE_COMMON, // Start in common state
			nullptr,
			IID_PPV_ARGS(&indexBuffer));
		indexBuffer->SetName(L"Index Buffer Resource Heap");
	}
	{
		auto Buffer = CD3DX12_RESOURCE_DESC::Buffer(IndexBufferSize);
		// Create an upload heap for the index buffer
		DX.m_pDevice->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&Buffer,
			D3D12_RESOURCE_STATE_GENERIC_READ, // GPU will read from this buffer and copy its contents to the default heap
			nullptr,
			IID_PPV_ARGS(&iBufferUploadHeap));
		iBufferUploadHeap->SetName(L"Index Buffer Upload Resource Heap");
	}
	// Initialize vertex buffer view
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = VertexBufferSize;

	// Initialize index buffer view
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = IndexBufferSize; // 32-bit unsigned integer (dword)
}

void Model::Prepare(ID3D12GraphicsCommandList* _commandList)
{
	// Create subresource data for vertex buffer
	D3D12_SUBRESOURCE_DATA vertexData;
	vertexData.pData = reinterpret_cast<BYTE*>(Vertexes.data()); // Pointer to vertex array
	vertexData.RowPitch = GetVertexBufferSize(); // Size of vertex buffer
	vertexData.SlicePitch = GetVertexBufferSize(); // Same as row pitch

	// Copy vertex buffer data from upload heap to default heap
	UpdateSubresources(_commandList, vertexBuffer, vBufferUploadHeap, 0, 0, 1, &vertexData);
	{
		auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER);
		// Transition vertex buffer to vertex and constant buffer state
		_commandList->ResourceBarrier(1, &Barrier);
	}
	// Create subresource data for index buffer
	D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = reinterpret_cast<BYTE*>(Indexes.data()); // Pointer to index array
	indexData.RowPitch = GetIndexBufferSize(); // Size of index buffer
	indexData.SlicePitch = GetIndexBufferSize(); // Same as row pitch

	// Copy index buffer data from upload heap to default heap
	UpdateSubresources(_commandList, indexBuffer, iBufferUploadHeap, 0, 0, 1, &indexData);

	// Transition index buffer to vertex and constant buffer state
	{
		auto Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			indexBuffer, D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER | D3D12_RESOURCE_STATE_INDEX_BUFFER);
		_commandList->ResourceBarrier(1, &Barrier);
	}
}

void Model::LoadFromFile(const std::string& FileName, bool useCustomColor, XMFLOAT4 CustomColor)
{
	HANDLE hFile = CreateFileA((ModelDir + FileName).c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	DWORD fileSize = GetFileSize(hFile, nullptr);
	std::vector<char> buffer(fileSize);
	DWORD bytesRead = 0;
	if (!ReadFile(hFile, buffer.data(), fileSize, &bytesRead, nullptr))
	{
		CloseHandle(hFile);
		return;
	}

	CloseHandle(hFile);

	LoadFromMemory(buffer, useCustomColor, CustomColor);
}

Model::Model()
{
	static int cBufferCounter = 0;
	MemoryOffset = cBufferCounter++;
}

bool Model::LoadFromMemory(const std::vector<char>& buffer, bool useCustomColor, XMFLOAT4 CustomColor)
{

	std::istringstream iss(std::string(buffer.begin(), buffer.end()));
	std::string line;
	std::vector<XMFLOAT3> positions; // A separate list to store the positions
	positions.reserve(buffer.size());
	Vertexes.reserve(buffer.size() / 32);

	std::vector<XMFLOAT2> uvs; // A separate list to store the UV coordinates

	while (std::getline(iss, line))
	{
		std::istringstream lineStream(line);
		std::string token;
		lineStream >> token;

		if (token == "v")
		{
			XMFLOAT3 pos;
			lineStream >> pos.x >> pos.y >> pos.z;
			positions.emplace_back(pos);
		}
		else if (token == "vt") // Process texture coordinates
		{
			XMFLOAT2 uv = {};
			lineStream >> uv.x >> uv.y;
			uv.y = 1.0f - uv.y; // Flip UV coordinate by Y-axis.
			uvs.push_back(uv);
		}
		else if (token == "f")
		{
			int Quad[4] = { -1 };
			int numIndexes = 0;
			int uvIndex[4] = { -1 }; // Store UV indices for each vertex

			while (lineStream >> token)
			{
				size_t pos = token.find('/');
				if (pos != std::string::npos)
				{
					uvIndex[numIndexes] = std::stoi(token.substr(pos + 1)) - 1;
					token = token.substr(0, pos);
				}
				Quad[numIndexes++] = std::stoi(token) - 1;
			}

			static const int triangleIndices[] = { 0, 1, 2 };
			static const int quadIndices[] = { 3, 0, 1, 1, 2, 3 };

			const int* indices = (numIndexes == 3) ? triangleIndices : quadIndices;
			const int numTriangles = (numIndexes == 3) ? 1 : 2;

			for (int t = 0; t < numTriangles * 3; ++t)
			{
				int i = indices[t];
				Vertexes.emplace_back(positions[Quad[i]].x, positions[Quad[i]].y, positions[Quad[i]].z,
					useCustomColor ? CustomColor.x : 1, useCustomColor ? CustomColor.y : 1,
					useCustomColor ? CustomColor.z : 1, useCustomColor ? CustomColor.w : 1,
					uvs[uvIndex[i]].x, uvs[uvIndex[i]].y);
				Indexes.push_back(static_cast<DWORD>(Vertexes.size() - 1));
			}
		}
	}
	Init();
	return true;
}
