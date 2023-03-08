#include "stdafx.h"
#include "StaticCamera.h"

void StaticCamera::Initialize(const SceneContext&)
{
		m_pCamera = new CameraComponent();
		AddComponent(m_pCamera);
}
