#pragma once
class NumberDisplayComponent;

struct CharacterDesc
{
	CharacterDesc(
		PxMaterial* pMaterial,
		float radius = .5f,
		float height = 2.f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
	}

	float maxMoveSpeed{ 15.f };
	float maxFallSpeed{ 15.f };

	float JumpSpeed{ 15.f };

	float moveAccelerationTime{ .3f };
	float fallAccelerationTime{ .3f };

	PxCapsuleControllerDesc controller{};

	float rotationSpeed{ 60.f };

	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Jump{ -1 };
};

class Character : public GameObject
{
public:
	Character(const CharacterDesc& characterDesc);
	~Character() override = default;

	Character(const Character& other) = delete;
	Character(Character&& other) noexcept = delete;
	Character& operator=(const Character& other) = delete;
	Character& operator=(Character&& other) noexcept = delete;

	void DrawImGui();
	void SetYaw(float yaw) { m_TotalYaw = yaw; }
	void SetPitch(float pitch) { m_TotalPitch = pitch; }
	void SetCharacterDesc(CharacterDesc& desc) { m_CharacterDesc = desc; }
	void SetAnimator(ModelAnimator* pAnimator) { m_pModelAnimator = pAnimator; }
	CharacterDesc GetCharacterDesc() const { return m_CharacterDesc; }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	CameraComponent* m_pCameraComponent{};
	ControllerComponent* m_pControllerComponent{};
	ModelAnimator* m_pModelAnimator{ nullptr };

	CharacterDesc m_CharacterDesc;
	float m_TotalPitch{}, m_TotalYaw{};				//Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveSpeed{};								//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)

	XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	XMFLOAT3 m_CurrentDirection{};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)

	enum InputIds : int {
		moveForward,
		moveBackwards,
		moveLeft,
		moveRight,
		jump
	};

	ModelComponent* m_pModelComponent{};
	FMOD::Channel* m_pChannel3D{};
	FMOD::Sound* m_pHooSound{};
	FMOD::Sound* m_pWooHooSound{};

	bool m_StoppedRunning{};

	enum CharacterState : int {
		Idle,
		Running,
		Attack,
		BeAttacked,
		Fall,
		Die,
		Jump
	};


	CharacterState m_State{};
	
	int m_MaxPossibleJumps = 2;
	int m_TimesJumped = 0;
};

