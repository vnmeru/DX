#include "Entity.h"

#include "Game.h"
#include "Engine/Renderer/DirectX12.h"

bool Entity::LoadModel(const char* pszPath, bool useCustomColor, XMFLOAT4 CustomColor)
{
	if (m_Model)
	{
		logger.Log(ERR, "Model already exists!");
		return false;
	}
	m_Model = new Model();
	m_Model->LoadFromFile(pszPath, useCustomColor, CustomColor);

	return true;
}

void Entity::SetRotation(float pitch, float yaw, float roll)
{
	const XMMATRIX Rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), XMConvertToRadians(roll));
	XMStoreFloat4x4(&m_Transform.RotMat, Rotation);
}

void Entity::Rotate(float pitch, float yaw, float roll)
{
	m_Transform.Rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(pitch), XMConvertToRadians(yaw), XMConvertToRadians(roll));
}

void Entity::UpdateRender()
{
	if(!IsRenderable())
	{
		logger.Log(ERR, "Trying to render non renderable model!");
		return;
	}

	// Calculate the new rotation matrix by multiplying the current rotation matrix by the rotation quaternion
	const XMMATRIX rotMat = XMLoadFloat4x4(&m_Transform.RotMat) * m_Transform.Rotation;
	// Store the new rotation matrix in the entity's transform
	XMStoreFloat4x4(&m_Transform.RotMat, rotMat);

	// Create a translation matrix based on the entity's position vector
	const XMMATRIX translationMat = XMMatrixTranslationFromVector(XMLoadFloat4(&m_Transform.Position));
	// Create the world matrix by first applying the rotation, then the translation, and finally the scale
	const XMMATRIX worldMat = rotMat * translationMat * m_Transform.Scale;
	// Store the new world matrix in the entity's transform
	XMStoreFloat4x4(&m_Transform.WorldMat, worldMat);

	// Load the camera's view and projection matrices
	const XMMATRIX viewMat = XMLoadFloat4x4(&Game::GetInstance().Camera.cameraViewMat);
	const XMMATRIX projMat = XMLoadFloat4x4(&Game::GetInstance().Camera.cameraProjMat);

	// Calculate the final WVP matrix by multiplying the world matrix by the view and projection matrices
	const XMMATRIX wvpMat = XMLoadFloat4x4(&m_Transform.WorldMat) * viewMat * projMat;
	// Transpose the WVP matrix for compatibility with the GPU
	const XMMATRIX transposed = XMMatrixTranspose(wvpMat);
	// Store the transposed WVP matrix in the entity's constant buffer
	XMStoreFloat4x4(&GetModel()->cBuffer.wvpMat, transposed);

	// Copy the entity's constant buffer to the GPU's constant buffer
	// Calculate the offset in the constant buffer based on the entity's memory offset
	// Note: ConstantBufferPerObjectAlignedSize is the size of each object's constant buffer in bytes, and m_nFrameIndex is the current frame index used by the GPU
	memcpy(DirectX12::GetInstance().m_pCBVGPUAddress[DirectX12::GetInstance().m_nFrameIndex] + (ConstantBufferPerObjectAlignedSize * GetModel()->MemoryOffset), &GetModel()->cBuffer.wvpMat, sizeof(GetModel()->cBuffer));
}