#include "PingleLogo.h"

PingleLogo::PingleLogo(XMFLOAT4 Position): Entity(Position)
{
	LoadModel("PingleLogo.obj",true, {0,1,0,1});
	SetRotation(90.f, 90.f, 0.f);

	m_Model->SetRenderMode("wireframe");
}

void PingleLogo::Update()
{
	Rotate(0.f, 10.f, 0.f);
}
