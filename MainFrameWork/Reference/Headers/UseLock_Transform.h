#pragma once

#include "Transform.h"
#include "AsTypes.h"
#include "Lock.h"

BEGIN(Engine)

class ENGINE_DLL CUseLock_Transform final : public CTransform
{

private:
	CUseLock_Transform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUseLock_Transform(const CUseLock_Transform& rhs);
	virtual ~CUseLock_Transform() = default;

	USE_LOCK
public:
	virtual Vec3 Get_State(STATE eState) override;

	virtual Matrix Get_WorldMatrix() override;


	virtual Matrix Get_WorldMatrix_TP() override;

	virtual Matrix Get_WorldMatrixInverse() override;

	virtual void Set_State(STATE eState, Vec3 vState) override;
	virtual void Set_WorldMatrix(Matrix matWorld) override;




public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual void Go_Straight(_float fSpeed, _float fTimeDelta) override;
	virtual void Go_Backward(_float fSpeed, _float fTimeDelta) override;
	virtual void Go_Left(_float fSpeed, _float fTimeDelta) override;
	virtual void Go_Right(_float fSpeed, _float fTimeDelta) override;
	virtual void Go_Up(_float fTimeDelta) override;
	virtual void Go_Down(_float fTimeDelta) override;

	virtual void Set_Scale(Vec3 vScaleInfo) override;
	virtual Vec3 Get_Scale() override;

	virtual void Turn(Vec3 vAxis, _float fTimeDelta) override;
	virtual void Turn_Speed(Vec3 vAxis, _float fSpeed, _float fTimeDelta) override;
	virtual void Rotation(Vec3 vAxis, _float fRadian) override;

	virtual void LookAt(Vec3 vAt) override;
	virtual void LookAt_ForLandObject(Vec3 vAt) override;
	virtual void LookAt_Dir(Vec3 vDir) override;
	virtual void LookAt_SetUp(Vec3 vAt) override;
	virtual void Move(Vec3 vTargetPos, _float fTimeDelta, _float fLimitDistance = 0.1f) override;
	virtual void Move_Dir(Vec3 vDir, _float fTimeDelta) override;
	virtual void LookAt_Lerp(Vec3 vAt, _float fSpeed, _float fTimeDelta) override;
	virtual void LookAt_Lerp_ForLand(Vec3 vAt, _float fSpeed, _float fTimeDelta) override;
	virtual void SetUp_Lerp(Vec3 vLook, _float fSpeed, _float fTimeDelta) override;
	virtual void Move_Pos(Vec3 vTargetPos) override;


	virtual void Set_Up(Vec3 vNormal) override;


public:
	static CUseLock_Transform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pObject, void* pArg) override;
	virtual void Free() override;
};

END