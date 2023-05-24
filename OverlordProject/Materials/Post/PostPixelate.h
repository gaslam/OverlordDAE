#pragma once
class PostPixelate :
    public PostProcessingMaterial
{
public:
	explicit PostPixelate();
	~PostPixelate() override = default;
	PostPixelate(const PostPixelate& other) = delete;
	PostPixelate(PostPixelate&& other) noexcept = delete;
	PostPixelate& operator=(const PostPixelate& other) = delete;
	PostPixelate& operator=(PostPixelate&& other) noexcept = delete;
	void SetPixels(const XMFLOAT2& pixels);
protected:
	virtual void UpdateBaseEffectVariables(const SceneContext& sceneContext, RenderTarget* pSource) override;

	void Initialize(const GameContext& /*gameContext*/) override;

private:
	XMFLOAT2 m_Pixels{};
	ID3DX11EffectVectorVariable* m_pVector{};
};

