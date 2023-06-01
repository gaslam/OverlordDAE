#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	delete[] m_ParticlesArray;

	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
	}
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
	CreateVertexBuffer(sceneContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	if(m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
	}

	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = m_ParticleCount * sizeof(VertexParticle);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	const HRESULT result = sceneContext.d3dContext.pDevice->CreateBuffer(&desc,nullptr , &m_pVertexBuffer);

	HANDLE_ERROR(result);
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	const float particleInterval{(m_EmitterSettings.maxEnergy + m_EmitterSettings.minEnergy) / 2.f / m_ParticleCount};
	const float elapsed{ sceneContext.pGameTime->GetElapsed() };
	m_LastParticleSpawn += elapsed;
	m_ActiveParticles = 0;
	D3D11_MAPPED_SUBRESOURCE pResource;
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pResource);

	VertexParticle* pBuffer = reinterpret_cast<VertexParticle*>(pResource.pData);

	for(UINT i{}; i < m_ParticleCount; ++i)
	{
		auto& particle = m_ParticlesArray[i];
		if(particle.isActive)
		{
			UpdateParticle(particle, elapsed);
		}

		if(!particle.isActive && m_LastParticleSpawn >= particleInterval && !m_Disabled)
		{
			SpawnParticle(particle);
			m_LastParticleSpawn -= particleInterval;
		}

		if(particle.isActive)
		{
			pBuffer[m_ActiveParticles] = particle.vertexInfo;
			++m_ActiveParticles;
		}
	}

	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	if(!p.isActive)
	{
		return;
	}

	p.currentEnergy -= elapsedTime;
	if(p.currentEnergy < 0)
	{
		p.isActive = false;
	}

	const XMVECTOR velocity{ XMLoadFloat3(&m_EmitterSettings.velocity) };
	const XMVECTOR newPos{ XMLoadFloat3(&p.vertexInfo.Position) + velocity * elapsedTime };
	XMStoreFloat3(&p.vertexInfo.Position, newPos);

	const float lifePercent{ p.currentEnergy / p.totalEnergy };
	const float fadeOutDelay{ 2.f };
	p.vertexInfo.Color = m_EmitterSettings.color;
	p.vertexInfo.Color.w *= lifePercent;

	p.vertexInfo.Color.w = m_EmitterSettings.color.w * lifePercent * fadeOutDelay;
	p.vertexInfo.Size = p.initialSize * (1.f + (p.sizeChange - 1.f) * (1.f - lifePercent));


}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	p.isActive = true;

	p.currentEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.totalEnergy = p.currentEnergy;

	XMVECTOR randomDir{

		MathHelper::randF(-1.f,1.f),
		MathHelper::randF(-1.f,1.f),
		MathHelper::randF(-1.f,1.f) };


	const float distance{ MathHelper::randF(m_EmitterSettings.minEmitterRadius,m_EmitterSettings.maxEmitterRadius) };

	XMFLOAT3 randomPosFl{};
	const XMVECTOR randomPos{ XMLoadFloat3( &GetTransform()->GetWorldPosition()) + randomDir * distance};
	XMStoreFloat3(&randomPosFl, randomPos );
	p.vertexInfo.Position = randomPosFl;

	const float randomSize{ MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize) };
	p.vertexInfo.Size = randomSize;
	p.initialSize = randomSize;

	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);
	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture->GetShaderResourceView());

	const auto pTechniqueContext = m_pParticleMaterial->GetTechniqueContext();

	sceneContext.d3dContext.pDeviceContext->IASetInputLayout(pTechniqueContext.pInputLayout);
	sceneContext.d3dContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	constexpr UINT startSlot{ 0 },numBuffers{1},strideSize{sizeof(VertexParticle)},offset{0};
	sceneContext.d3dContext.pDeviceContext->IASetVertexBuffers(startSlot, numBuffers, &m_pVertexBuffer, &strideSize, &offset);


	const auto pTechnique = pTechniqueContext.pTechnique;
	D3DX11_TECHNIQUE_DESC techDesc{};
	pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		pTechnique->GetPassByIndex(p)->Apply(0, sceneContext.d3dContext.pDeviceContext);
		sceneContext.d3dContext.pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
		ImGui::Checkbox("Stop", &m_Disabled);
	}
}