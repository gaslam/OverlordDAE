#pragma once
#include "Misc/json.hpp"
#include "Prefabs/BallPrefab.h"

class CubePrefab;
class SpherePrefab;

using json = nlohmann::json;

class BallComponent :
    public BaseComponent
{
public:
	BallComponent() = default;
    virtual void Initialize(const SceneContext&) override;
    void Update(const SceneContext&) override;
private:
    float m_TimeInterval{20.f};
    float m_TimePassed{};
    struct DirPusher
    {
        GameObject* pCube;
        XMFLOAT3 dir;
        float force;
    };
    std::vector<std::unique_ptr<BallPrefab>> m_pSpheres{};
    std::vector<std::unique_ptr<DirPusher>> m_pDirGuides{};
    float m_MaxSpeed{};
    float m_Radius{};

    XMFLOAT4 m_Color{ };
    XMFLOAT3 m_BallDir{ };
    XMFLOAT3 m_BallPos{ };

    PxMaterial* m_pBallMaterial{};

    void PushBalls(float elapsedTime);
    void SpawnBall();
    void SpawnPushers(json json);
    void DeleteBalls();
};

