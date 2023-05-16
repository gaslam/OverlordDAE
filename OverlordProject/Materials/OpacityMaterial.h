#pragma once
class OpacityMaterial final : public Material<OpacityMaterial>
{
public:
    OpacityMaterial();
    ~OpacityMaterial() override = default;

    OpacityMaterial(const OpacityMaterial& other) = delete;
    OpacityMaterial(OpacityMaterial&& other) noexcept = delete;
    OpacityMaterial& operator=(const OpacityMaterial& other) = delete;
    OpacityMaterial& operator=(OpacityMaterial&& other) noexcept = delete;

    void SetDiffuseTexture(const std::wstring& assetFile) override;
    void SetOpacityTexture(const std::wstring& assetFile);
    void SetUseOpacityMap(const bool canUseOpacity);

protected:
    void InitializeEffectVariables() override;
private:
    TextureData* m_pDiffuseTexture{ nullptr };
    TextureData* m_pOpacityTexture{ nullptr };
    bool m_UseOpacityMap{ false };
};

