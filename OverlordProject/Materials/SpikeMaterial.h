#pragma once
class SpikeMaterial :
    public Material<SpikeMaterial>
{
public:
    SpikeMaterial();
    ~SpikeMaterial() override = default;

    SpikeMaterial(const SpikeMaterial& other) = delete;
    SpikeMaterial(SpikeMaterial&& other) noexcept = delete;
    SpikeMaterial& operator=(const SpikeMaterial& other) = delete;
    SpikeMaterial& operator=(SpikeMaterial&& other) noexcept = delete;

    void SetDiffuseColor(const XMFLOAT4& color);
    void SetSpikeLength(float length);
protected:
    virtual void InitializeEffectVariables() override;
};

