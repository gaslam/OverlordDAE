#pragma once
#include <unordered_map>
class UberMaterial final :
    public Material<UberMaterial>
{
public:
    UberMaterial();
    ~UberMaterial() override = default;

    UberMaterial(const UberMaterial& other) = delete;
    UberMaterial(UberMaterial&& other) noexcept = delete;
    UberMaterial& operator=(const UberMaterial& other) = delete;
    UberMaterial& operator=(UberMaterial&& other) noexcept = delete;;

#pragma region SetterFunctions
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
    void SetUseOpacityIntensity(bool value);
    void SetUseSpecularIntensity(bool value);
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
    void SetTechniqueIdx(int idx);
#pragma endregion SetterFunctions


#pragma region GetterFunctions
    bool GetUseTextureDiffuse() const { return m_UseDiffuseTexture; }
    bool GetUseOpacityIntensity() const { return m_UseTextureOpacityIntensity; }
    bool GetUseSpecularIntensity() const { return m_UseSpecularIntensity; }
    int GetShininess() const { return m_Shininess; }
    float GetOpacityIntensity() const { return m_OpacityIntensity; }
    XMFLOAT4 GetDiffuseColor() const { return m_ColorDiffuse; }
    XMFLOAT4 GetSpecularColor() const { return m_ColorSpecular; }
    std::unordered_map<int, const char*> GetTechniqueDesciptions();
#pragma endregion GetterFunctions

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
    bool m_UseSpecularIntensity{ false };

    int m_Shininess{ 0 };
    int m_TechniqueIdx{};
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

