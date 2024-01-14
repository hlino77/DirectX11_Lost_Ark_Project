#include "stdafx.h"
#include "UI_ChaosDungeon_NameFrame.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "TextBox.h"

CUI_ChaosDungeon_NameFrame::CUI_ChaosDungeon_NameFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ChaosDungeon_NameFrame::CUI_ChaosDungeon_NameFrame(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ChaosDungeon_NameFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ChaosDungeon_NameFrame::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("ChaosDungeon_NameFrame");

	m_fSizeX = 504;
	m_fSizeY = 335;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = (g_iWinSizeY * 0.5f) + 100.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (FAILED(Initialize_StageName()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChaosDungeon_NameFrame::Initialize_StageName()
{
	m_szFont = L"´øÆÄ¿¬¸¶µÈÄ®³¯";
	Ready_TextBox();

	Set_Active(true);

	return S_OK;
}

void CUI_ChaosDungeon_NameFrame::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_ChaosDungeon_NameFrame::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_ChaosDungeon_NameFrame::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	Print_Stage_Name();

	return S_OK;
}

HRESULT CUI_ChaosDungeon_NameFrame::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Dungeon_NameFrame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChaosDungeon_NameFrame::Bind_ShaderResources()
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

void CUI_ChaosDungeon_NameFrame::Start_Stage_Name()
{
	m_strStageName.clear();
	m_pStageNameWnd->Set_Active(true);
}

void CUI_ChaosDungeon_NameFrame::End_Stage_Name()
{
	m_strStageName.clear();
	m_pStageNameWnd->Set_Active(false);
}

void CUI_ChaosDungeon_NameFrame::Print_Stage_Name()
{
	if (nullptr == m_pStageNameWnd)
		return;

	m_pStageNameWnd->Set_Active(true);
	m_pStageNameWnd->Clear_Text();
	m_pStageNameWnd->Set_Alpha(1.f);

	Vec3 vResultPos = Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);//Vec3(116.f - g_iWinSizeX * 0.5f, -78.f + g_iWinSizeY * 0.5f, 0.f);//Vec3(m_fX- g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f);
	m_pStageNameWnd->Get_TransformCom()->Set_Scale(Vec3(220.f, 80.f, 0.f));
	m_pStageNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vResultPos);

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"´øÆÄ¿¬¸¶µÈÄ®³¯", m_strStageName);
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pStageNameWnd->Set_Text(m_strWndTag, m_szFont, m_strStageName, Vec2(110.f, 40.f), Vec2(0.5f, 0.5f), vOrigin, 0.f, m_vColor);
	m_pStageNameWnd->Set_Text(m_strWndTag + TEXT("-1"), TEXT("³Ø½¼Lv1°íµñBold"), TEXT("º£¸¥ ºÏºÎ"), Vec2(110.f, 20.f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(0.51f, 0.52f, 0.52f, 1.f));
}

void CUI_ChaosDungeon_NameFrame::Set_Active(_bool bActive)
{
	//m_bActive = bActive;
	m_pStageNameWnd->Set_Active(bActive);
}

HRESULT CUI_ChaosDungeon_NameFrame::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pStageNameWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("ChaosStageWnd");
		m_strWndTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(220.f, 80.0f);
		m_pStageNameWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pStageNameWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pStageNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pStageNameWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}


	Safe_Release(pGameInstance);
	return S_OK;
}

CUI_ChaosDungeon_NameFrame* CUI_ChaosDungeon_NameFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ChaosDungeon_NameFrame* pInstance = new CUI_ChaosDungeon_NameFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ChaosDungeon_NameFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ChaosDungeon_NameFrame::Clone(void* pArg)
{
	CUI_ChaosDungeon_NameFrame* pInstance = new CUI_ChaosDungeon_NameFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ChaosDungeon_NameFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ChaosDungeon_NameFrame::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	m_pStageNameWnd->Set_Dead(true);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
