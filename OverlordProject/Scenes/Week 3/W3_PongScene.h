#pragma once
class SpherePrefab;
class CubePrefab;
class W3_PongScene :
    public GameScene
{
public:
	W3_PongScene();
	~W3_PongScene() override = default;

	W3_PongScene(const W3_PongScene& other) = delete;
	W3_PongScene(W3_PongScene&& other) noexcept = delete;
	W3_PongScene& operator=(const W3_PongScene& other) = delete;
	W3_PongScene& operator=(W3_PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
	float m_TotalTimeToRest{ 3.f };
	float m_AccTimeToRest{ 0.f };
	SpherePrefab* m_pRedSphere{ nullptr };
	const static int m_TotalCollObjsPerType{ 2 };
	CubePrefab* m_pCubes[2]{ nullptr,nullptr };
	CubePrefab* m_pFloorAndGround[2]{ nullptr,nullptr };
	CubePrefab* m_pWalls[2]{ nullptr,nullptr };
	const float m_BallSpeed{ 5.5f };
	XMFLOAT2 m_BallDir{ 1.f,1.f };
	enum Direction : int {
		LeftRight,
		UpDown
	};
	void ChangeDirection(const Direction& direction);
	void ResetBall();

	void InitSphere(PxMaterial* mat);
	void InitCubes(PxMaterial* mat);
	void InitGroundAndFloor(PxMaterial* mat);
	void InitWalls(PxMaterial* mat);
};

