#pragma once
template <typename T>
class SpawnerComponent :
	public BaseComponent
{
public:
	SpawnerComponent(XMFLOAT3& pos, GameObject* pOwner) : m_pOwner{pOwner}, m_Pos{pos}
	{
	}

	void Initialize(const SceneContext&) override
	{
		const bool isBaseOf = std::is_base_of_v<BaseComponent, T> == true;
		ASSERT_IF(!isBaseOf, L"Component is not member of BaseComponent!!");
		m_pComponent = m_pOwner->AddComponent(new T{});
		m_pTransform = m_pOwner->GetTransform();
		m_pTransform->Translate(m_Pos);
	};
	T* Spawn(GameObject* object) const
	{
		const auto pComponent = m_pComponent->Clone<T>(object);
		return pComponent;
	}
private:
	T* m_pComponent{ nullptr };
	TransformComponent* m_pTransform{ nullptr };
	XMFLOAT3 m_Pos;
	GameObject* m_pOwner{};
};