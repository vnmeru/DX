#include "WilliamSphere.h"

WilliamSphere::WilliamSphere(XMFLOAT4 pos, bool bCustomColor) : Entity(pos)
{
	LoadModel("sphere.obj");
	
	m_Model->SetTextureName("sphere");
}

void WilliamSphere::Update()
{
	Rotate(0.f, 2.f, 0.f);
}
