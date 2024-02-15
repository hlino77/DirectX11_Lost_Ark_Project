#include "stdafx.h"
#include "UI_ChaosDungeon_TimerFrame.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "TextBox.h"

CUI_ChaosDungeon_TimerFrame::CUI_ChaosDungeon_TimerFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ChaosDungeon_TimerFrame::CUI_ChaosDungeon_TimerFrame(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ChaosDungeon_TimerFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ChaosDungeon_TimerFrame::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("ChaosDungeon_TimerFrame");

	m_fSizeX = 240.f;
	m_fSizeY = 80.f;
	m_fX = 354.f;
	m_fY = 66.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (FAILED(Initialize_StageName()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChaosDungeon_TimerFrame::Initialize_StageName()
{
	m_szFont = L"던파연마된칼날";
	Ready_TextBox();
	Set_Active(true);
	Print_Stage_Timer();
	return S_OK;
}

void CUI_ChaosDungeon_TimerFrame::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_ChaosDungeon_TimerFrame::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_ChaosDungeon_TimerFrame::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();
	
	m_pStageNameWnd->Render();

	return S_OK;
}

HRESULT CUI_ChaosDungeon_TimerFrame::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Dungeon_TimerFrame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChaosDungeon_TimerFrame::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

void CUI_ChaosDungeon_TimerFrame::Start_Stage_Timer()
{
	m_strStageTimer.clear();
	m_pStageNameWnd->Set_Active(true);
}

void CUI_ChaosDungeon_TimerFrame::End_Stage_Timer()
{
	m_strStageTimer.clear();
	m_pStageNameWnd->Set_Active(false);
}

void CUI_ChaosDungeon_TimerFrame::Print_Stage_Timer()
{
	if (nullptr == m_pStageNameWnd)
		return;

	m_pStageNameWnd->Clear_Text();
	m_pStageNameWnd->Set_Alpha(1.f);

	Vec3 vResultPos = Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 25.f) + g_iWinSizeY * 0.5f, 0.f);//Vec3(m_fX- g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);
	m_pStageNameWnd->Get_TransformCom()->Set_Scale(Vec3(240.f, 80.0f, 0.f));
	m_pStageNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"던파연마된칼날", m_strTimerAnnounce);
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pStageNameWnd->Set_Text(L"ChaosAnnounceWnd", m_szFont, m_strTimerAnnounce, Vec2(120.f, 40.f), Vec2(0.4f, 0.4f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//m_pStageNameWnd->Set_Text(L"ChaosTimerWnd", m_szFont, m_strStageTimer, Vec2(110.f, 40.f), Vec2(1.0f, 1.0f), vOrigin, 0.f, m_vColor);
}

void CUI_ChaosDungeon_TimerFrame::Set_Active(_bool bActive)
{
	//m_bActive = bActive;
	m_pStageNameWnd->Set_Active(bActive);
}

HRESULT CUI_ChaosDungeon_TimerFrame::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pStageNameWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("ChaosAnnounceWnd");
		tTextDesc.vSize = Vec2(240.f, 80.0f);
		m_pStageNameWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pStageNameWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_pStageNameWnd->Set_Render(false);
		m_pStageNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pStageNameWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

CUI_ChaosDungeon_TimerFrame* CUI_ChaosDungeon_TimerFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ChaosDungeon_TimerFrame* pInstance = new CUI_ChaosDungeon_TimerFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ChaosDungeon_TimerFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ChaosDungeon_TimerFrame::Clone(void* pArg)
{
	CUI_ChaosDungeon_TimerFrame* pInstance = new CUI_ChaosDungeon_TimerFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ChaosDungeon_TimerFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ChaosDungeon_TimerFrame::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	m_pStageNameWnd->Set_Dead(true);
}
