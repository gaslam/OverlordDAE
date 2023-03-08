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
	const float m_BallSpeed{ 5.5f };
	const float m_CubeSpeed{ 5.5f };
	const static int m_TotalCollObjsPerType{ 2 };
	SpherePrefab* m_pRedSphere{ nullptr };
	CubePrefab* m_pCubes[2]{ nullptr,nullptr };
	CubePrefab* m_pFloorAndRoof[2]{ nullptr,nullptr };
	CubePrefab* m_pWalls[2]{ nullptr,nullptr };
	XMFLOAT2 m_BallDir{ 1.f,1.f };

	float m_CubeMinHeight{};
	float m_CubeMaxHeight{};

	enum Direction : int {
		LeftRight,
		UpDown
	};

	enum MovementInputPlayerOne : int {
		p1Up,
		p1Down
	};

	enum MovementInputPlayerTwo : int {
		p2Up = 2,
		p2Down = 3
	};

	void ChangeDirection(const Direction& direction);
	void ResetBall();

	void InitSphere(PxMaterial* mat);
	void InitCubes(PxMaterial* mat);
	void InitGroundAndFloor(PxMaterial* mat);
	void InitWalls(PxMaterial* mat);
	void AddInput();
	void MoveCube(CubePrefab* prefab, float distance);
};

