#include "stdafx.h"
#include "UI_SkillIcon_Frame.h"
#include "GameInstance.h"
#include "Player.h"
#include "Player_Skill.h"
#include "ServerSessionManager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"

CUI_SkillIcon_Frame::CUI_SkillIcon_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_SkillIcon_Frame::CUI_SkillIcon_Frame(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_SkillIcon_Frame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_eSkillKey = *static_cast<_uint*>(pArg) + 1;
        wstring strIndex = to_wstring(m_eSkillKey);
        m_strUITag = TEXT("SkillIcon_Frame");
        m_strUITag += strIndex;
    }
    else
        m_strUITag = TEXT("SkillIcon_Frame");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 64.f;
    m_fSizeY = 64.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_SkillIcon_Frame::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    Picking_UI();
    Get_Player_BindingSkill();

    m_fResultCool = m_fCoolMaxTime - m_fCurrCool;
}

void CUI_SkillIcon_Frame::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
    if (!m_bPick)
    {
        if (0.f < m_fAlphaShine)
            m_fAlphaShine -= 2.f* fTimeDelta;
        else if (0 >= m_fAlphaShine)
            m_fAlphaShine = 0.f;
    }
    else
    {
        if (0.8f > m_fAlphaShine)
            m_fAlphaShine += 2.5f * fTimeDelta;
        else if (0.8f <= m_fAlphaShine)
            m_fAlphaShine = 0.8f;
    }

    if (m_fCoolMaxTime == m_fResultCool)
        m_fCoolAngle = XM_PI;
    else
    {
        m_fCoolRatio = 1.0f - (m_fResultCool / m_fCoolMaxTime);
        m_fCoolAngle = -XM_PI + (2 * XM_PI * m_fCoolRatio);
    }

}

HRESULT CUI_SkillIcon_Frame::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if(m_bHaveSkill)
    {
        if (!m_bPicked)
        {
            if (FAILED(Bind_ShaderResources_Skill()))
                return E_FAIL;
        }
        else if(m_bPicked)
        {
            if (FAILED(Bind_ShaderResources_Picked()))
                return E_FAIL;
        }
        m_pTextureCom_Skill->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);

        m_pShaderCom->Begin(8);
        m_pVIBufferCom->Render();
    }

    if (FAILED(Bind_ShaderResources_Shine()))
        return E_FAIL;
    m_pTextureCom_Shine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pTextureCom_Frame->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_SkillIcon_Frame::Set_SkillIcon(const wstring& strSkillName)
{

}

void CUI_SkillIcon_Frame::Get_Player_BindingSkill()
{
    CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
    CTexture* pTexture = nullptr;

    if ((_uint)CHR_CLASS::GUNSLINGER == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectType())
        Get_Player_GN(pPlayer, pTexture);

}

void CUI_SkillIcon_Frame::Get_Player_GN(CPlayer* _pPlayer, CTexture* _pTexture)
{
    if (nullptr != _pPlayer)
    {
        CPlayer_Controller_GN::GN_IDENTITY eIDentity = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(_pPlayer)->Get_GN_Controller())->Get_GN_Identity();
        switch (eIDentity)
        {
        case CPlayer_Controller_GN::GN_IDENTITY::HAND:
            m_iTextureIndex = 0;
            break;
        case CPlayer_Controller_GN::GN_IDENTITY::SHOT:
            if (5 <= m_eSkillKey)
                m_iTextureIndex = 1;
            else
                m_iTextureIndex = 0;
            break;
        case CPlayer_Controller_GN::GN_IDENTITY::LONG:
            if (4 >= m_eSkillKey)
                m_iTextureIndex = 1;
            else
                m_iTextureIndex = 0;
            break;
        }

        m_pSkill = static_cast<CPlayer_Gunslinger*>(_pPlayer)->
            Get_GN_Controller()->Get_PlayerSkill_Weapon(eIDentity, (CPlayer_Controller::SKILL_KEY)m_eSkillKey);
        if (nullptr != m_pSkill)
        {
            _pTexture = (m_pSkill->Get_Skill_Texture());
            m_fCoolMaxTime = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(_pPlayer)->Get_GN_Controller())->Get_Skill_CoolTime((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            m_fCurrCool = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(_pPlayer)->Get_GN_Controller())->Get_Skill_CoolDown((CPlayer_Controller::SKILL_KEY)m_eSkillKey);
            Safe_AddRef(_pTexture);
            if (nullptr != _pTexture)
            {
                m_bHaveSkill = true;
                m_pTextureCom_Skill = static_cast<CTexture*>(_pTexture->Clone(nullptr, nullptr));
            }
            Safe_Release(_pTexture);
        }
        else
        {
            m_bHaveSkill = false;
            m_pTextureCom_Skill = nullptr;
        }
    }
}

void CUI_SkillIcon_Frame::Get_Slayer_WR(CPlayer* _pPlayer, CTexture* _pTexture)
{
}

HRESULT CUI_SkillIcon_Frame::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Empty"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Shine"),
        TEXT("Com_TextureShine"), (CComponent**)&m_pTextureCom_Shine)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Frame"),
        TEXT("Com_TextureFrame"), (CComponent**)&m_pTextureCom_Frame)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources()
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

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources_Picked()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vecPickedColor, sizeof(Vec4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fCoolAngle, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources_Shine()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlphaShine, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources_Skill()
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fCoolAngle, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_SkillIcon_Frame::Bind_ShaderResources_ChangeFrame()
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

  

    return S_OK;
}

CUI_SkillIcon_Frame* CUI_SkillIcon_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_SkillIcon_Frame* pInstance = new CUI_SkillIcon_Frame(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_SkillIcon_Frame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_SkillIcon_Frame::Clone(void* pArg)
{
    CUI_SkillIcon_Frame* pInstance = new CUI_SkillIcon_Frame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_SkillIcon_Frame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_SkillIcon_Frame::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    Safe_Release(m_pTextureCom);//엠프티
    Safe_Release(m_pTextureCom_Skill);//테스트용
    Safe_Release(m_pTextureCom_Shine);//마우스 올릴시 빛나는 이펙트
    Safe_Release(m_pTextureCom_Frame);//프레임
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
