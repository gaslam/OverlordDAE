#pragma once
class UberMaterial final :
    public Material<UberMaterial>
{
public:
    UberMaterial();
    ~UberMaterial() override = default;

    UberMaterial(const UberMaterial& other) = delete;
    UberMaterial(UberMaterial&& other) noexcept = delete;
    UberMaterial& operator=(const UberMaterial& other) = delete;
    UberMaterial& operator=(UberMaterial&& other) noexcept = delete;

    void SetDiffuseTexture(const std::wstring& assetFile);
    void SetSpecularLevelTexture(const std::wstring& assetFile);
    void SetNormalTexture(const std::wstring& assetFile);
    void SetEnvironmentMapTexture(const std::wstring& assetFile);
    void SetOpacityMapTexture(const std::wstring& assetFile);

    void SetUseDiffuseTexture(bool value);
    void SetFlipGreenChannel(bool value);
    void SetUseFresnelFalloff(bool value);
    void SetUseEnvMapping(bool value);
    void SetUseTextureOpacity(bool value);
    void SetUseTextureNormal(bool value);
    void SetUseSpecularBlinn(bool value);
    void SetUseSpecularPhong(bool value);
    void SetShininess(int value);
    void SetAmbientIntensity(float value);
    void SetReflectionStrength(float value);
    void SetRefractionStrength(float value);
    void SetFresnelPower(float value);
    void SetFresnelMultiplier(float value);
    void SetFresnelHardness(float value);
    void SetRefractionIndex(float index);
    void SetOpacityIntensity(float index);
    void SetColorDiffuse(XMFLOAT4& color);
    void SetColorSpecular(XMFLOAT4& color);
    void SetColorAmbient(XMFLOAT4& color);
    void SetColorFresnel(XMFLOAT4& color);

protected:
    void InitializeEffectVariables() override;
private:
    bool m_UseDiffuseTexture{ false };
    bool m_FlipGreenChannel{ false };
    bool m_UseTextureNormal{ false };
    bool m_UseEnvMapping{ false };
    bool m_UseFresnelFalloff{ false };
    bool m_UseTextureOpacityIntensity{ false };
    bool m_UseSpecularBlinn{ false };
    bool m_UseSpecularPhong{ false };

    int m_Shininess{ 0 };
    float m_AmbientIntensity{ 0.f };
    float m_ReflectionStrength{ 0.f };
    float m_RefractionStrength{ 0.f };
    float m_RefractionIndex{ 0.3f };
    float m_FresnelPower{};
    float m_FresnelMultiplier{};
    float m_FresnelHardness{};
    float m_OpacityIntensity{};

    TextureData* m_pDiffuseTexture{ nullptr };
    TextureData* m_pSpecularLvlTexture{ nullptr };
    TextureData* m_pNormalTexture{ nullptr };
    TextureData* m_pEnvironmentMapTexture{ nullptr };
    TextureData* m_pOpacityMapTexture{ nullptr };

    XMFLOAT4 m_ColorDiffuse{ 1,1,1,1 };
    XMFLOAT4 m_ColorSpecular{ 1,1,1,1 };
    XMFLOAT4 m_ColorAmbient{ 1,1,1,1 };
    XMFLOAT4 m_ColorFresnel{ 1,1,1,1 };
};

