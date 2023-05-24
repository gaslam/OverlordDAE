#pragma once
#include <functional>

enum class PxTriggerAction
{
	ENTER,
	LEAVE
};

class GameObject
{
public:
	typedef std::function<void (GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)> PhysicsCallback;

	GameObject();
	virtual ~GameObject() = default;
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) noexcept = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) noexcept = delete;

	template<typename T>
	std::enable_if_t<std::is_base_of_v<GameObject, T>, T*>
	AddChild(T* pObject)
	{
		AddChild_(pObject);
		return pObject;
	}
	void RemoveChild(GameObject* obj);

	template<typename T>
	std::enable_if_t<std::is_base_of_v<BaseComponent, T>, T*>
	AddComponent(T* pComp)
	{
		AddComponent_(pComp);
		return pComp;
	}
	void RemoveComponent(BaseComponent* pComponent);
	void OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action) const;

	const std::wstring& GetTag() const { return m_Tag; }
	void SetTag(const std::wstring& tag) { m_Tag = tag; }

	TransformComponent* GetTransform() const { return m_pTransform; }

	GameScene* GetScene() const;
	GameObject* GetParent() const { return m_pParentObject; }

	void SetOnTriggerCallBack(PhysicsCallback callback);

#pragma region
	template <class T>
	bool HasComponent(bool searchChildren = false)
	{
		return GetComponent<T>(searchChildren) != nullptr;
	}

	template <class T>
	T* GetComponent(bool searchChildren = false)
	{
		const type_info& ti = typeid(T);
		for (const auto& component : m_pComponents)
		{
			if (component && typeid(*component.get()) == ti)
				return dynamic_cast<T*>(component.get());
		}

		if (searchChildren)
		{
			for (auto& child : m_pChildren)
			{
				if (child->GetComponent<T>(searchChildren) != nullptr)
					return child->GetComponent<T>(searchChildren);
			}
		}

		return nullptr;
	}

	template <class T>
	std::vector<T*> GetComponents(bool searchChildren = false)
	{
		const type_info& ti = typeid(T);
		std::vector<T*> components;

		for (auto component : m_pComponents)
		{
			if (component && typeid(*component) == ti)
				components.push_back(dynamic_cast<T*>(component));
		}

		if (searchChildren)
		{
			for (auto child : m_pChildren)
			{
				auto childComponents = child->GetComponents<T>(searchChildren);

				for (auto* childComp : childComponents)
					components.push_back(dynamic_cast<T*>(childComp));
			}
		}

		return components;
	}

	template <class T>
	T* GetChild()
	{
		const type_info& ti = typeid(T);
		for (const auto& child : m_pChildren)
		{
			if (child && typeid(*child) == ti)
				return dynamic_cast<T*>(child.get());
		}
		return nullptr;
	}

	template <class T>
	std::vector<T*> GetChildren()
	{
		const type_info& ti = typeid(T);
		std::vector<T*> children;

		for (const auto& child : m_pChildren)
		{
			if (child && typeid(*child) == ti)
				children.push_back(dynamic_cast<T*>(child.get()));
		}
		return children;
	}
#pragma endregion Template Methods

protected:
	virtual void Initialize(const SceneContext&) {};
	virtual void PostInitialize(const SceneContext& ) {}
	virtual void Draw(const SceneContext&) {}
	virtual void PostDraw(const SceneContext&) {}
	virtual void Update(const SceneContext&) {}
	virtual void OnParentAttach(GameObject* /*pParent*/) {}
	virtual void OnParentDetach(GameObject* /*pPreviousParent*/) {}
	virtual void OnSceneAttach(GameScene* /*pScene*/){}
	virtual void OnSceneDetach(GameScene* /*pScene*/){}

private:
	friend class GameScene; //Handles private interface

	void RootInitialize(const SceneContext& sceneContext);
	void RootPostInitialize(const SceneContext& sceneContext);
	void RootUpdate(const SceneContext& sceneContext);
	void RootDraw(const SceneContext& sceneContext);
	void RootPostDraw(const SceneContext& sceneContext); //TODO: collapse in single Draw with context
	void RootShadowMapDraw(const SceneContext& sceneContext) const; //TODO: collapse in single Draw with context
	void RootOnSceneAttach(GameScene* pScene);
	void RootOnSceneDetach(GameScene* pScene);

	void AddChild_(GameObject* pObject);
	void AddComponent_(BaseComponent* pComponent);

	std::vector<std::unique_ptr<GameObject>> m_pChildren{};
	std::vector<std::unique_ptr<BaseComponent>> m_pComponents{};

	bool m_IsInitialized{}, m_IsActive{};
	GameScene* m_pParentScene{};
	GameObject* m_pParentObject{};
	TransformComponent* m_pTransform{};
	PhysicsCallback m_OnTriggerCallback{};
	std::wstring m_Tag{};
};
