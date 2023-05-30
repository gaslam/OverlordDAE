#include "stdafx.h"
#include "BallComponent.h"

#include "Prefabs/SpherePrefab.h"

void BallComponent::Initialize(const SceneContext&)
{
	auto pParent = GetGameObject();
	if (pParent == nullptr)
	{
		Logger::LogWarning(L"pParent is null!! Cannot spawn rolling balls!!");
	}
	m_pBallMaterial = PxGetPhysics().createMaterial(0.5f, 0.5f, 0.5f);

	json data = *ContentManager::Load<json>(L"GameData/BallSpawner.json");
	json colorJson = data.at("color");
	json dirGuidersJson = data.at("dirGuiders");
	json dirJson = data.at("moveDir");
	json spawnPosJson = data.at("spawnPos");
	json despawnJson = data.at("despawner");
	m_Color = XMFLOAT4{ colorJson["r"], colorJson["g"],colorJson["b"],colorJson["a"] };
	m_BallDir = XMFLOAT3{ dirJson["x"], dirJson["y"],dirJson["z"] };
	m_BallPos = XMFLOAT3{ spawnPosJson["x"], spawnPosJson["y"],spawnPosJson["z"] };
	m_MaxSpeed = data["speed"];
	m_Radius = data["radius"];
	m_BallPos.y += m_Radius / 2.f;

	SpawnBall();
	SpawnPushers(dirGuidersJson);

	const json despawnerPos{ despawnJson.at("centerPos") };
	const json despawnerRot{ despawnJson.at("rotDir") };
	const float despawnerWidth{ despawnJson["width"] };
	const float despawnerHeight{ despawnJson["height"] };
	const float despawnerDepth{ despawnJson["depth"] };

	const XMFLOAT3 despawnerPosFl{ despawnerPos["x"],despawnerPos["y"],despawnerPos["z"] };
	XMFLOAT3 despawnerRotFl{ despawnerRot["x"],despawnerRot["y"],despawnerRot["z"] };

	GameObject* pDespawner = pParent->AddChild(new GameObject{});
	const auto pRigidBody = pDespawner->AddComponent(new RigidBodyComponent{ true });
	pRigidBody->AddCollider(PxBoxGeometry{ despawnerWidth,despawnerHeight,despawnerDepth }, *m_pBallMaterial, true);

	auto pTransform{ pDespawner->GetTransform() };

	pTransform->Rotate(despawnerRotFl, false);
	pTransform->Translate(despawnerPosFl);

	pDespawner->SetOnTriggerCallBack([this, pParent](GameObject*, GameObject* pOther, PxTriggerAction action)
		{
			if (typeid(*pOther) == typeid(SpherePrefab) && action == PxTriggerAction::ENTER)
			{
				auto it = std::find_if(m_pSpheres.begin(), m_pSpheres.end(), [pOther](BallPrefab* ball)
					{
						if (ball == pOther)
						{
							ball->SetMarkForDeletion(true);
							return true;
						}
						return false;
					});
			}
		});
}

void BallComponent::Update(const SceneContext& sceneContext)
{
	auto elapsed = sceneContext.pGameTime->GetElapsed();
	PushBalls(elapsed);
	m_TimePassed += elapsed;
	if (m_TimePassed >= m_TimeInterval)
	{
		SpawnBall();
		m_TimePassed = 0.f;
	}

	DeleteBalls();
}

void BallComponent::PushBalls(float elapsedTime)
{
	auto canContinue = std::any_of(m_pSpheres.begin(), m_pSpheres.end(), [](const BallPrefab* ball)
		{
			return ball->MustBePushed();
		});

	if (!canContinue)
	{
		return;
	}

	for (const auto& sphere : m_pSpheres)
	{
		if (sphere->MustBePushed())
		{
			auto pRigid = sphere->GetComponent<RigidBodyComponent>();
			auto toBePushed = sphere->GetDirToBePushed();
			XMVECTOR dir = XMLoadFloat3(&toBePushed) * elapsedTime;
			XMFLOAT3 dirFl;
			XMStoreFloat3(&dirFl, dir);
			pRigid->AddForce(dirFl, PxForceMode::eIMPULSE);
		}
	}
}

void BallComponent::SpawnBall()
{
	auto pParent = GetGameObject();
	if (pParent == nullptr)
	{
		Logger::LogWarning(L"pParent is null!! Cannot spawn rolling balls!!");
	}
	BallPrefab* pBall = pParent->AddChild(new BallPrefab{ m_Color,m_Radius });
	m_pSpheres.emplace_back(pBall);

	auto pRigid = pBall->AddComponent(new RigidBodyComponent());
	pRigid->GetPxRigidActor()->is<PxRigidDynamic>()->setMaxLinearVelocity(m_MaxSpeed);
	pRigid->AddCollider(PxSphereGeometry{ m_Radius }, *m_pBallMaterial);
	pBall->GetTransform()->Translate(m_BallPos);
	pRigid->SetDensity(2.f);
	const XMVECTOR moveDir = XMLoadFloat3(&m_BallPos) * m_MaxSpeed;
	XMFLOAT3 force{};
	XMStoreFloat3(&force, moveDir);
	pRigid->AddForce(force, PxForceMode::eIMPULSE);
}

void BallComponent::SpawnPushers(json data)
{
	auto pParent = GetGameObject();
	if (pParent == nullptr)
	{
		Logger::LogWarning(L"pParent is null!! Cannot spawn rolling balls!!");
	}
	for (auto guider : data)
	{
		json guideDir{ guider["dir"] };
		json guidePos{ guider["centerPos"] };
		const float width{ guider["width"] };
		const float height{ guider["height"] };
		const float depth{ guider["depth"] };
		const float force{ guider["force"] };
		const XMFLOAT3 pusherDir{ guideDir["x"], guideDir["y"],guideDir["z"] };
		const XMFLOAT3 pusherPos{ guidePos["x"], guidePos["y"],guidePos["z"] };

		DirPusher dirPusher{};
		dirPusher.pCube = pParent->AddChild(new GameObject());
		dirPusher.pCube->GetTransform()->Translate(pusherPos);
		dirPusher.dir = pusherDir;
		dirPusher.force = force;


		auto pRigidBody{ dirPusher.pCube->AddComponent(new RigidBodyComponent(true)) };
		pRigidBody->AddCollider(PxBoxGeometry{ width,height,depth }, *m_pBallMaterial, true);
		dirPusher.pCube->SetOnTriggerCallBack([dirPusher, this](GameObject*, GameObject* pOther, PxTriggerAction action)
			{
				if (typeid(*pOther) == typeid(BallPrefab) && action == PxTriggerAction::ENTER)
				{
					const auto currentSphere = reinterpret_cast<BallPrefab*>(pOther);
					for (const auto& sphere : m_pSpheres)
					{
						if (currentSphere == sphere)
						{
							sphere->SetMustBePushed(true);
							const XMVECTOR dir{ XMLoadFloat3(&dirPusher.dir) * dirPusher.force };
							XMFLOAT3 dirToPush{};
							XMStoreFloat3(&dirToPush, dir);
							sphere->SetDirToBePushed(dirToPush);
							Logger::LogInfo(L"Sphere entered!!");
						}
					}
				}


				if (typeid(*pOther) == typeid(BallPrefab) && action == PxTriggerAction::LEAVE)
				{
					auto currentSphere = reinterpret_cast<BallPrefab*>(pOther);
					for (const auto& sphere : m_pSpheres)
					{
						if (currentSphere == sphere)
						{
							sphere->SetMustBePushed(false);
							XMFLOAT3 noDir{};
							sphere->SetDirToBePushed(noDir);
						}
					}
				}
			});
	}
}

void BallComponent::DeleteBalls()
{
	auto it = std::find_if(m_pSpheres.begin(), m_pSpheres.end(), [this](BallPrefab* object)
		{
			if (object->IsMarkedAsDeleted())
			{
				GameObject* gameObject = GetGameObject();
				gameObject->RemoveChild(object);
				return true;
			}
			return false;
		});
	if (it == m_pSpheres.end())
	{
		return;
	}
	m_pSpheres.erase(it);
}
