#include "stdafx.h"
#include "PostPixelate.h"


PostPixelate::PostPixelate() :
	PostProcessingMaterial(L"Effects/Post/Pixelate.fx"),
	m_Pixels{}
{
}

void PostPixelate::UpdateBaseEffectVariables(const SceneContext&, RenderTarget* pSource)
{
	//Set Source Texture
	const auto pSourceSRV = pSource->GetColorShaderResourceView();
	m_pBaseEffect->GetVariableByName("gTexture")->AsShaderResource()->SetResource(pSourceSRV);
	//From this point on, pSource (RenderTarget) is bound as an SRV (ShaderResourceView, read from) to the pipeline
	//In case we want to use pSource as a RTV (RenderTargetView, render to) we have to unbind it first as an SRV
}

void PostPixelate::SetPixels(const XMFLOAT2& pixels)
{
	m_pBaseEffect->GetVariableByName("gPixelAmount")->AsVector()->SetFloatVector(&pixels.x);
}

void PostPixelate::Initialize(const GameContext&)
{
	m_pVector = m_pBaseEffect->GetVariableByName("gPixelAmount")->AsVector();
}
