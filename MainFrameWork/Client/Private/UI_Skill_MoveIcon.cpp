#include "stdafx.h"
#include "UI_Skill_MoveIcon.h"
#include "GameInstance.h"
#include "UI_SkillIcon_Frame.h"

CUI_SkillI_MoveFrame::CUI_SkillI_MoveFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_SkillI_MoveFrame::CUI_SkillI_MoveFrame(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_SkillI_MoveFrame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_SkillI_MoveFrame::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Skill_MoveIcon");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 44.f;
    m_fSizeY = 44.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_SkillI_MoveFrame::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

  /*  if (m_bMoveIcon)
    {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(g_hWnd, &pt);

        m_fX = pt.x;
        m_fY = pt.y;

        m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
        m_pTransformCom->Set_State(CTransform::STATE_POSITION,
            Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

        if (KEY_AWAY(KEY::LBTN))
        {
            if ((nullptr != m_pCurrSkillFrame)&&(nullptr != m_pPickedSkillFrame))
            {
                if (m_pCurrSkillFrame != m_pPickedSkillFrame)
                {
                    static_cast<CUI_SkillIcon_Frame*>(m_pPreSkillFrame)->Set_PickedFrame(false);
                    static_cast<CUI_SkillIcon_Frame*>(m_pPreSkillFrame)->Set_IsHaveSkill(false);
                    static_cast<CUI_SkillIcon_Frame*>(m_pCurrSkillFrame)->Set_IsHaveSkill(true);
                    m_pPreSkillFrame = nullptr;
                    m_pCurrSkillFrame = nullptr;
                    m_pPickedSkillFrame = nullptr;
                    m_bMoveIcon = false;
                    Safe_Release(m_pSkillIcon);
                }
            }
        }

    }*/

    if (m_bPicked)
    {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(g_hWnd, &pt);

        m_fX = pt.x;
        m_fY = pt.y;

        m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
        m_pTransformCom->Set_State(CTransform::STATE_POSITION,
            Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    }
}

void CUI_SkillI_MoveFrame::LateTick(_float fTimeDelta)
{
    if(m_bMoveIcon)
        __super::LateTick(fTimeDelta);
}

HRESULT CUI_SkillI_MoveFrame::Render()
{
    if (FAILED(Bind_ShaderResources()))
      return E_FAIL;
       
    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_SkillI_MoveFrame::Set_IsMove()
{
    if(!m_bMoveIcon)
        m_bMoveIcon = true;
}

void CUI_SkillI_MoveFrame::Set_PickedIconFrame(CUI* pSkillFrame)
{
    if ((!m_bMoveIcon) && (static_cast<CUI_SkillIcon_Frame*>(pSkillFrame)->Get_IsHaveSkill()))
    {
        Set_IsMove();
        Set_PreIconFrame(pSkillFrame);
        m_pPickedSkillFrame = pSkillFrame;

        m_pSkillIcon = static_cast<CTexture*>(static_cast<CUI_SkillIcon_Frame*>(m_pPickedSkillFrame)->
            Get_SkillTexture()->Clone(nullptr, nullptr));
    }
}

void CUI_SkillI_MoveFrame::Set_CurrIconFrame(CUI* pSkillFrame)
{
    m_pCurrSkillFrame = pSkillFrame;
}

HRESULT CUI_SkillI_MoveFrame::Ready_Components()
{
    __super::Ready_Components();

    return S_OK;
}

HRESULT CUI_SkillI_MoveFrame::Bind_ShaderResources()
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

    if (nullptr != m_pPickedSkillFrame)
        m_pSkillIcon->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_SkillI_MoveFrame* CUI_SkillI_MoveFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_SkillI_MoveFrame* pInstance = new CUI_SkillI_MoveFrame(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_SkillI_MoveFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_SkillI_MoveFrame::Clone(void* pArg)
{
    CUI_SkillI_MoveFrame* pInstance = new CUI_SkillI_MoveFrame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_SkillI_MoveFrame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_SkillI_MoveFrame::Free()
{
    __super::Free();
    Safe_Release(m_pPreSkillFrame);
    Safe_Release(m_pPickedSkillFrame);
    Safe_Release(m_pCurrSkillFrame);

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
 
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
