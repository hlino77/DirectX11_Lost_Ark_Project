#pragma once
#include "Server_Defines.h"
#include "Camera.h"

BEGIN(Server)

class CCamera_Free_Server final : public CCamera
{
private:
	CCamera_Free_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	CCamera_Free_Server(const CCamera_Free_Server& rhs, CTransform::TRANSFORMDESC* pArg);
	virtual ~CCamera_Free_Server() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	virtual HRESULT Ready_Components() override;

public:
	static CCamera_Free_Server* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


private:
	_bool m_bMouse = true;
};

END