#pragma once
#include "D:\DAE\Year_2\Graphics_Programming\OverlordEngine_x64_Start\OverlordEngine\Misc\Material.h"
class SkyboxMaterial :
    public Material<SkyboxMaterial>
{
public:
    SkyboxMaterial();
    ~SkyboxMaterial() override = default;

    SkyboxMaterial(const SkyboxMaterial& other) = delete;
    SkyboxMaterial(SkyboxMaterial&& other) noexcept = delete;
    SkyboxMaterial& operator=(const SkyboxMaterial& other) = delete;
    SkyboxMaterial& operator=(SkyboxMaterial&& other) noexcept = delete;

    void SetCubeMapTexture(const std::wstring& assetFile);
    void InitializeEffectVariables() override;

private:
    TextureData* m_pSkyboxTexture{};
};

