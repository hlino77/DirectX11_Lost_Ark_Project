#pragma once

#include "Component.h"
#include "AsTypes.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

	typedef struct tagTransformDesc
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
	}TRANSFORMDESC;


protected:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	virtual Vec3 Get_State(STATE eState) PURE;

	virtual Matrix Get_WorldMatrix() PURE;


	virtual Matrix Get_WorldMatrix_TP() PURE;

	virtual Matrix Get_WorldMatrixInverse() PURE;

	virtual void Set_State(STATE eState, Vec3 vState) PURE;
	virtual void Set_WorldMatrix(Matrix matWorld) PURE;




public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	virtual void Go_Straight(_float fSpeed, _float fTimeDelta) PURE;
	virtual void Go_Backward(_float fSpeed, _float fTimeDelta) PURE;
	virtual void Go_Left(_float fSpeed, _float fTimeDelta) PURE;
	virtual void Go_Right(_float fSpeed, _float fTimeDelta) PURE;
	virtual void Go_Up(_float fTimeDelta) PURE;
	virtual void Go_Down(_float fTimeDelta) PURE;

	virtual void Set_Scale(Vec3 vScaleInfo) PURE;
	virtual Vec3 Get_Scale() PURE;

	virtual void Turn(Vec3 vAxis, _float fTimeDelta) PURE;
	virtual void Turn_Speed(Vec3 vAxis, _float fSpeed, _float fTimeDelta) PURE;
	virtual void Rotation(Vec3 vAxis, _float fRadian) PURE;

	virtual void LookAt(Vec3 vAt) PURE;
	virtual void LookAt_ForLandObject(Vec3 vAt) PURE;
	virtual void LookAt_Dir(Vec3 vDir) PURE;
	virtual void LookAt_SetUp(Vec3 vAt) PURE;
	virtual void Move(Vec3 vTargetPos, _float fTimeDelta, _float fLimitDistance = 0.1f) PURE;
	virtual void Move_Dir(Vec3 vDir, _float fTimeDelta) PURE;
	virtual void LookAt_Lerp(Vec3 vAt, _float fSpeed, _float fTimeDelta) PURE;
	virtual void LookAt_Lerp_ForLand(Vec3 vAt, _float fSpeed, _float fTimeDelta) PURE;
	virtual void SetUp_Lerp(Vec3 vLook, _float fSpeed, _float fTimeDelta) PURE;
	virtual void Move_Pos(Vec3 vTargetPos) PURE;


	virtual void Set_Up(Vec3 vNormal) PURE;

protected:
	Matrix					m_WorldMatrix;
	TRANSFORMDESC			m_TransformDesc;

public:
	virtual void Free() override;
};

END