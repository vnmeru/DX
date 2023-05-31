#pragma once

#include <d3d12.h>
#include <string>
#include <vector>

#include "Application.h"

struct Vertex {
	Vertex(float x, float y, float z, float r, float g, float b, float a, float u, float v )
		: pos(x, y, z), color(r, g, b, a), uv(u,v) {}
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 uv;
};

//TODO: Add model manager
class Model
{
public:
	void Init();

	void Prepare(ID3D12GraphicsCommandList* _commandList);

	Model();
	~Model(){}
	void SetTextureName(const std::string& TextureName) { m_TextureName = TextureName; }
	const std::string& GetTextureName() const { return m_TextureName; };
	std::string m_TextureName = "white";

	// This function loads 3D model data from a memory buffer
	// The buffer is assumed to contain ASCII text in the OBJ file format
	bool LoadFromMemory(const std::vector<char>& buffer, bool useCustomColor, XMFLOAT4 CustomColor);
	void LoadFromFile(const std::string& FileName, bool useCustomColor = false, XMFLOAT4 CustomColor = {});

	int GetVertexBufferSize() const { return static_cast<int>(Vertexes.size() * sizeof(Vertex)); };
	int GetIndexBufferSize() const { return static_cast<int>(Indexes.size() * sizeof(DWORD)); };
	
	std::vector<DWORD> Indexes{};
	std::vector<Vertex> Vertexes{};
	const std::vector<DWORD>& GetIndexes() const { return Indexes; }
	const std::vector<Vertex>& GetVertexes() const { return Vertexes; }

	ID3D12Resource* vertexBuffer{}; // a default buffer in GPU memory that we will load vertex data for our triangle into
	ID3D12Resource* indexBuffer{}; // a default buffer in GPU memory that we will load index data for our triangle into
	
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{}; // a structure containing a pointer to the vertex data in gpu memory
										   // the total size of the buffer, and the size of each element (vertex)
	D3D12_INDEX_BUFFER_VIEW indexBufferView{}; // a structure holding information about the index buffer
	ID3D12Resource* vBufferUploadHeap{};
	ID3D12Resource* iBufferUploadHeap{};
	ConstantBufferPerObject cBuffer{};
	int MemoryOffset = 0;
	int numModelIndices{};
	std::string GetRenderMode() const { return RenderMode; }
	void SetRenderMode(const std::string& newRenderMode) { RenderMode = newRenderMode; }
	std::string RenderMode = "default";

};
