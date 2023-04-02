#pragma once
class ThirdPersonCamera;
class PlayableCharacter final :
    public GameObject
{
public:
    PlayableCharacter();
    void SetAnimator(ModelAnimator* pAnimator);
    void SetScale(float scale) { m_Scale = scale; }
    ThirdPersonCamera* GetCamera() const { return m_pCamera; }
protected:
    void Initialize(const SceneContext& sceneContext) override;
    void Update(const SceneContext& sceneContext) override;
private:
    ModelComponent* m_pModelComponent{};
    ThirdPersonCamera* m_pCamera{};
    float m_Scale{0.5f};
    float m_RunningSpeed{XMConvertToRadians(15.f)};
    float m_TargetFowardRotationYaw{};
    float m_TotalYaw{};
    float m_AngularSpeed{ 300.f };
    float m_CharacterSpeed{ 20.f };

    enum CharacterState : int {
        Idle,
        Running,
        Jumping
    };
    
    enum Actions: int{
    moveForward,
    moveBackwards,
    moveLeft,
    moveRight
    };

    CharacterState m_State{};

    void HandleButtons(float deltaTime, float& targetAngle, InputManager* pInput, TransformComponent* pCharacterTransform, bool& pressed);
    void HandleMovement(const SceneContext& sceneContext, float deltaTime);
    void HandleRotation(float deltaTime, float& targetAngle, TransformComponent* pCharacterTransform, bool& isMoving);
    void ChangeState(CharacterState& state);
};

