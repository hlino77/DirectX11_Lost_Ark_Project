#include "stdafx.h"
#include "ToolBase.h"
//#include "ToolMediator.h"
#include "GameInstance.h"

CToolBase::CToolBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	m_pDevice->AddRef();
	m_pContext->AddRef();
}

HRESULT CToolBase::Initialize(void* pArg)
{
	m_pGameInstance = GET_INSTANCE(CGameInstance);

	return S_OK;
}

//void CToolBase::SetMediator(CToolBaseMediator* pMediator)
//{ 
//	m_pMediator = pMediator;
//	Safe_AddRef(m_pMediator);
//	//m_pMediator->AddRef();
//}

void CToolBase::s2cPushBack(vector<_char*>& vecChar, string str)
{
	const _char* szSrc = str.c_str();
	size_t len = strlen(szSrc) + 1;
	_char* szCopy = new _char[len];
	strcpy_s(szCopy, len, szSrc);
	vecChar.push_back(szCopy);
}

void CToolBase::s2cPushBack(vector<const _char*>& vecChar, string str)
{
	const _char* szSrc = str.c_str();
	size_t len = strlen(szSrc) + 1;
	_char* szCopy = new _char[len];
	strcpy_s(szCopy, len, szSrc);
	vecChar.push_back(szCopy);
}

void CToolBase::s2cPushBackRef(vector<_char*>& vecChar, string& str)
{
	const _char* szSrc = str.c_str();
	size_t len = strlen(szSrc) + 1;
	_char* szCopy = new _char[len];
	strcpy_s(szCopy, len, szSrc);
	vecChar.push_back(szCopy);
}

void CToolBase::s2cPushBackRef(vector<const _char*>& vecChar, string& str)
{
	const _char* szSrc = str.c_str();
	size_t len = strlen(szSrc) + 1;
	_char* szCopy = new _char[len];
	strcpy_s(szCopy, len, szSrc);
	vecChar.push_back(szCopy);
}

void CToolBase::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	//Safe_Release(m_pMediator);
	RELEASE_INSTANCE(CGameInstance);
}
