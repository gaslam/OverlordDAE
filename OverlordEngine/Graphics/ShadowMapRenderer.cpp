#include "stdafx.h"
#include "ShadowMapRenderer.h"
#include "Misc/ShadowMapMaterial.h"

ShadowMapRenderer::~ShadowMapRenderer()
{
	SafeDelete(m_pShadowRenderTarget)
}

void ShadowMapRenderer::Initialize()
{
	TODO_W8(L"Implement Initialize")
	//1. Create a separate RenderTarget (see RenderTarget class), store in m_pShadowRenderTarget
	//	- RenderTargets are initialized with the RenderTarget::Create function, requiring a RENDERTARGET_DESC
	//	- Initialize the relevant fields of the RENDERTARGET_DESC (enableColorBuffer:false, enableDepthSRV:true, width & height)

		RENDERTARGET_DESC targetDesc {};
	targetDesc.enableColorBuffer = false;
	targetDesc.enableDepthSRV = true;
	targetDesc.width = 1280;
	targetDesc.height = 720;

	m_pShadowRenderTarget = new RenderTarget{ m_GameContext.d3dContext };
	m_pShadowRenderTarget->Create(targetDesc);

	//2. Create a new ShadowMapMaterial, this will be the material that 'generated' the ShadowMap, store in m_pShadowMapGenerator
	//	- The effect has two techniques, one for static meshes, and another for skinned meshes (both very similar, the skinned version also transforms the vertices based on a given set of boneTransforms)
	//	- We want to store the TechniqueContext (struct that contains information about the Technique & InputLayout required for rendering) for both techniques in the m_GeneratorTechniqueContexts array.
	//	- Use the ShadowGeneratorType enum to retrieve the correct TechniqueContext by ID, and also use that ID to store it inside the array (see BaseMaterial::GetTechniqueContext)
	m_pShadowMapGenerator = MaterialManager::Get()->CreateMaterial<ShadowMapMaterial>();
	const int staticGeneratorIdx = static_cast<int>(ShadowGeneratorType::Static);
	const int skinnedGeneratorIdx = static_cast<int>(ShadowGeneratorType::Skinned);
	m_GeneratorTechniqueContexts[staticGeneratorIdx] = m_pShadowMapGenerator->GetTechniqueContext(staticGeneratorIdx);
	m_GeneratorTechniqueContexts[skinnedGeneratorIdx] = m_pShadowMapGenerator->GetTechniqueContext(skinnedGeneratorIdx);
}

void ShadowMapRenderer::UpdateMeshFilter(const SceneContext& sceneContext, MeshFilter* pMeshFilter) const
{
	//Here we want to Update the MeshFilter of ModelComponents that need to be rendered to the ShadowMap
	//Updating the MeshFilter means that we want to create a corresponding VertexBuffer for our ShadowGenerator material

	//1. Figure out the correct ShadowGeneratorType (either Static, or Skinned) with information from the incoming MeshFilter
	//2. Retrieve the corresponding TechniqueContext from m_GeneratorTechniqueContexts array (Static/Skinned)
	//3. Build a corresponding VertexBuffer with data from the relevant TechniqueContext you retrieved in Step2 (MeshFilter::BuildVertexBuffer)

	ShadowGeneratorType type = pMeshFilter->HasAnimations() ? ShadowGeneratorType::Skinned : ShadowGeneratorType::Static;
	auto techniqueContext = m_GeneratorTechniqueContexts[static_cast<int>(type)];
	pMeshFilter->BuildVertexBuffer(sceneContext,techniqueContext.inputLayoutID,techniqueContext.inputLayoutSize,techniqueContext.pInputLayoutDescriptions);

}

void ShadowMapRenderer::Begin(const SceneContext& sceneContext)
{
	TODO_W8(L"Implement Begin")
	//This function is called once right before we start the Shadow Pass (= generating the ShadowMap)
	//This function is responsible for setting the pipeline into the correct state, meaning
	//	- Making sure the ShadowMap is unbound from the pipeline as a ShaderResource (SRV), so we can bind it as a RenderTarget (RTV)
	//	- Calculating the Light ViewProjection, and updating the relevant Shader variables
	//	- Binding the ShadowMap RenderTarget as Main Game RenderTarget (= Everything we render is rendered to this rendertarget)
	//	- Clear the current (which should be the ShadowMap RT) rendertarget

	//1. Making sure that the ShadowMap is unbound from the pipeline as ShaderResourceView (SRV) is important, because we cannot use the same resource as a ShaderResourceView (texture resource inside a shader) and a RenderTargetView (target everything is rendered too) at the same time. In case this happens, you'll see an error in the output of visual studio - warning you that a resource is still bound as a SRV and cannot be used as an RTV.
	//	-> Unbinding an SRV can be achieved using DeviceContext::PSSetShaderResource [I'll give you the implementation for free] - double check your output because depending on your usage of ShaderResources, the actual slot the ShadowMap is using can be different, but you'll see a warning pop-up with the correct slot ID in that case.
	constexpr ID3D11ShaderResourceView* const pSRV[] = { nullptr };
	sceneContext.d3dContext.pDeviceContext->PSSetShaderResources(1, 1, pSRV);


	//2. Calculate the Light ViewProjection and store in m_LightVP
	const float viewWidth = 100.f * sceneContext.aspectRatio;
	const float viewHeight = 100.f;
	const float nearZ = 0.1f;
	const float farZ = 500.f;

	auto lightViewProj = XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
	// - Use XMMatrixOrtographicLH to create Projection Matrix (constants used for the demo below - feel free to change)
	//		*viewWidth> 100.f * aspectRatio
	//		*viewHeight> 100.f
	//		*nearZ>0.1f
	//		*farZ>500.f
	const Light light = sceneContext.pLights->GetDirectionalLight();
	const auto lightDir = XMLoadFloat4(&light.direction);
	const auto lightPos = XMLoadFloat4(&light.position);
	const auto focusPosition = XMVectorAdd(lightDir, lightPos);
	auto lightView = XMMatrixLookAtLH(lightPos, focusPosition, { 0,1,0,0 });
	//- Use XMMatrixLookAtLH to create a View Matrix
	//		*eyePosition: Position of the Direction Light (SceneContext::pLights > Retrieve Directional Light)
	//		*focusPosition: Calculate using the Direction Light position and direction
	//- Use the Projection & View Matrix to calculate the ViewProjection of this Light, store in m_LightVP

	XMStoreFloat4x4(&m_LightVP, lightView * lightViewProj);

	//3. Update this matrix (m_LightVP) on the ShadowMapMaterial effect
	m_pShadowMapGenerator->SetLightVP(m_LightVP);

	//4. Set the Main Game RenderTarget to m_pShadowRenderTarget (OverlordGame::SetRenderTarget) - Hint: every Singleton object has access to the GameContext...
	m_GameContext.pGame->SetRenderTarget(m_pShadowRenderTarget);

	//5. Clear the ShadowMap rendertarget (RenderTarget::Clear)
	m_pShadowRenderTarget->Clear();
}

void ShadowMapRenderer::DrawMesh(const SceneContext& sceneContext, MeshFilter* pMeshFilter, const XMFLOAT4X4& meshWorld, const std::vector<XMFLOAT4X4>& meshBones)
{
	TODO_W8(L"Implement DrawMesh")
		//This function is called for every mesh that needs to be rendered on the shadowmap (= cast shadows)

		//1. Figure out the correct ShadowGeneratorType (Static or Skinned)
		ShadowGeneratorType type = pMeshFilter->HasAnimations() ? ShadowGeneratorType::Skinned : ShadowGeneratorType::Static;

	//2. Retrieve the correct TechniqueContext for m_GeneratorTechniqueContexts
	auto techniqueContext = m_GeneratorTechniqueContexts[static_cast<int>(type)];
	//3. Set the relevant variables on the ShadowMapMaterial
	//		- world of the mesh
	//		- if animated, the boneTransforms

	m_pShadowMapGenerator->SetWorld(meshWorld);

	if (pMeshFilter->HasAnimations())
	{
		m_pShadowMapGenerator->SetBones(reinterpret_cast<const float*>(meshBones.data()), static_cast<UINT>(meshBones.size()));
	}

	auto pDeviceContxt = sceneContext.d3dContext.pDeviceContext;

	pDeviceContxt->IASetInputLayout(techniqueContext.pInputLayout);
	pDeviceContxt->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto subMesh : pMeshFilter->GetMeshes())
	{
		const UINT offset = 0;
		const auto& vertexBufferData = pMeshFilter->GetVertexBufferData(techniqueContext.inputLayoutID, subMesh.id);
		pDeviceContxt->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride,
			&offset);

		pDeviceContxt->IASetIndexBuffer(subMesh.buffers.pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		D3DX11_TECHNIQUE_DESC desc;
		techniqueContext.pTechnique->GetDesc(&desc);
		for (UINT pass = 0; pass < desc.Passes; ++pass)
		{
			techniqueContext.pTechnique->GetPassByIndex(pass)->Apply(0, pDeviceContxt);
			pDeviceContxt->DrawIndexed(subMesh.indexCount, 0, 0);
		}
	}

	//4. Setup Pipeline for Drawing (Similar to ModelComponent::Draw, but for our ShadowMapMaterial)
	//	- Set InputLayout (see TechniqueContext)
	//	- Set PrimitiveTopology
	//	- Iterate the SubMeshes of the MeshFilter (see ModelComponent::Draw), for each SubMesh:
	//		- Set VertexBuffer
	//		- Set IndexBuffer
	//		- Set correct TechniqueContext on ShadowMapMaterial - use ShadowGeneratorType as ID (BaseMaterial::SetTechnique)
	//		- Perform Draw Call (same as usual, iterate Technique Passes, Apply, Draw - See ModelComponent::Draw for reference)
}

void ShadowMapRenderer::End(const SceneContext&) const
{
		//This function is called at the end of the Shadow-pass, all shadow-casting meshes should be drawn to the ShadowMap at this point.
		//Now we can reset the Main Game Rendertarget back to the original RenderTarget, this also Unbinds the ShadowMapRenderTarget as RTV from the Pipeline, and can safely use it as a ShaderResourceView after this point.

		//1. Reset the Main Game RenderTarget back to default (OverlordGame::SetRenderTarget)
		//		- Have a look inside the function, there is a easy way to do this...

		m_GameContext.pGame->SetRenderTarget(nullptr);
		
}

ID3D11ShaderResourceView* ShadowMapRenderer::GetShadowMap() const
{
	return m_pShadowRenderTarget->GetDepthShaderResourceView();
}

void ShadowMapRenderer::Debug_DrawDepthSRV(const XMFLOAT2& position, const XMFLOAT2& scale, const XMFLOAT2& pivot) const
{
	if (m_pShadowRenderTarget->HasDepthSRV())
	{
		SpriteRenderer::Get()->DrawImmediate(m_GameContext.d3dContext, m_pShadowRenderTarget->GetDepthShaderResourceView(), position, XMFLOAT4{ Colors::White }, pivot, scale);

		//Remove from Pipeline
		constexpr ID3D11ShaderResourceView* const pSRV[] = { nullptr };
		m_GameContext.d3dContext.pDeviceContext->PSSetShaderResources(0, 1, pSRV);
	}
}
