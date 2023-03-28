#include "stdafx.h"
#include "ModelAnimator.h"
#include <algorithm>

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter) :
	m_pMeshFilter{ pMeshFilter }
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	TODO_W6_();

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)

		const float elapsedSec = sceneContext.pGameTime->GetElapsed();
		const float durationInTicks = m_CurrentClip.duration;
		const float ticksPerSecond = m_CurrentClip.ticksPerSecond;

		float passedTicks = elapsedSec * ticksPerSecond * m_AnimationSpeed;
		if(passedTicks > durationInTicks)passedTicks = fmodf(passedTicks, durationInTicks);

		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount

		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0.f)
			{
				m_TickCount += durationInTicks;
			}
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > durationInTicks)
			{
				m_TickCount -= durationInTicks;
			}
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount

		auto it = std::max_element(m_CurrentClip.keys.begin(), m_CurrentClip.keys.end(), [&](const AnimationKey& key1, const AnimationKey& key2) {
			return key2.tick > key1.tick && key2.tick < m_TickCount;
			});
		keyA = *it;
		keyB = *(it + 1);
		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		//Clear the m_Transforms vector
		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector
		for (int i{}; i < m_pMeshFilter->m_BoneCount; ++i)
		{
			const auto& transformA = keyA.boneTransforms[i];
			const auto& transformB = keyB.boneTransforms[i];

			float blendFactor{ (m_TickCount - keyA.tick) / (m_CurrentClip.duration / (m_CurrentClip.keys.size() - 1)) };
			XMVECTOR scaleA;
			XMVECTOR rotQuatA;
			XMVECTOR transA;
			XMMatrixDecompose(&scaleA, &rotQuatA, &transA, XMLoadFloat4x4(&transformA));

			XMVECTOR scaleB;
			XMVECTOR rotQuatB;
			XMVECTOR transB;
			XMMatrixDecompose(&scaleB, &rotQuatB, &transB, XMLoadFloat4x4(&transformB));

			auto transLerp = XMVectorLerp(transA, transB, blendFactor);
			auto rotLerp = XMQuaternionSlerp(rotQuatA, rotQuatB, blendFactor);
			auto scaleLerp = XMVectorLerp(scaleA, scaleB, blendFactor);
			const XMMATRIX  matrix{
			XMMatrixScalingFromVector(scaleLerp) *
			XMMatrixRotationQuaternion(rotLerp) *
			XMMatrixTranslationFromVector(transLerp) };
			XMStoreFloat4x4(&m_Transforms[i], matrix);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
		//Set m_ClipSet to false
		//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
		//If found,
		//	Call SetAnimation(Animation Clip) with the found clip
		//Else
		//	Call Reset
		//	Log a warning with an appropriate message

		m_ClipSet = false;
	auto& animations = m_pMeshFilter->GetAnimationClips();
	const auto it = std::find_if(animations.begin(), animations.end(), [clipName](const AnimationClip& clip) {
		return clip.name == clipName;
		});
	if (it == animations.end())
	{
		Reset();
		Logger::LogWarning(L"Cannot find animation with name: ", clipName);
	}
	else
	{
		SetAnimation(*it);
	}
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
		//Set m_ClipSet to false
		//Check if clipNumber is smaller than the actual m_AnimationClips vector size
		//If not,
			//	Call Reset
			//	Log a warning with an appropriate message
			//	return
		//else
			//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
			//	Call SetAnimation(AnimationClip clip)
		if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
		{
			AnimationClip clip = m_pMeshFilter->m_AnimationClips[clipNumber];
			SetAnimation(clip);
		}
		else
		{
			Reset();
			Logger::LogWarning(L"Cannot find animation with clipnumber: ", clipNumber);
		}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	m_ClipSet = true;
	m_CurrentClip = clip;
	Reset(false);

}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false

	//Set m_TickCount to zero
	//Set m_AnimationSpeed to 1.0f

	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)

	if (pause)
	{
		m_IsPlaying = false;
	}
	else
	{
		m_IsPlaying = true;
	}

	m_TickCount = 0;
	m_AnimationSpeed = 1.0f;
	if (m_ClipSet)
	{
		auto key = m_CurrentClip.keys[0];
		auto transform = key.boneTransforms;
		m_Transforms.assign(transform.begin(), transform.end());
	}
	else
	{
		XMMATRIX identity = XMMatrixIdentity();
		XMFLOAT4X4 matrix;
		XMStoreFloat4x4(&matrix, identity);
		std::fill(m_Transforms.begin(), m_Transforms.end(), matrix);
	}
}
