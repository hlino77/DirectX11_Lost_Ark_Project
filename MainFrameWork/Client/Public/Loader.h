#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 1. 다음레벨ㄹ에 대한 로딩 작업을 술앨ㅇㅎ나다. */

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual~CLoader() = default;

public:
	const wstring Get_LoadingText() const {
		return m_strLoading;
	}

	_bool Get_Finished() const {
		return m_isFinished;
	}

public:
	/* eNextLevel에 대한 로딩을 하기위해 스레드를 생성한다. */
	HRESULT Initialize(LEVELID eNextLevel);
	_int	Loading();


private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	LEVELID					m_eNextLevel = { LEVEL_END };
	wstring					m_strLoading = { TEXT("") };
	_bool					m_isFinished = { false };

private:
	HANDLE					m_hThread = { 0 };
	CRITICAL_SECTION		m_Critical_Section;



private:
	HRESULT Loading_For_Level_Logo();
	HRESULT Loading_For_Level_Tool();
	HRESULT Loading_For_Level_ServerSelect();
	HRESULT Loading_For_Level_Lobby();
	HRESULT Loading_For_Level_Bern();

	
	HRESULT Load_MapData(LEVELID eLevel, const wstring& szFilePath);
	HRESULT Load_ColMesh(LEVELID eLevel, const wstring& szFilePath);
	HRESULT	Loading_IdentityTexture();

	HRESULT Loading_Model_For_Level_Bern();
	HRESULT Loading_Model_For_Level_Lobby();
	HRESULT	Loading_Skill_For_Level_Bern();
public:
	static CLoader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVELID eNextLevel);
	virtual void Free() override;
};

END 