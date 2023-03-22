#pragma once
class BoneObject :
    public GameObject
{
public:
    BoneObject(BaseMaterial* pMaterial, float length = 5.f);
    ~BoneObject() override = default;
    BoneObject(const BoneObject& other) = delete;
    BoneObject(BoneObject&& other) noexcept = delete;
    BoneObject& operator=(const BoneObject& other) = delete;
    BoneObject& operator=(BoneObject&& other) noexcept = delete;

    void AddBone(BoneObject* pObject);

    const XMFLOAT4X4& GetBindPos() const { return m_BindPos; }
    void CalculateBindPos();
protected:
    void Initialize(const SceneContext&) override;

private:
    float m_Length{};
    BaseMaterial* m_pMaterial{ nullptr };
    XMFLOAT4X4 m_BindPos{};
};

