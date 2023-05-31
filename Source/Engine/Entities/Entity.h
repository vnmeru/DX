#pragma once
#include <DirectXMath.h>
#include "../Model/Model.h"
using namespace DirectX; // we will be using the directxmath library

struct Transform
{
	XMFLOAT4 Position = {};
	XMMATRIX Scale = XMMatrixScaling(1.f, 1.f, 1.f); // scale
	XMMATRIX Rotation = {};
	XMFLOAT4X4 WorldMat = {};
	XMFLOAT4X4 RotMat = {};
};
struct Entity
{
	Entity() : Entity(XMFLOAT4(0.f,0.f,0.f,0.f)){};
	Entity(XMFLOAT4 Position)
	{
		// initialize rotation matrix to identity matrix
		m_Transform.Rotation = XMMatrixIdentity();

		// set position vector
		m_Transform.Position = Position;

		// load position vector into a XMVECTOR
		const XMVECTOR posVec = XMLoadFloat4(&m_Transform.Position);

		// create a translation matrix from position vector
		const auto tmpMat = XMMatrixTranslationFromVector(posVec);

		// initialize rotation matrix to identity matrix
		XMStoreFloat4x4(&m_Transform.RotMat, XMMatrixIdentity());

		// store world matrix as a combination of translation and rotation
		XMStoreFloat4x4(&m_Transform.WorldMat, tmpMat);
	}

	bool LoadModel(const char* pszPath, bool useCustomColor = false, XMFLOAT4 CustomColor = {});

	virtual ~Entity() = default;

	bool IsRenderable() const { return m_Model != nullptr; }

	// Returns the model attached to this entity
	Model* GetModel() { return m_Model; }

	// Sets the rotation of the entity using Euler angles
	void SetRotation(float pitch, float yaw, float roll);

	// Rotates the entity using Euler angles
	void Rotate(float pitch, float yaw, float roll);

	// Updates the entity's world matrix and renders it
	void UpdateRender();

	// Returns whether the entity should be updated or not
	virtual bool CanUpdate() const { return false; }

	// Updates the entity
	virtual void Update() {};

	// Pointer to the model attached to this entity
	Model* m_Model = nullptr;

	// The transform of the entity
	Transform m_Transform;
};

