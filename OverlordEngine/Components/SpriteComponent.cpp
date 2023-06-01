#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color, bool canDraw) :
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color),
	m_CanDraw{ canDraw }
{
	m_enablePostDraw = true;
}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::PostDraw(const SceneContext&)
{
	if (!m_pTexture || !m_CanDraw)
		return;

	TODO_W4(L"Draw the sprite with SpriteRenderer::Draw")

		//Here you need to draw the SpriteComponent using the Draw of the sprite renderer
		// The sprite renderer is a singleton
		// you will need to position (X&Y should be in screenspace, Z contains the depth between [0,1]), the rotation and the scale from the owning GameObject
		// You can use the MathHelper::QuaternionToEuler function to help you with the z rotation 
		auto pTransform = m_pGameObject->GetComponent<TransformComponent>();
	auto pos = pTransform->GetWorldPosition();
	auto scale = pTransform->GetWorldScale();

	SpriteRenderer::Get()->AppendSprite(
		m_pTexture,
		XMFLOAT2{ pos.x,pos.y }, m_Color, m_Pivot, XMFLOAT2{ scale.x,scale.y },
		MathHelper::QuaternionToEuler(pTransform->GetWorldRotation()).z);
}
