#include "..\Public\GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Component_Manager.h"
#include "Light_Manager.h"
#include "Key_Manager.h"
#include "Utils.h"
#include "Text_Manager.h"
#include "NavigationMgr.h"
#include "EventMgr.h"
#include "PhysXMgr.h"
#include "QuadTreeMgr.h"
#include "Sound_Manager.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pUtilities(CUtils::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pKey_Manager(CKey_Manager::GetInstance())
	, m_pText_Manager(CText_Manager::GetInstance())
	, m_pNavigationMgr(CNavigationMgr::GetInstance())
	, m_pEventMgr(CEventMgr::GetInstance())
	, m_pPhysXMgr(CPhysXMgr::GetInstance())
	, m_pQuadTreeMgr(CQuadTreeMgr::GetInstance())
	, m_pSoundMgr(CSound_Manager::GetInstance())
{
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pKey_Manager);
	Safe_AddRef(m_pText_Manager);
	Safe_AddRef(m_pNavigationMgr);
	Safe_AddRef(m_pEventMgr);
	Safe_AddRef(m_pQuadTreeMgr);

	Safe_AddRef(m_pUtilities);
	Safe_AddRef(m_pPhysXMgr);
	Safe_AddRef(m_pSoundMgr);


}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, _uint iNumLayerType, 
	const GRAPHIC_DESC& GraphicDesc, 
	_Inout_ ID3D11Device** ppDevice, 
	_Inout_ ID3D11DeviceContext** ppContext, 
	_In_ HWND hWnd,
	_In_ HINSTANCE hInst)
{
	/* 그래픽디바이스 초기화 처리. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, ppDevice, ppContext)))
		return E_FAIL;

	/* 사운드디바이스 초기화 처리. */
	/* 입력디바이스 초기화 처리. */

	/* 오브젝트 매니져의 예약 처리. */
	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels, iNumLayerType)))
		return E_FAIL;

	/* 컴포넌트 매니져의 예약 처리. */
	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	if (FAILED(m_pKey_Manager->Reserve_Manager(*ppDevice, *ppContext, hWnd)))
		return E_FAIL;

	if (FAILED(m_pInput_Device->Initialize(hInst, hWnd)))
		return E_FAIL;


	if (FAILED(m_pText_Manager->Reserve_Manager(*ppDevice, *ppContext)))
		return E_FAIL;

	if (FAILED(m_pQuadTreeMgr->Reserve_Manager(*ppDevice, *ppContext)))
		return E_FAIL;

	if (FAILED(m_pSoundMgr->Ready_Sound()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGameInstance::Initialize_Engine_Server(_uint iNumLevels, _uint iNumLayerType)
{
	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels, iNumLayerType)))
		return E_FAIL;

	/* 컴포넌트 매니져의 예약 처리. */
	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;


	return S_OK;
}

void CGameInstance::Tick(_float fTimeDelta)
{
	m_pInput_Device->Update();
	m_pKey_Manager->Tick(fTimeDelta);
	m_pEventMgr->Tick(fTimeDelta);
	m_pObject_Manager->Tick(fTimeDelta);
	m_pLevel_Manager->Tick(fTimeDelta);
	m_pPipeLine->Update();


	m_pEventMgr->LateTick(fTimeDelta);
	m_pQuadTreeMgr->Tick(fTimeDelta);

	m_pObject_Manager->LateTick(fTimeDelta);
	m_pPhysXMgr->LateTick(fTimeDelta);
	m_pLevel_Manager->LateTick(fTimeDelta);
}

void CGameInstance::Tick_Server(_float fTimeDelta)
{
	m_pEventMgr->Tick(fTimeDelta);
	m_pObject_Manager->Tick(fTimeDelta);
	m_pLevel_Manager->Tick(fTimeDelta);


	m_pEventMgr->LateTick(fTimeDelta);
	m_pObject_Manager->LateTick(fTimeDelta);
	m_pLevel_Manager->LateTick(fTimeDelta);
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return;

	m_pObject_Manager->Clear(iLevelIndex);
	// m_pComponent_Manager->Clear(iLevelIndex);
}

_float CGameInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);	
}

HRESULT CGameInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

HRESULT CGameInstance::Clear_BackBuffer_View(Vec4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{

	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	return m_pGraphic_Device->Present();
}

ID3D11Device* CGameInstance::Get_Device()
{
	return m_pGraphic_Device->Get_Device();
}

ID3D11DeviceContext* CGameInstance::Get_Context()
{
	return m_pGraphic_Device->Get_Context();
}

_char CGameInstance::Get_DIKState(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKState(eKeyID);
}

_char CGameInstance::Get_DIMKeyState(DIMK eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMKeyState(eMouseKeyID);
}

_long CGameInstance::Get_DIMMoveState(DIMM eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMMoveState(eMouseMoveID);
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;
	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

HRESULT CGameInstance::Render_Debug()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Render_Debug();
}

_uint CGameInstance::Get_CurrLevelIndex()
{
	return m_pLevel_Manager->Get_CurrLevelIndex();
}

void CGameInstance::Set_Loading(_bool bLoading)
{
	m_pLevel_Manager->Set_Loading(bLoading);
}

void CGameInstance::Set_LoadingNext(_uint iNextLevel)
{
	m_pLevel_Manager->Set_LoadingNext(iNextLevel);
}

CLevel* CGameInstance::Get_CurrLevel()
{
	return m_pLevel_Manager->Get_CurrLevel();
}

HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);	
}

CGameObject* CGameInstance::Add_GameObject(_uint iLevelIndex, _uint iNumLayerType, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

		return m_pObject_Manager->Add_GameObject(iLevelIndex, iNumLayerType, strPrototypeTag, pArg);
}

CGameObject* CGameInstance::Clone_GameObject(const wstring& strPrototypeTag, void* pArg)
{
	return m_pObject_Manager->Clone_GameObject(strPrototypeTag, pArg);
}

CGameObject* CGameInstance::Find_GameObejct(_uint iLevelIndex, const _uint iLayerType, const wstring& strObjectTag)
{
	return m_pObject_Manager->Find_GameObejct(iLevelIndex, iLayerType, strObjectTag);
}

CGameObject* CGameInstance::Find_GameObejct(_uint iLevelIndex, const _uint iLayerType, _int iObjectID)
{
	return m_pObject_Manager->Find_GameObejct(iLevelIndex, iLayerType, iObjectID);
}

CGameObject* CGameInstance::Find_NearGameObject(_uint iLevelIndex, const _uint iLayerType, CGameObject* pObject)
{
	return m_pObject_Manager->Find_NearGameObject(iLevelIndex, iLayerType, pObject);
}

list<CGameObject*>& CGameInstance::Find_GameObjects(_uint iLevelIndex, const _uint iLayerType)
{
	return m_pObject_Manager->Find_GameObjects(iLevelIndex, iLayerType);
}

HRESULT CGameInstance::Delete_GameObject(_uint iLevelIndex, const _uint iLayerType, const CGameObject* pGameObject)
{
	return m_pObject_Manager->Delete_GameObject(iLevelIndex, iLayerType, pGameObject);
}


string CGameInstance::wstring_to_string(const wstring& strW)
{
	return m_pUtilities->wstring_to_string(strW);
}

wstring CGameInstance::string_to_wstring(const string& strS)
{
	return m_pUtilities->string_to_wstring(strS);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring& strProtoTypeTag, CComponent* pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strProtoTypeTag, pPrototype);
}

CComponent* CGameInstance::Clone_Component(CGameObject* pObject, _uint iLevelIndex, const wstring& strProtoTypeTag, void* pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(pObject, iLevelIndex, strProtoTypeTag, pArg);
}

HRESULT CGameInstance::Check_Prototype(_uint iLevelIndex, const wstring& strProtoTypeTag)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;
	return m_pComponent_Manager->Check_Prototype(iLevelIndex, strProtoTypeTag);
}

const LIGHTDESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr;

	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc);
}

HRESULT CGameInstance::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc, CTexture* pTexture)
{
	return m_pLight_Manager->Add_Light(pDevice, pContext, LightDesc, pTexture);
}

void CGameInstance::Ready_LightMatrix(Vec3 vOffset, Vec3 vLook)
{
	m_pLight_Manager->Ready_LightMatrix(vOffset, vLook);
}

void CGameInstance::Ready_StaticLightMatrix(Vec3 vPos, Vec3 vLook)
{
	m_pLight_Manager->Ready_StaticLightMatrix(vPos, vLook);
}


Matrix CGameInstance::Get_DirectionLightMatrix()
{
	return m_pLight_Manager->Get_DirectionLightMatrix();
}

Matrix CGameInstance::Get_StaticLightMatrix()
{
	return m_pLight_Manager->Get_StaticLightMatrix();
}

void CGameInstance::Update_LightMatrix(Vec3 vPos)
{
	m_pLight_Manager->Update_LightMatrix(vPos);
}



HRESULT CGameInstance::Reset_Lights()
{
	return m_pLight_Manager->Reset_Lights();
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eTransformState, Matrix TransformMatrix)
{
	if (nullptr == m_pPipeLine)
		return;

	m_pPipeLine->Set_Transform(eTransformState, TransformMatrix);
}

Matrix CGameInstance::Get_TransformMatrix(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return XMMatrixIdentity();

	return m_pPipeLine->Get_TransformMatrix(eTransformState);
}


Matrix CGameInstance::Get_TransformFloat4x4_TP(CPipeLine::TRANSFORMSTATE eTransformState) const
{
	if (nullptr == m_pPipeLine)
		return Matrix();

	return m_pPipeLine->Get_TransformFloat4x4_TP(eTransformState);
}

Vec4 CGameInstance::Get_CamPosition()
{
	if (nullptr == m_pPipeLine)
		return Vec4();

	return m_pPipeLine->Get_CamPosition();
}

const BoundingFrustum& CGameInstance::Get_CamFrustum()
{
	return m_pPipeLine->Get_CamFrustum();
}

KEY_STATE CGameInstance::GetKeyState(KEY _eKey)
{
	if (nullptr == m_pKey_Manager)
		return KEY_STATE::NONE;

	return m_pKey_Manager->GetKeyState(_eKey);
}

const POINT& CGameInstance::GetMousePos()
{

	return POINT();
}

HRESULT CGameInstance::Make_QaudTree(Vec3 vPos, Vec3 vScale, _uint iMaxDepth)
{
	return m_pQuadTreeMgr->Make_QaudTree(vPos, vScale, iMaxDepth);
}

HRESULT CGameInstance::Reset_QaudTree()
{
	return m_pQuadTreeMgr->Reset_QaudTree();
}

_bool CGameInstance::Add_Object_To_QuadTree(CSphereCollider* pCollider)
{
	return m_pQuadTreeMgr->Add_Object(pCollider);
}

void CGameInstance::AddFont(const wstring& szTextName, const wstring& szFontPath)
{
	m_pText_Manager->AddFont(szTextName, szFontPath);
}

void CGameInstance::DrawFont(const wstring& szTextName, const wstring& szString, const Vec2& vPosition, const Vec4& vColor, const _float& fRotation, const Vec2& vOrigin, const Vec2& vScale)
{
	m_pText_Manager->DrawFont(szTextName, szString, vPosition, vColor, fRotation, vOrigin, vScale);
}

Vec2 CGameInstance::MeasureString(const wstring& szTextName, const wstring& szString)
{
	return m_pText_Manager->MeasureString(szTextName, szString);
}

void CGameInstance::InputText(wstring& szInputText)
{
	m_pText_Manager->InputText(szInputText);
}

HRESULT CGameInstance::PlaySoundFile(const wstring& strSoundKey, _uint iChannel, _float fVolume)
{
	return m_pSoundMgr->PlaySoundFile(strSoundKey, iChannel, fVolume);
}

HRESULT CGameInstance::PlayBGM(const wstring& strSoundKey, _uint iChannel, _float fVolume)
{
	return m_pSoundMgr->PlayBGM(strSoundKey, iChannel, fVolume);
}

HRESULT CGameInstance::StopSound(_uint iChannel)
{
	return m_pSoundMgr->StopSound(iChannel);
}

HRESULT CGameInstance::StopSoundAll()
{
	return m_pSoundMgr->StopSoundAll();
}

HRESULT CGameInstance::SetChannelVolume(_uint iChannel, _float fVolume)
{
	return m_pSoundMgr->SetChannelVolume(iChannel, fVolume);
}

HRESULT CGameInstance::CheckPlaySoundFile(const wstring& strSoundKey, _uint iChannel, _float fVolume)
{
	return m_pSoundMgr->CheckPlaySoundFile(strSoundKey, iChannel, fVolume);
}

HRESULT CGameInstance::Initialize_LoopChannel(_uint iStart, _uint iEnd)
{
	return m_pSoundMgr->Initialize_LoopChannel(iStart, iEnd);
}

HRESULT CGameInstance::PlaySound_Distance(const wstring& strSoundKey, _uint iChannel, _float fVolume, Vec3 vPos, _float fRange)
{
	return m_pSoundMgr->PlaySound_Distance(strSoundKey, iChannel, fVolume, vPos, fRange);
}

HRESULT CGameInstance::PlaySoundFile_LoopChannel(const wstring& strSoundKey, _float fVolume)
{
	return m_pSoundMgr->PlaySoundFile_LoopChannel(strSoundKey, fVolume);
}

HRESULT CGameInstance::PlaySound_Distance_LoopChannel(const wstring& strSoundKey, _float fVolume, Vec3 vPos, _float fRange)
{
	return m_pSoundMgr->PlaySound_Distance_LoopChannel(strSoundKey, fVolume, vPos, fRange);
}

HRESULT CGameInstance::Find_Stop_Sound(const wstring& strSoundKey)
{
	return m_pSoundMgr->Find_Stop_Sound(strSoundKey);
}

HRESULT CGameInstance::Add_SoundTrack(const wstring& strSoundTrack, const wstring& strSoundKey)
{
	return m_pSoundMgr->Add_SoundTrack(strSoundTrack, strSoundKey);
}

const wstring& CGameInstance::Get_RandomSoundKey(const wstring& strSoundTrack)
{
	return m_pSoundMgr->Get_RandomSoundKey(strSoundTrack);
}



void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->DestroyInstance();
	CLevel_Manager::GetInstance()->DestroyInstance();
	CObject_Manager::GetInstance()->DestroyInstance();
	CComponent_Manager::GetInstance()->DestroyInstance();
	CTimer_Manager::GetInstance()->DestroyInstance();
	CPipeLine::GetInstance()->DestroyInstance();
	CInput_Device::GetInstance()->DestroyInstance();
	CGraphic_Device::GetInstance()->DestroyInstance();
	CLight_Manager::GetInstance()->DestroyInstance();
	CQuadTreeMgr::GetInstance()->DestroyInstance();

	CKey_Manager::GetInstance()->DestroyInstance();
	CUtils::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pQuadTreeMgr);

	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pUtilities);
}
