#define IMGUI_DEFINE_MATH_OPERATORS
#include "stdafx.h"
#include "ToolBase.h"
//#include "ToolMediator.h"
#include "Level_Tool.h"
#include "GameInstance.h"
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_win32.h"
#include "ImGUI\imgui_impl_dx11.h"
#include "ImGUI\imgui_internal.h"

CToolBase::CToolBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	m_pDevice->AddRef();
	m_pContext->AddRef();
}

HRESULT CToolBase::Initialize(void* pArg)
{
	m_pLevel_Tool = reinterpret_cast<CLevel_Tool*>(pArg);

	m_pGameInstance = GET_INSTANCE(CGameInstance);

	return S_OK;
}

//void CToolBase::SetMediator(CToolBaseMediator* pMediator)
//{ 
//	m_pMediator = pMediator;
//	Safe_AddRef(m_pMediator);
//	//m_pMediator->AddRef();
//}

//const _char* CToolBase::s2c(string str)
//{
//	const _char* szSrc = str.c_str();
//	size_t len = strlen(szSrc) + 1;
//	_char* szCopy = new _char[len];
//	strcpy_s(szCopy, len, szSrc);
//	return szCopy;
//}

CLevel_Tool* CToolBase::GetLevelTool()
{
	return m_pLevel_Tool;
}

const Matrix& CToolBase::Get_MQ_Part_Matrix(_uint iIndex)
{
	if (0 == iIndex)
		return m_matMq_Part_R;
	else if (1 == iIndex)
		return m_matMq_Part_L;
}

void CToolBase::Set_MQ_Part_Matrix(_uint iIndex, Matrix matMq)
{
	if (0 == iIndex)
		m_matMq_Part_R = matMq;
	else if (1 == iIndex)
		m_matMq_Part_L = matMq;
}

const _char* CToolBase::s2c(const string& str)
{
	const _char* szSrc = str.c_str();
	size_t len = strlen(szSrc) + 1;
	_char* szCopy = new _char[len];
	strcpy_s(szCopy, len, szSrc);
	return szCopy;
}

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

_bool CToolBase::Is_MouseClickedGUI()
{
	ImGuiContext* Context = ImGui::GetCurrentContext();
	if (!Context)
		return false;

	for (int i = 0; i < Context->Windows.Size; i++)
	{
		ImGuiWindow* window = Context->Windows[i];
		if (window->Flags & ImGuiWindowFlags_NoMouseInputs)
			continue;

		if (window->Rect().Contains(Context->IO.MousePos))
			return true;
	}

	return false;
}

void CToolBase::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	//Safe_Release(m_pMediator);
	RELEASE_INSTANCE(CGameInstance);
}
