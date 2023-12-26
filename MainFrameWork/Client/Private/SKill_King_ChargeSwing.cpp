#include "stdafx.h"
#include "SKill_King_ChargeSwing.h"
#include "GameInstance.h"


CSKill_King_ChargeSwing::CSKill_King_ChargeSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSKill_King_ChargeSwing::CSKill_King_ChargeSwing(const CSKill_King_ChargeSwing& rhs)
          : CSkill(rhs)
{
}

HRESULT CSKill_King_ChargeSwing::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSKill_King_ChargeSwing::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
    return S_OK;
}

void CSKill_King_ChargeSwing::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CSKill_King_ChargeSwing::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CSKill_King_ChargeSwing::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}

void CSKill_King_ChargeSwing::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_King_ChargeSwing::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_King_ChargeSwing::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSKill_King_ChargeSwing::Ready_Coliders()
{
    return S_OK;
}

HRESULT CSKill_King_ChargeSwing::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;

    return S_OK;
}

CSKill_King_ChargeSwing* CSKill_King_ChargeSwing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSKill_King_ChargeSwing* pInstance = new CSKill_King_ChargeSwing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_King");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSKill_King_ChargeSwing::Clone(void* pArg)
{
    CSKill_King_ChargeSwing* pInstance = new CSKill_King_ChargeSwing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_King");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSKill_King_ChargeSwing::Free()
{
	__super::Free();
}