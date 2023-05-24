#pragma once
class SceneManager final: public Singleton<SceneManager>
{
public:

	void AddGameScene(GameScene* pScene);
	void RemoveGameScene(GameScene* pScene);
	void SetActiveGameScene(const std::wstring& sceneName);
	void NextScene();
	void PreviousScene();
	GameScene* GetActiveScene() const { return m_ActiveScene; }
	const SceneContext& GetActiveSceneContext() const { return m_ActiveScene->GetSceneContext(); }
	SceneSettings& GetActiveSceneSettings() const { return m_ActiveScene->GetSceneSettings(); }

protected:
	void Initialize() override;

private:
	friend class OverlordGame;
	friend class Singleton<SceneManager>;
	SceneManager() = default;

	void PostInitialize() const;
	void WindowStateChanged(int state, bool active) const;
	void Update();
	void Draw() const;
	void OnGUI() const;

	std::vector<std::unique_ptr<GameScene>> m_pScenes{};
	GameScene* m_ActiveScene{}, * m_NewActiveScene{};
};

