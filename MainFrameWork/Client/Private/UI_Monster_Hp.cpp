#include "stdafx.h"
#include "UI_Monster_Hp.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "TextBox.h"
#include "ServerSessionManager.h"

CUI_Monster_Hp::CUI_Monster_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Monster_Hp::CUI_Monster_Hp(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Monster_Hp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Monster_Hp::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("NamePlate");

	m_fSizeX = 504;
	m_fSizeY = 335;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = (g_iWinSizeY * 0.5f) + 100.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (nullptr != pArg)
	{
		m_pOwner = static_cast<CGameObject*>(pArg);
		if (FAILED(Initialize_StageName(CServerSessionManager::GetInstance()->Get_NickName())))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Monster_Hp::Initialize_StageName(const wstring& strName)
{
	m_szFont = L"³Ø½¼Lv1°íµñ";
	m_strName = strName;
	Ready_TextBox(m_strName);
	Set_Active(true);

	return S_OK;
}

void CUI_Monster_Hp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Update_NamePlatePos();
}

void CUI_Monster_Hp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Monster_Hp::Render()
{
	//if (FAILED(Bind_ShaderResources()))
	//	return E_FAIL;

	//m_pShaderCom->Begin(0);

	//m_pVIBufferCom->Render();
	Print_InGame_Name();

	return S_OK;
}

HRESULT CUI_Monster_Hp::Ready_Components()
{
	__super::Ready_Components();

	return S_OK;
}

HRESULT CUI_Monster_Hp::Bind_ShaderResources()
{
	return S_OK;
}


void CUI_Monster_Hp::Start_InGame_Name()
{
	m_strName.clear();
	m_pInGameNameWnd->Set_Active(true);
}

void CUI_Monster_Hp::End_InGame_Name()
{
	m_strName.clear();
	m_pInGameNameWnd->Set_Active(false);
}

void CUI_Monster_Hp::Update_NamePlatePos()
{
	if (nullptr != m_pOwner)
	{
		Vec3 vHostPos = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

		vHostPos.y += 1.7f;

		Matrix ViewMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
		Matrix ProjMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);

		vHostPos = XMVector3TransformCoord(vHostPos, ViewMatrix);
		vHostPos = XMVector3TransformCoord(vHostPos, ProjMatrix);

		//vHostPos.x *= g_iWinSizeX * 0.5f;
		//vHostPos.y *= g_iWinSizeY * 0.5f;

		Vec3 vScale = m_pTransformCom->Get_Scale();

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(vHostPos.x, vHostPos.y + (vScale.y * 0.35f), 0.1f));

		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		m_pInGameNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(vPos.x, vPos.y, 0.0f));
	}
}

void CUI_Monster_Hp::Print_InGame_Name()
{
	if (nullptr == m_pInGameNameWnd)
		return;

	m_pInGameNameWnd->Set_Active(true);
	m_pInGameNameWnd->Clear_Text();
	m_pInGameNameWnd->Set_Alpha(1.f);

	m_pInGameNameWnd->Get_TransformCom()->Set_Scale(Vec3(300.f, 40.f, 0.f));
	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, m_strName);
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pInGameNameWnd->Set_Text(m_strTag + TEXT("-1"), m_szFont, m_strName, Vec2(150.f - 1.f , 20.f), Vec2(0.35f, 0.35f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pInGameNameWnd->Set_Text(m_strTag + TEXT("-2"), m_szFont, m_strName, Vec2(150.f + 1.f, 20.f), Vec2(0.35f, 0.35f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pInGameNameWnd->Set_Text(m_strTag , m_szFont, m_strName, Vec2(150.f, 20.f), Vec2(0.35f, 0.35f), vOrigin, 0.f, Vec4(1.0f, 0.98f, 0.54f, 1.f));
	
}

void CUI_Monster_Hp::Set_Active(_bool bActive)
{
	//m_bActive = bActive;
	m_pInGameNameWnd->Set_Active(bActive);
}

HRESULT CUI_Monster_Hp::Ready_TextBox(const wstring& strName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pInGameNameWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("NamePlate") + strName;
		m_strTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(300.f, 40.0f);
		m_pInGameNameWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pInGameNameWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pInGameNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pInGameNameWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

CUI_Monster_Hp* CUI_Monster_Hp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Monster_Hp* pInstance = new CUI_Monster_Hp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Monster_Hp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_Hp::Clone(void* pArg)
{
	CUI_Monster_Hp* pInstance = new CUI_Monster_Hp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Monster_Hp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_Hp::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	m_pInGameNameWnd->Set_Dead(true);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
