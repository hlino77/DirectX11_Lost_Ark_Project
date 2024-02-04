#include "stdafx.h"
#include "UI_PartyHPWnd.h"
#include "GameInstance.h"
#include "TextBox.h"
#include "Player.h"

CUI_PartyHPWnd::CUI_PartyHPWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}

CUI_PartyHPWnd::CUI_PartyHPWnd(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_PartyHPWnd::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_PartyHPWnd::Initialize(void* pArg)
{

	if (nullptr != pArg)
	{
		m_strUITag = TEXT("UI_PartyHPWnd") + to_wstring(*static_cast<_uint*>(pArg));

		m_fSizeX = 208.f;
		m_fSizeY = 28.f;
		m_fX = 160.f;
		m_fY = 350.f + (60.f * (*static_cast<_uint*>(pArg)));
		m_fHpWndAlpha = 1.5f;
		m_iEmblemIndex = *static_cast<_uint*>(pArg);
		if (0 == m_iEmblemIndex)
			m_bPartyLeader = true;
	}
	else
	{
		m_strUITag = TEXT("UI_PartyHPWnd");

		m_fSizeX = 208.f;
		m_fSizeY = 28.f;
		m_fX = 160.f;
		m_fY = 280.f;
		m_fHpWndAlpha = 1.5f;
	}

	/*
	if (nullptr != pArg)
	{
		PARTYHP_DESC*	pPartyDesc;
		pPartyDesc	= static_cast<PARTYHP_DESC*>(pArg);
		m_strUITag = TEXT("UI_PartyHPWnd") + to_wstring(pPartyDesc->iPartyIndex);
		m_strName = pPartyDesc->pPlayer->Get_NickName();
		m_fMaxHp = (_float)m_pIndexPlayer->Get_PlayerStat_Desc().iMaxHp;
		m_fCurrHp = (_float )m_pIndexPlayer->Get_PlayerStat_Desc().iCurHp;
		m_fHpRatio = m_fCurrHp/m_fMaxHp;
		m_fSizeX = 208.f;
		m_fSizeY = 28.f;
		m_fX = 160.f;
		m_fY = 280.f + (60.f * (pPartyDesc->iPartyIndex));
		m_fHpWndAlpha = 1.5f;
		m_iEmblemIndex = pPartyDesc->iPartyIndex;
		if (0 == (pPartyDesc->iPartyIndex - 1))
			m_bPartyLeader = true;
	}
	*/

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pTransformCom
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	m_fX_Emblem = 63.f * 0.5f;
	//m_pTransform_PartyEmblem
	m_pTransform_PartyEmblem->Set_Scale(Vec3(63.f * 0.7f, 62.f * 0.7f, 1.f));
	m_pTransform_PartyEmblem->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX_Emblem - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	
	m_fX_Num = 70.f;
	//m_pTransform_Num
	m_pTransform_Num->Set_Scale(Vec3(12.f , 18.f, 1.f));
	m_pTransform_Num->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX_Num - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	//m_pTransform_LeaderCrown
	m_pTransform_LeaderCrown->Set_Scale(Vec3(23.f, 20.f, 1.f));
	m_pTransform_LeaderCrown->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - (m_fSizeX * 0.45f) ) - g_iWinSizeX * 0.5f, -(m_fY - (m_fSizeY * 0.5f) - 10.f) + g_iWinSizeY * 0.5f, 0.2f));
	
	//m_pTransform_DeathMark
	m_pTransform_DeathMark->Set_Scale(Vec3(40.f, 40.f, 1.f));
	m_pTransform_DeathMark->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX_Emblem - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	
	if(FAILED(Initialize_TextBox()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_PartyHPWnd::Initialize_TextBox()
{
	m_strFont = TEXT("³Ø½¼Lv1°íµñBold");
	if (FAILED(Ready_NameTextBox()))
		return E_FAIL;
	m_pTextBox->Set_Active(true);
	Print_NickName();
	return S_OK;
}

void CUI_PartyHPWnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_PartyHPWnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Update_PlayeHp();
	if (0 == m_fHpRatio)//TestCode
	{
		m_iNumTextureIndex = 1;
		m_fDeathMarkAlpha = 1.f;
		m_vTextColor = Vec4(0.3f, 0.3f, 0.3f, 1.f);
	}
	else
	{
		m_iNumTextureIndex = 0;
		m_fDeathMarkAlpha = 0.f;
		m_vTextColor = Vec4(1.f, 1.f, 1.f, 1.f);
	}
	Print_NickName();
}

HRESULT CUI_PartyHPWnd::Render()
{
	if(FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_HpFill()))
		return E_FAIL;
	m_pShaderCom->Begin(16);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Emblem()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_DeathMark()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Num()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (m_bPartyLeader)
	{
		if (FAILED(Bind_ShaderResources_LeaderCrown()))
			return E_FAIL;
		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render();
	}

	m_pTextBox->Render();

	return S_OK;
}

void CUI_PartyHPWnd::Print_NickName()
{
	if (nullptr == m_pTextBox)
		return;

	m_pTextBox->Clear_Text();
	m_pTextBox->Set_Alpha(1.0f);
	m_pTextBox->Get_TransformCom()->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));
	m_pTextBox->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(L"³Ø½¼Lv1°íµñBold", m_strName);
	Vec2 vOrigin = vMeasure * 0.5f;
	m_pTextBox->Set_Text(m_strWndTag, m_strFont, m_strName, Vec2(m_fSizeX * 0.1f , m_fSizeY * 0.2f), Vec2(0.4f, 0.4f), Vec2(0.f, 0.f), 0.f, m_vTextColor);
}

void CUI_PartyHPWnd::Update_PlayeHp()
{
	if (nullptr == m_pIndexPlayer)
		return;

	m_fCurrHp = m_pIndexPlayer->Get_PlayerStat_Desc().iCurHp;
	m_fHpRatio = m_fCurrHp / m_fMaxHp;

	/*if ((0 == m_fHpRatio)||(TEXT("Dead_End") == m_pIndexPlayer->Get_State()))
	{
		m_iNumTextureIndex = 1;
		m_fDeathMarkAlpha = 1.f;
	}
	else
	{
		m_iNumTextureIndex = 0;
		m_fDeathMarkAlpha = 0.f;
	}*/
}

HRESULT CUI_PartyHPWnd::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	/*m_pTexture_PartyHPWnd*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_HpWnd"),
		TEXT("Com_Texture_PartyHPWnd"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	/*m_pTexture_PartyHPFill*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_HpFill"),
		TEXT("Com_Texture_PartyHPFill"), (CComponent**)&m_pTexture_PartyHPFill)))
		return E_FAIL;
	/*m_pTexture_PartyEmblem*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_HPUI_Emblem"),
		TEXT("Com_Texture_PartyEmblem"), (CComponent**)&m_pTexture_PartyEmblem)))
		return E_FAIL;
	/*m_pTexture_Num*/
	switch (m_iEmblemIndex)
	{
	case 0:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_No.1"),
			TEXT("Com_Texture_Num"), (CComponent**)&m_pTexture_Num)))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_No.2"),
			TEXT("Com_Texture_Num"), (CComponent**)&m_pTexture_Num)))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_No.3"),
			TEXT("Com_Texture_Num"), (CComponent**)&m_pTexture_Num)))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_No.4"),
			TEXT("Com_Texture_Num"), (CComponent**)&m_pTexture_Num)))
			return E_FAIL;
		break;
	}
	/*m_pTexture_LeaderCrown*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_PartyLeader"),
		TEXT("Com_Texture_LeaderCrown"), (CComponent**)&m_pTexture_LeaderCrown)))
		return E_FAIL;
	/*m_pTexture_DeathMark*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Party_Death_Mark"),
		TEXT("Com_Texture_DeathMark"), (CComponent**)&m_pTexture_DeathMark)))
		return E_FAIL;

	/*m_pTransform_PartyHP;*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_PartyHP"), (CComponent**)&m_pTransform_PartyHP)))
		return E_FAIL;
	/* m_pTransform_PartyEmblem */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_PartyEmblem"), (CComponent**)&m_pTransform_PartyEmblem)))
		return E_FAIL;
	/*m_pTransform_LeaderCrown*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_LeaderCrown"), (CComponent**)&m_pTransform_LeaderCrown)))
		return E_FAIL;
	/*m_pTransform_Num*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Num"), (CComponent**)&m_pTransform_Num)))
		return E_FAIL;
	/*m_pTransform_DeathMark*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_DeathMark"), (CComponent**)&m_pTransform_DeathMark)))
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_PartyHPWnd::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fHpWndAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_PartyHPWnd::Bind_ShaderResources_HpFill()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fHpRatio, sizeof(_float))))
		return E_FAIL;
	m_pTexture_PartyHPFill->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_PartyHPWnd::Bind_ShaderResources_Emblem()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_PartyEmblem->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_PartyEmblem->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iEmblemIndex);
	return S_OK;
}

HRESULT CUI_PartyHPWnd::Bind_ShaderResources_DeathMark()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_DeathMark->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	m_fDeathMarkAlpha = 1.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fDeathMarkAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_DeathMark->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_PartyHPWnd::Bind_ShaderResources_Num()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Num->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_Num->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iNumTextureIndex);

	return S_OK;
}

HRESULT CUI_PartyHPWnd::Bind_ShaderResources_LeaderCrown()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_LeaderCrown->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_LeaderCrown->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_PartyHPWnd::Ready_NameTextBox()
{
	if (nullptr == m_pTextBox)
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		Safe_AddRef(pGameInstance);

		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = to_wstring(m_iEmblemIndex) + TEXT("'s_Player_NameWnd");
		m_strWndTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(m_fSizeX, m_fSizeY);
		m_pTextBox = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pTextBox)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}
		m_pTextBox->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pTextBox->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
		//m_pTextBox->Set_Render(false);

		Safe_Release(pGameInstance);
	}

	return S_OK;
}

CUI_PartyHPWnd* CUI_PartyHPWnd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PartyHPWnd* pInstance = new CUI_PartyHPWnd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_PartyHPWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PartyHPWnd::Clone(void* pArg)
{
	CUI_PartyHPWnd* pInstance = new CUI_PartyHPWnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_PartyHPWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PartyHPWnd::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_PartyEmblem);
	Safe_Release(m_pTexture_PartyHPFill);

	Safe_Release(m_pTexture_Num);
	Safe_Release(m_pTransform_Num);

	Safe_Release(m_pTransform_PartyHP);
	Safe_Release(m_pTransform_PartyEmblem);

	Safe_Release(m_pTexture_LeaderCrown);
	Safe_Release(m_pTransform_LeaderCrown);

	Safe_Release(m_pTexture_DeathMark);
	Safe_Release(m_pTransform_DeathMark);

	if (nullptr != m_pTextBox)
		m_pTextBox->Set_Dead(true);
}
