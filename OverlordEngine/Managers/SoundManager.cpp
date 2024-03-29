#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	Initialize();
}

SoundManager::~SoundManager()
{
	if (m_pFmodSystem)
	{
#pragma warning(push)
#pragma warning(disable : 26812)
		m_pFmodSystem->release();
#pragma warning(pop)
	}
}

bool SoundManager::ErrorCheck(FMOD_RESULT res)
{
	if (res != FMOD_OK)
	{
		std::wstringstream strstr;
		strstr << L"FMOD error! \n[" << res << L"] " << FMOD_ErrorString(res) << std::endl;
		Logger::LogError(strstr.str());
		return false;
	}

	return true;
}

void SoundManager::Update()
{
	////UPDATE FMOD SYSTEM (3D sound requirement)
	//FMOD_RESULT result = m_pFmodSystem->update();
	//ErrorCheck(result);
}

void SoundManager::Initialize()
{
	FMOD::Debug_Initialize(FMOD_DEBUG_LEVEL_ERROR);

	unsigned int version{};
	int numdrivers{};

	/*
	Create a System object and initialize.
	*/
	HANDLE_ERROR(System_Create(&m_pFmodSystem));
	HANDLE_ERROR(m_pFmodSystem->getVersion(&version));
	ASSERT_IF(version < FMOD_VERSION, L"Initialization Failed!\n\nYou are using an old version of FMOD {:#x}. This program requires {:#x}\n",
		version, FMOD_VERSION)

	HANDLE_ERROR(m_pFmodSystem->getNumDrivers(&numdrivers));

	if (numdrivers == 0)
	{
		HANDLE_ERROR(m_pFmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND));
	}
	else
	{
		HANDLE_ERROR(m_pFmodSystem->init(32, FMOD_INIT_NORMAL, nullptr));
	}

}