#pragma once
#include <unordered_map>
class CharacterComponent : public BaseComponent
{
public:
	CharacterComponent(ModelAnimator* pAnimator, float scale = 1.f);
	void SetScale(float scale) { m_Scale = scale; }
	float GetScale() const { return m_Scale; }
	void SetAnimator(ModelAnimator* animator) { 
		m_pAnimator = animator;
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
		}
	}
	ModelAnimator* GetAnimator() const { return m_pAnimator; }
protected:
	void Update(const SceneContext& sceneContext) override;
	void Initialize(const SceneContext& sceneContext) override;
	void AddAnimation(std::wstring name);
	void ChangeState(int id);
private:
	int m_AnimationClipId{ 0 };
	UINT m_ClipCount{};
	float m_AnimationSpeed{ 1.f };
	float m_Scale{ 1.f };
	ModelAnimator* m_pAnimator{};

	std::unordered_map<int,std::wstring> m_Animations{};
};

