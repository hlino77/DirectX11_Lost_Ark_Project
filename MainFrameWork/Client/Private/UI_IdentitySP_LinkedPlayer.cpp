#include "stdafx.h"
#include "UI_IdentitySP_LinkedPlayer.h"
#include "Player.h"

CUI_IdentitySP_LinkedPlayer::CUI_IdentitySP_LinkedPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_IdentitySP_LinkedPlayer::CUI_IdentitySP_LinkedPlayer(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_IdentitySP_LinkedPlayer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Initialize(void* pArg)
{
    return S_OK;
}

void CUI_IdentitySP_LinkedPlayer::Tick(_float fTimeDelta)
{
}

void CUI_IdentitySP_LinkedPlayer::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_IdentitySP_LinkedPlayer::Render()
{
    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Ready_Components()
{
    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources()
{
    return S_OK;
}

CUI_IdentitySP_LinkedPlayer* CUI_IdentitySP_LinkedPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

CGameObject* CUI_IdentitySP_LinkedPlayer::Clone(void* pArg)
{
    return nullptr;
}

void CUI_IdentitySP_LinkedPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);

	Safe_Release(m_pTransformCom);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
