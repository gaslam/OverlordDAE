#include "stdafx.h"
#include "CharacterComponent.h"

CharacterComponent::CharacterComponent(ModelAnimator* pAnimator, float scale) : m_pAnimator{pAnimator}, m_Scale{scale}
{
}

void CharacterComponent::Update(const SceneContext&)
{
}

void CharacterComponent::Initialize(const SceneContext&)
{
	GetTransform()->Scale(m_Scale);
	m_pAnimator->SetAnimation(0);
	m_pAnimator->SetAnimationSpeed(m_AnimationSpeed);

	//Gather Clip Names
	m_ClipCount = m_pAnimator->GetClipCount();
	auto clipNames = new char* [m_ClipCount];
	for (UINT i{ 0 }; i < m_ClipCount; ++i)
	{
		auto wClipName = m_pAnimator->GetClip(static_cast<int>(i)).name;
		auto clipName = StringUtil::utf8_encode(wClipName);
		const auto clipSize = clipName.size();
		clipNames[i] = new char[clipSize + 1];
		strncpy_s(clipNames[i], clipSize + 1, clipName.c_str(), clipSize);
		AddAnimation(wClipName);
		delete clipNames[i];
	}
	delete[] clipNames;
}

void CharacterComponent::AddAnimation(std::wstring name)
{
	m_Animations.insert(std::pair<int, std::wstring>(static_cast<int>(m_Animations.size()),name));
}

void CharacterComponent::ChangeState(int id)
{
	auto clipName = m_Animations[id];
	m_pAnimator->SetAnimation(clipName);
}
