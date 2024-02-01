#include "stdafx.h"
#include "UI_InGame_NamePlate.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "TextBox.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Npc.h"

CUI_InGame_NamePlate::CUI_InGame_NamePlate(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_InGame_NamePlate::CUI_InGame_NamePlate(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_InGame_NamePlate::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_InGame_NamePlate::Initialize(void* pArg)
{
	m_strUITag = TEXT("NamePlate");
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (nullptr != pArg)
	{
		m_pOwner = static_cast<CGameObject*>(pArg);
		if (nullptr != m_pOwner)
		{
			if (OBJ_TYPE::PLAYER == m_pOwner->Get_ObjectType())
			{
				if (FAILED(Initialize_StageName(dynamic_cast<CPlayer*>(m_pOwner)->Get_NickName())))
					return E_FAIL;
			}
			else if (OBJ_TYPE::NPC == m_pOwner->Get_ObjectType())
			{
				if (FAILED(Initialize_StageName(dynamic_cast<CNpc*>(m_pOwner)->Get_NpcName())))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CUI_InGame_NamePlate::Initialize_StageName(const wstring& strName)
{
	m_szFont = L"³Ø½¼Lv1°íµñBold";
	m_strName = strName;
	Ready_TextBox(m_strName);
	Print_InGame_Name();
	Set_Active(true);
	return S_OK;
}

void CUI_InGame_NamePlate::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
}

void CUI_InGame_NamePlate::LateTick(_float fTimeDelta)
{
	Update_NamePlatePos();
}

HRESULT CUI_InGame_NamePlate::Render()
{
	return S_OK;
}

HRESULT CUI_InGame_NamePlate::Ready_Components()
{
	return S_OK;
}

HRESULT CUI_InGame_NamePlate::Bind_ShaderResources()
{
	return S_OK;
}


void CUI_InGame_NamePlate::Start_InGame_Name()
{
	m_strName.clear();
	m_pInGameNameWnd->Set_Active(true);
}

void CUI_InGame_NamePlate::End_InGame_Name()
{
	m_strName.clear();
	m_pInGameNameWnd->Set_Active(false);
}

void CUI_InGame_NamePlate::Update_NamePlatePos()
{
	if (nullptr != m_pOwner)
	{
		Vec3 vHostPos = m_pOwner->Get_EffectPos();

		m_pInGameNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION
			, Vec3(vHostPos.x * g_iWinSizeX * 0.5f, vHostPos.y * g_iWinSizeY * 0.5f, 0.2f));
	
		if (((-1.f <= vHostPos.x) && (1.f >= vHostPos.x)) && ((-1.f <= vHostPos.y) && (1.f >= vHostPos.y)) && ((0.f <= vHostPos.z)&&(1.f >= vHostPos.z)))
			m_pInGameNameWnd->Set_Render(true);
		else
			m_pInGameNameWnd->Set_Render(false);
	}
}

void CUI_InGame_NamePlate::Print_InGame_Name()
{
	if ((nullptr == m_pInGameNameWnd)||(nullptr == m_pOwner))
		return;

	m_pInGameNameWnd->Clear_Text();
	m_pInGameNameWnd->Set_Alpha(1.f);                                                                    

	m_pInGameNameWnd->Get_TransformCom()->Set_Scale(Vec3(300.f, 40.f, 0.f));
	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, m_strName);
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pInGameNameWnd->Set_Text(m_strTag + TEXT("-1"), m_szFont, m_strName, Vec2(150.f - 1.f , 20.f), Vec2(0.35f, 0.35f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pInGameNameWnd->Set_Text(m_strTag + TEXT("-2"), m_szFont, m_strName, Vec2(150.f + 1.f, 20.f), Vec2(0.35f, 0.35f), vOrigin, 0.f, Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_pInGameNameWnd->Set_Text(m_strTag , m_szFont, m_strName, Vec2(150.f, 20.f), Vec2(0.35f, 0.35f), vOrigin, 0.f, Vec4(0.98f, 0.98f, 0.76f, 1.f));
	
}

void CUI_InGame_NamePlate::Set_Active(_bool bActive)
{
	m_bActive = bActive;
	m_pInGameNameWnd->Set_Active(bActive);
}

HRESULT CUI_InGame_NamePlate::Ready_TextBox(const wstring& strName)
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

CUI_InGame_NamePlate* CUI_InGame_NamePlate::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InGame_NamePlate* pInstance = new CUI_InGame_NamePlate(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_InGame_NamePlate");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_NamePlate::Clone(void* pArg)
{
	CUI_InGame_NamePlate* pInstance = new CUI_InGame_NamePlate(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_InGame_NamePlate");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_NamePlate::Free()
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
