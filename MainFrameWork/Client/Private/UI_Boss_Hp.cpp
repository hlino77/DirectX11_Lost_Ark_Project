#include "stdafx.h"
#include "UI_Boss_Hp.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "TextBox.h"
#include "ServerSessionManager.h"
#include "Boss.h"
#include "UI_Manager.h"

CUI_Boss_Hp::CUI_Boss_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Boss_Hp::CUI_Boss_Hp(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Boss_Hp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Boss_Hp::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("Boss_HpUI");

	m_fSizeX = 100;
	m_fSizeY = 80;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	m_fSizeXHp = 452.f;
	m_fSizeYHp = 21.f;
	m_fXHp = g_iWinSizeX * 0.5f;
	m_fYHp = 70.f;
	m_pTransformCom_Hp->Set_Scale(Vec3(m_fSizeXHp, m_fSizeYHp, 1.f));
	m_pTransformCom_Hp->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fXHp - g_iWinSizeX * 0.5f, -m_fYHp + g_iWinSizeY * 0.5f, 0.2f));

	_float fSizeX = 615.f;
	_float fSizeY = 155.f;
	_float fX = g_iWinSizeX * 0.5f;
	_float fY = 70.f;
	m_pTransformCom_Frame->Set_Scale(Vec3(fSizeX, fSizeY, 1.f));
	m_pTransformCom_Frame->Set_State(CTransform::STATE_POSITION,
		Vec3(fX - g_iWinSizeX * 0.5f, -fY + g_iWinSizeY * 0.5f, 0.2f));


	_float fSizeX_Groggy = fSizeX * 0.8f;
	_float fSizeY_Groggy = 10.f;
	_float fY_Groggy = 90.f;
	m_pTransformCom_Groggy->Set_Scale(Vec3(fSizeX_Groggy, fSizeY_Groggy, 1.f));
	m_pTransformCom_Groggy->Set_State(CTransform::STATE_POSITION,
		Vec3(fX - g_iWinSizeX * 0.5f, -fY_Groggy + g_iWinSizeY * 0.5f, 0.2f));

	m_pTransformCom_GroggyGauge->Set_Scale(Vec3(fSizeX_Groggy, fSizeY_Groggy * 0.8f, 1.f));
	m_pTransformCom_GroggyGauge->Set_State(CTransform::STATE_POSITION,
		Vec3(fX - g_iWinSizeX * 0.5f, -fY_Groggy + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (nullptr != pArg)
	{
		m_pOwner = static_cast<CGameObject*>(pArg);
		m_strBossName = dynamic_cast<CBoss*>(m_pOwner)->Get_ObjectTag();
		m_iTargetName = dynamic_cast<CBoss*>(m_pOwner)->Get_ObjectID();
		m_iMaxHp = dynamic_cast<CBoss*>(m_pOwner)->Get_MaxHp();
		m_iCurrHp = dynamic_cast<CBoss*>(m_pOwner)->Get_Hp();
		m_iPreHp = m_iCurrHp;
		m_fHpRatio = (_float)m_iCurrHp /(_float)m_iMaxHp;
		m_iMaxGroggyGauge = dynamic_cast<CBoss*>(m_pOwner)->Get_MaxGroggyGauge();
		m_iGroggyGauge = dynamic_cast<CBoss*>(m_pOwner)->Get_CurrGroggyGauge();
		m_fGroggyRatio = (_float)m_iGroggyGauge / (_float)m_iMaxGroggyGauge;
		Initialize_BossHP();
		Initialize_Position();
	}

	return S_OK;
}

HRESULT CUI_Boss_Hp::Initialize_BossHP()
{
	
	m_szFont = L"³Ø½¼Lv1°íµñBold";
	Ready_TextBox(m_strBossName);
	Set_Active(false);

	return S_OK;
}

void CUI_Boss_Hp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if(!m_bDead)
		Update_PreHp();
}

void CUI_Boss_Hp::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (!m_bDead)
		Update_Hp(fTimeDelta);
}

HRESULT CUI_Boss_Hp::Render()
{
	if (FAILED(Bind_ShaderResources_Frame()))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Frame->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iBossType)))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	//³ªÁß¿¡ ÁÙ¾îµé HP
	if (FAILED(Bind_ShaderResources_Hp_Rough()))
		return E_FAIL;
	if (FAILED(m_pTextureCom_NextHp->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iNextHpColor)))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	//½ÇÁ¦ HP
	if (FAILED(Bind_ShaderResources_Hp()))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Hp->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iCurrHpColor)))
		return E_FAIL;
	m_pShaderCom->Begin(16);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iBossType)))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_GroggyFrame()))
		return E_FAIL;
	if (FAILED(m_pTextureCom_Groggy->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_GroggyGauge()))
		return E_FAIL;
	if (FAILED(m_pTextureCom_GroggyGauge->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	m_pShaderCom->Begin(16);
	m_pVIBufferCom->Render();

	//if(m_bRender)
		//Print_BossHp();

	return S_OK;
}

HRESULT CUI_Boss_Hp::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Emblem"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Hp"),
		TEXT("Com_TextureHp"), (CComponent**)&m_pTextureCom_Hp)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_TransformHp"), (CComponent**)&m_pTransformCom_Hp)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_HpFrame"),
		TEXT("Com_TextureFrame"), (CComponent**)&m_pTextureCom_Frame)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_TransformFrame"), (CComponent**)&m_pTransformCom_Frame)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Hp"),
		TEXT("Com_TextureNextHp"), (CComponent**)&m_pTextureCom_NextHp)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Groggy_GaugeFrame"),
		TEXT("Com_TextureGroggyFrame"), (CComponent**)&m_pTextureCom_Groggy)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Groggy_Gauge"),
		TEXT("Com_TextureGroggyGauge"), (CComponent**)&m_pTextureCom_GroggyGauge)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_TransformGroggy"), (CComponent**)&m_pTransformCom_Groggy)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_TransformGroggyGauge"), (CComponent**)&m_pTransformCom_GroggyGauge)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Boss_Hp::Bind_ShaderResources()
{
	if(FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Boss_Hp::Bind_ShaderResources_Hp_Rough()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_Hp->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return  S_OK;
}

HRESULT CUI_Boss_Hp::Bind_ShaderResources_Hp()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_Hp->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fHpRatio, sizeof(_float))))
		return E_FAIL;

	return  S_OK;
}

HRESULT CUI_Boss_Hp::Bind_ShaderResources_Frame()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_Frame->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fAlpha = 0.8f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return  S_OK;
}

HRESULT CUI_Boss_Hp::Bind_ShaderResources_GroggyFrame()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_Groggy->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return  S_OK;
}

HRESULT CUI_Boss_Hp::Bind_ShaderResources_GroggyGauge()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_GroggyGauge->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fGroggyRatio, sizeof(_float))))
		return E_FAIL;

	return  S_OK;
}

void CUI_Boss_Hp::Initialize_Position()
{
	if (TEXT("Boss_Golem") == m_strBossName)
	{
		m_iBossType = (_uint)BOSS_GOLEM;
		m_fX = 560.f;
		m_fY = 55.f;
		m_fSizeXHp = 452.f;
		m_fSizeYHp = 18.f;
		m_fXHp = 817.f;
		m_fYHp = 60.f;

		m_iHpCount = 10;
		m_strOutputName = TEXT("¸ùÈ¯±º´Ü ·è");
		m_strMonsterRank = TEXT("º¸½º");
	}
	else if (TEXT("Boss_King") == m_strBossName)
	{
		m_iBossType = (_uint)BOSS_KING;
		m_fX = 540.f;
		m_fY = 50.f;
		m_fSizeXHp = 472.f;
		m_fSizeYHp = 20.f;
		m_fXHp = 807.f;
		m_fYHp = 57.f;

		m_iHpCount = 20;
		m_strOutputName = TEXT("¸ùÈ¯±º´Ü Å·");
		m_strMonsterRank = TEXT("º¸½º");
	}
	else if (TEXT("Boss_Valtan") == m_strBossName)
	{
		m_iBossType = (_uint)BOSS_VALTAN;
		m_fX = 530.f;
		m_fY = 50.f;
		m_fSizeXHp = 520.f;
		m_fSizeYHp = 20.f;
		m_fXHp = 830.f;
		m_fYHp = 55.f;

		m_iHpCount = 160;
		m_strOutputName = TEXT("¸¶¼ö±º´ÜÀå ¹ßÅº");
		m_strMonsterRank = TEXT("±º´ÜÀå");
	}
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	m_pTransformCom_Hp->Set_Scale(Vec3(m_fSizeXHp, m_fSizeYHp, 1.f));
	m_pTransformCom_Hp->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fXHp - g_iWinSizeX * 0.5f, -m_fYHp + g_iWinSizeY * 0.5f, 0.2f));

	m_pTransformCom_Groggy->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fXHp - g_iWinSizeX * 0.5f, -(m_fYHp + 30.f) + g_iWinSizeY * 0.5f, 0.2f));
	m_pTransformCom_GroggyGauge->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fXHp - g_iWinSizeX * 0.5f, -(m_fYHp + 30.f) + g_iWinSizeY * 0.5f, 0.2f));

	m_pInGameNameWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fXHp - g_iWinSizeX * 0.5f, -(m_fYHp -30.f) + g_iWinSizeY * 0.5f, 0.f));
	m_pInGameHpWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fXHp - g_iWinSizeX * 0.5f, -m_fYHp + g_iWinSizeY * 0.5f, 0.f));
	m_pInGameHpCountWnd->Get_TransformCom()->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fXHp + 200.f) - g_iWinSizeX * 0.5f, -m_fYHp + g_iWinSizeY * 0.5f, 0.f));

	m_iCurrHpColor = HP_BLUE;
	m_iNextHpColor = HP_SIAN;
	m_fDivideCountHp = (_float)m_iMaxHp / (_float)m_iHpCount;
}

void CUI_Boss_Hp::Update_Hp(_float fTimeDelta)
{
	m_iGroggyGauge = dynamic_cast<CBoss*>(m_pOwner)->Get_CurrGroggyGauge();
	m_fGroggyRatio = (_float)m_iGroggyGauge / (_float)m_iMaxGroggyGauge;

	m_iCurrHp = dynamic_cast<CBoss*>(m_pOwner)->Get_Hp();
	if(0 <= (m_iHpCount - 1))
		m_fHpRatio = ((_float)m_iCurrHp - (m_fDivideCountHp * (m_iHpCount - 1))) / m_fDivideCountHp;
	else
		m_fHpRatio = (_float)m_iCurrHp / m_fDivideCountHp;

	if (0.f >= m_fHpRatio)
	{
		if(0 <= (m_iHpCount-1))
			m_iHpCount -= 1;
	
		m_iCurrHpColor = m_iNextHpColor;
		m_iNextHpColor++;
		if(((_uint)HP_PURPLE < m_iNextHpColor)&& (1 < (m_iHpCount - 1)))
			m_iNextHpColor = (_uint)HP_BLUE;
		else if (1 > (m_iHpCount - 1))
		{
			m_iCurrHpColor = (_uint)HP_RED;
			m_iNextHpColor = (_uint)HP_END;
		}
	}
}

void CUI_Boss_Hp::Update_PreHp()
{
	if (m_iPreHp != m_iCurrHp)
	{
		CUI_Manager::GetInstance()->Set_CurrHPUI(this);
		Print_BossHp();
		m_iPreHp = m_iCurrHp;
	}
}



void CUI_Boss_Hp::Update_BossHp()
{
	if (nullptr != m_pInGameHpWnd)
	{
		m_pInGameHpWnd->Clear_Text();
		m_pInGameHpWnd->Set_Alpha(1.f);
		m_pInGameHpWnd->Get_TransformCom()->Set_Scale(Vec3(480.f, 20.0f, 0.f));
		if (0 >= m_iCurrHp)
			m_iCurrHp = 0;
		wstring strHPInfo = to_wstring(m_iCurrHp) + TEXT("/") + to_wstring(m_iMaxHp);
		Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, strHPInfo);
		Vec2 vOrigin = vMeasure * 0.5f;
		m_pInGameHpWnd->Set_Text(m_strTagHP, m_szFont, strHPInfo, Vec2(225.f, 10.f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.f));
	}

	if (nullptr != m_pInGameHpCountWnd)
	{
		m_pInGameHpCountWnd->Clear_Text();
		m_pInGameHpCountWnd->Set_Alpha(1.f);
		m_pInGameHpCountWnd->Get_TransformCom()->Set_Scale(Vec3(50.f, 20.0f, 0.f));
		wstring strHpCount = TEXT("x") + to_wstring(m_iHpCount);
		Vec2 vMeasure = CGameInstance::GetInstance()->MeasureString(m_szFont, strHpCount);
		Vec2 vOrigin = vMeasure * 0.5f;
		m_pInGameHpCountWnd->Set_Text(m_strTagHpCount, m_szFont, strHpCount, Vec2(25.f, 10.f), Vec2(0.3f, 0.3f), vOrigin, 0.f, Vec4(1.0f, 1.0f, 1.0f, 1.f));
	}
}

void CUI_Boss_Hp::Set_Active(_bool bActive)
{
	if(bActive == true)
		Update_BossHp();
	m_bActive = bActive;
	m_bRender = bActive;
	m_pInGameNameWnd->Set_Active(bActive);
	m_pInGameHpWnd->Set_Active(bActive);
	m_pInGameHpCountWnd->Set_Active(bActive);
}

HRESULT CUI_Boss_Hp::Ready_TextBox(const wstring& strName)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pInGameNameWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = strName + TEXT("_NameTag") + to_wstring(m_iTargetName);
		m_strTag = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(480.0f, 20.0f);
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

	if (nullptr == m_pInGameHpWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = strName + TEXT("_HPUI") + to_wstring(m_iTargetName);
		m_strTagHP = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(480.0f, 20.0f);
		m_pInGameHpWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pInGameHpWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pInGameHpWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pInGameHpWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}

	if (nullptr == m_pInGameHpCountWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = strName + TEXT("_HPCountUI") + to_wstring(m_iTargetName);
		m_strTagHpCount = tTextDesc.szTextBoxTag;
		tTextDesc.vSize = Vec2(50.f, 20.0f);
		m_pInGameHpCountWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));

		if (nullptr == m_pInGameHpCountWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pInGameHpCountWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pInGameHpCountWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

CUI_Boss_Hp* CUI_Boss_Hp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Boss_Hp* pInstance = new CUI_Boss_Hp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Boss_Hp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Boss_Hp::Clone(void* pArg)
{
	CUI_Boss_Hp* pInstance = new CUI_Boss_Hp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Boss_Hp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Boss_Hp::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	m_pInGameNameWnd->Set_Dead(true);
	m_pInGameHpWnd->Set_Dead(true);
	m_pInGameHpCountWnd->Set_Dead(true);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_Hp);
	Safe_Release(m_pTextureCom_Frame);
	Safe_Release(m_pTextureCom_NextHp);
	Safe_Release(m_pTextureCom_Groggy);
	Safe_Release(m_pTextureCom_GroggyGauge);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTransformCom_Hp);
	Safe_Release(m_pTransformCom_Frame);
	Safe_Release(m_pTransformCom_Groggy);
	Safe_Release(m_pTransformCom_GroggyGauge);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
