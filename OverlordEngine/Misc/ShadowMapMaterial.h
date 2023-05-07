#pragma once
class ShadowMapMaterial final: public Material<ShadowMapMaterial>
{
public:
	ShadowMapMaterial();
	~ShadowMapMaterial() override = default;
	ShadowMapMaterial(const ShadowMapMaterial& other) = delete;
	ShadowMapMaterial(ShadowMapMaterial&& other) noexcept = delete;
	ShadowMapMaterial& operator=(const ShadowMapMaterial& other) = delete;
	ShadowMapMaterial& operator=(ShadowMapMaterial&& other) noexcept = delete;

	void SetLightVP(const XMFLOAT4X4& lightViewProj);
	void SetWorld(const XMFLOAT4X4& worldMatrix);
	void SetBones(const float* pData, UINT count);

protected:
	void InitializeEffectVariables() override;
private:
	ID3DX11EffectMatrixVariable* m_pLightViewProj{nullptr};
	ID3DX11EffectMatrixVariable* m_pWorld{nullptr};
	ID3DX11EffectMatrixVariable* m_pBones{nullptr};
};

