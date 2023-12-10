#include "UseLock_Transform.h"


CUseLock_Transform::CUseLock_Transform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CTransform(pDevice, pContext)
{

}

CUseLock_Transform::CUseLock_Transform(const CUseLock_Transform & rhs)
	: CTransform(rhs)
{

}

Vec3 CUseLock_Transform::Get_State(STATE eState)
{
	READ_LOCK
	return Vec3(m_WorldMatrix.m[eState][0], m_WorldMatrix.m[eState][1], m_WorldMatrix.m[eState][2]);
}

Matrix CUseLock_Transform::Get_WorldMatrix()
{
	READ_LOCK
	return m_WorldMatrix;
}

Matrix CUseLock_Transform::Get_WorldMatrix_TP()
{
	READ_LOCK
	return m_WorldMatrix.Transpose();
}

Matrix CUseLock_Transform::Get_WorldMatrixInverse()
{
	READ_LOCK
	return m_WorldMatrix.Invert();
}

void CUseLock_Transform::Set_State(STATE eState, Vec3 vState)
{
	WRITE_LOCK
	m_WorldMatrix.m[eState][0] = vState.x;
	m_WorldMatrix.m[eState][1] = vState.y;
	m_WorldMatrix.m[eState][2] = vState.z;
}

void CUseLock_Transform::Set_WorldMatrix(Matrix matWorld)
{
	WRITE_LOCK
	m_WorldMatrix = matWorld;
}

HRESULT CUseLock_Transform::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUseLock_Transform::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CUseLock_Transform::Go_Straight(_float fSpeed, _float fTimeDelta)
{
	Vec3 vPosition = Get_State(CUseLock_Transform::STATE_POSITION);
	Vec3 vLook = Get_State(CUseLock_Transform::STATE_LOOK);
	vLook.Normalize();

	vPosition += vLook * fSpeed * fTimeDelta;

	Set_State(CUseLock_Transform::STATE_POSITION, vPosition);
}

void CUseLock_Transform::Go_Backward(_float fSpeed, _float fTimeDelta)
{
	Vec3 vPosition = Get_State(CUseLock_Transform::STATE_POSITION);
	Vec3 vLook = Get_State(CUseLock_Transform::STATE_LOOK);
	vLook.Normalize();

	vPosition -= vLook * fSpeed * fTimeDelta;

	Set_State(CUseLock_Transform::STATE_POSITION, vPosition);
}

void CUseLock_Transform::Go_Left(_float fSpeed, _float fTimeDelta)
{
	Vec3 vPosition = Get_State(CUseLock_Transform::STATE_POSITION);
	Vec3 vLook = Get_State(CUseLock_Transform::STATE_RIGHT);
	vLook.Normalize();

	vPosition -= vLook * fSpeed * fTimeDelta;

	Set_State(CUseLock_Transform::STATE_POSITION, vPosition);
}

void CUseLock_Transform::Go_Right(_float fSpeed, _float fTimeDelta)
{
	Vec3 vPosition = Get_State(CUseLock_Transform::STATE_POSITION);
	Vec3 vLook = Get_State(CUseLock_Transform::STATE_RIGHT);
	vLook.Normalize();

	vPosition += vLook * fSpeed * fTimeDelta;

	Set_State(CUseLock_Transform::STATE_POSITION, vPosition);
}

void CUseLock_Transform::Go_Up(_float fTimeDelta)
{
	Vec3 vPosition = Get_State(CUseLock_Transform::STATE_POSITION);
	Vec3 vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	vUp.Normalize();

	vPosition += vUp * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CUseLock_Transform::STATE_POSITION, vPosition);
}

void CUseLock_Transform::Go_Down(_float fTimeDelta)
{
	Vec3 vPosition = Get_State(CUseLock_Transform::STATE_POSITION);
	Vec3 vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	vUp.Normalize();

	vPosition -= vUp * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CUseLock_Transform::STATE_POSITION, vPosition);
}

void CUseLock_Transform::Set_Scale(Vec3 vScaleInfo)
{
	Vec3 vRight = Get_State(CUseLock_Transform::STATE_RIGHT);
	vRight.Normalize();

	Vec3 vUp = Get_State(CUseLock_Transform::STATE_UP);
	vUp.Normalize();

	Vec3 vLook = Get_State(CUseLock_Transform::STATE_LOOK);
	vLook.Normalize();


	WRITE_LOCK
	Set_State(CUseLock_Transform::STATE_RIGHT, vRight * vScaleInfo.x);
	Set_State(CUseLock_Transform::STATE_UP, vUp * vScaleInfo.y);
	Set_State(CUseLock_Transform::STATE_LOOK, vLook * vScaleInfo.z);
}

Vec3 CUseLock_Transform::Get_Scale()
{
	READ_LOCK
	return Vec3(Get_State(CUseLock_Transform::STATE_RIGHT).Length(), Get_State(CUseLock_Transform::STATE_UP).Length(), Get_State(CUseLock_Transform::STATE_LOOK).Length());
}

void CUseLock_Transform::Turn(Vec3 vAxis, _float fTimeDelta)
{
	Quaternion vRot = Quaternion::CreateFromAxisAngle(vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);
	Matrix matRot = Matrix::CreateFromQuaternion(vRot);
	
	Matrix matWorld = m_WorldMatrix * matRot;


	WRITE_LOCK
	Set_State(CUseLock_Transform::STATE_RIGHT, Vec3(matWorld.m[0]));
	Set_State(CUseLock_Transform::STATE_UP, Vec3(matWorld.m[1]));
	Set_State(CUseLock_Transform::STATE_LOOK, Vec3(matWorld.m[2]));
}

void CUseLock_Transform::Rotation(Vec3 vAxis, _float fRadian)
{
	// Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(60.0f));

	Matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	RotationMatrix.CreateFromQuaternion(Quaternion::CreateFromAxisAngle(vAxis, fRadian));
	Vec3 vScale = Get_Scale();

	Vec3 vRight(RotationMatrix.m[0]);
	Vec3 vUp(RotationMatrix.m[1]);
	Vec3 vLook(RotationMatrix.m[2]);

	vRight.Normalize();
	vUp.Normalize();
	vLook.Normalize();


	WRITE_LOCK
	Set_State(CUseLock_Transform::STATE_RIGHT, vRight  * vScale.x);
	Set_State(CUseLock_Transform::STATE_RIGHT, vUp * vScale.y);
	Set_State(CUseLock_Transform::STATE_RIGHT, vLook * vScale.z);
}

void CUseLock_Transform::LookAt(Vec3 vAt)
{
	Vec3		vLook = vAt - Get_State(CUseLock_Transform::STATE_POSITION);

	Vec3		vRight = Vec3(0.0f, 1.0f, 0.0f).Cross(vLook);

	Vec3		vUp = vLook.Cross(vRight);

	Vec3		vScale = Get_Scale();


	WRITE_LOCK
	Set_State(CUseLock_Transform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CUseLock_Transform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CUseLock_Transform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CUseLock_Transform::LookAt_ForLandObject(Vec3 vAt)
{
	Vec3 vLook = vAt - Get_State(CUseLock_Transform::STATE_POSITION);

	Vec3 vUp(0.0f, 1.0f, 0.0f);

	Vec3 vScale = Get_Scale();


	Vec3 vRight = vUp.Cross(vLook);
	vRight.Normalize();
	vRight *= vScale.x;

	vLook = vRight.Cross(vUp);
	vLook.Normalize();
	vLook *= vScale.z;

	vUp *= vScale.y;


	WRITE_LOCK
	Set_State(CUseLock_Transform::STATE_RIGHT, vRight);
	Set_State(CUseLock_Transform::STATE_LOOK, vLook);
	Set_State(CUseLock_Transform::STATE_UP, vUp);
}

void CUseLock_Transform::Move(Vec3 vTargetPos, _float fTimeDelta, _float fLimitDistance)
{
	Vec3		vPosition = Get_State(CUseLock_Transform::STATE_POSITION);
	Vec3		vDirection = vTargetPos - vPosition;

	_float		fDistance = vDirection.Length();

	vDirection.Normalize();

	if(fDistance > fLimitDistance)
		vPosition += vDirection * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CUseLock_Transform::STATE_POSITION, vPosition);
}

CUseLock_Transform * CUseLock_Transform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUseLock_Transform*			pInstance = new CUseLock_Transform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CUseLock_Transform::Clone(CGameObject* pObject, void * pArg)
{
	CUseLock_Transform*			pInstance = new CUseLock_Transform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CUseLock_Transform::Free()
{
	__super::Free();
}

void CUseLock_Transform::Move_Dir(Vec3 vDir, _float fTimeDelta)
{
	vDir.Normalize();
	Vec3 vPosition = Get_State(CUseLock_Transform::STATE_POSITION);
	vPosition += vDir * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CUseLock_Transform::STATE_POSITION, vPosition);
}

void CUseLock_Transform::LookAt_Lerp(Vec3 vAt, _float fSpeed, _float fTimeDelta)
{
	Vec3 vPlayerLook = Get_State(CUseLock_Transform::STATE_LOOK);
	vPlayerLook.Normalize();

	Vec3 vTargetLook = vAt;
	vTargetLook.Normalize();

	Vec3 vUp = Get_State(CUseLock_Transform::STATE_UP);
	vUp.Normalize();


	Vec3 vTargetRight = vUp.Cross(vTargetLook);
	vTargetLook = vTargetRight.Cross(vUp);
	vTargetLook.Normalize();

	_float fRadian = acosf(min(1.0f, vTargetLook.Dot(vPlayerLook)));
	if (fRadian <= fSpeed * fTimeDelta)
	{
		vPlayerLook = vTargetLook;

		Vec3 vRight = vUp.Cross(vPlayerLook);
		vRight.Normalize();

		vPlayerLook = vRight.Cross(vUp);
		vPlayerLook.Normalize();

		vUp = vPlayerLook.Cross(vRight);
		vUp.Normalize();

		Vec3 vScale = Get_Scale();

		vRight *= vScale.x;
		vUp *= vScale.y;
		vPlayerLook *= vScale.z;

		WRITE_LOCK
		Set_State(STATE::STATE_RIGHT, vRight);
		Set_State(STATE::STATE_UP, vUp);
		Set_State(STATE::STATE_LOOK, vPlayerLook);

		return;
	}
	
	Vec3 vPlayerUp = Get_State(CUseLock_Transform::STATE_UP);
	Vec3 vCrossUp = vPlayerLook.Cross(vTargetLook);

	if (vPlayerUp.Dot(vCrossUp) >= 0)
	{
		Turn_Speed(vUp, fSpeed, fTimeDelta);
	}
	else
	{
		Turn_Speed(vUp, -fSpeed, fTimeDelta);
	}
}

void CUseLock_Transform::LookAt_Lerp_ForLand(Vec3 vAt, _float fSpeed, _float fTimeDelta)
{
	Vec3 vPlayerLook = Get_State(CUseLock_Transform::STATE_LOOK);
	vPlayerLook.y = 0.0f;
	vPlayerLook.Normalize();

	Vec3 vTargetLook = vAt;
	vTargetLook.y = 0.0f;
	vTargetLook.Normalize();

	Vec3 vUp = Get_State(CUseLock_Transform::STATE_UP);
	vUp.Normalize();

	Vec3 vSkyUp(0.0f, 1.0f, 0.0f);


	_float fRadian = acosf(min(1.0f, vTargetLook.Dot(vPlayerLook)));
	if (fRadian <= fSpeed * fTimeDelta)
	{
		vPlayerLook = vTargetLook;

		Vec3 vRight = vSkyUp.Cross(vPlayerLook);
		vRight.Normalize();


		vPlayerLook = vRight.Cross(vUp);
		vPlayerLook.Normalize();

		Vec3 vScale = Get_Scale();

		vRight *= vScale.x;
		vUp *= vScale.y;
		vPlayerLook *= vScale.z;

		WRITE_LOCK
		Set_State(STATE::STATE_RIGHT, vRight);
		Set_State(STATE::STATE_UP, vUp);
		Set_State(STATE::STATE_LOOK, vPlayerLook);

		return;
	}

	Vec3 vPlayerUp = Get_State(CUseLock_Transform::STATE_UP);
	Vec3 vCrossUp = vPlayerLook.Cross(vTargetLook);

	if (vPlayerUp.Dot(vCrossUp) >= 0)
	{
		Turn_Speed(vSkyUp, fSpeed, fTimeDelta);
	}
	else
	{
		Turn_Speed(vSkyUp, -fSpeed, fTimeDelta);
	}
}

void CUseLock_Transform::SetUp_Lerp(Vec3 vLook, _float fSpeed, _float fTimeDelta)
{
	Vec3 vPlayerLook = Get_State(CUseLock_Transform::STATE_LOOK);
	vPlayerLook.Normalize();


	Vec3 vUp = Get_State(CUseLock_Transform::STATE_UP);
	vUp.Normalize();

	vLook.Normalize();


	_float fDot = vUp.Dot(vLook);

	Vec3 vTargetLook;

	if (fDot == 0.0f)
	{
		_float fLookDot = vPlayerLook.Dot(vLook);
		vTargetLook = vPlayerLook * fLookDot;
	}
	else
	{
		_float fLookDot = vPlayerLook.Dot(vLook);
		vTargetLook = vPlayerLook * fLookDot + vUp * fDot;
	}

	vTargetLook.Normalize();


	Vec3 vRight = Get_State(CUseLock_Transform::STATE_RIGHT);
	vRight.Normalize();

	
	_float fRadian = acosf(min(1.0f, vTargetLook.Dot(vPlayerLook)));
	if (fRadian <= fSpeed * fTimeDelta)
	{
		vPlayerLook = vTargetLook;

		Vec3 vUp = vPlayerLook.Cross(vRight);
		vUp.Normalize();

		Vec3 vScale = Get_Scale();

		vRight *= vScale.x;
		vUp *= vScale.y;
		vPlayerLook *= vScale.z;

		WRITE_LOCK
		Set_State(STATE::STATE_RIGHT, vRight);
		Set_State(STATE::STATE_UP, vUp);
		Set_State(STATE::STATE_LOOK, vPlayerLook);

		return;
	}

	Vec3 vPlayerRight = Get_State(CUseLock_Transform::STATE_RIGHT);
	Vec3 vCrossRight = vPlayerLook.Cross(vTargetLook);

	if (vPlayerRight.Dot(vCrossRight) >= 0)
	{
		Turn_Speed(vRight, fSpeed, fTimeDelta);
	}
	else
	{
		Turn_Speed(vRight, -fSpeed, fTimeDelta);
	}
}

void CUseLock_Transform::Move_Pos(Vec3 vTargetPos)
{
	Vec3 vPos;
	{
		READ_LOCK
		vPos = Vec3(m_WorldMatrix.m[3]);
	}
	
	vPos += vTargetPos;
	Set_State(STATE::STATE_POSITION, vPos);
}

void CUseLock_Transform::Set_Up(Vec3 vNormal)
{
	Vec3 vLook = Get_State(STATE::STATE_LOOK);
	vLook.Normalize();

	Vec3 vUp = vNormal;

	Vec3 vRight = vUp.Cross(vLook);
	vLook = vRight.Cross(vUp);

	vRight.Normalize();
	vUp.Normalize();
	vLook.Normalize();

	Vec3 vScale = Get_Scale();

	vRight *= vScale.x;
	vUp *= vScale.y;
	vLook *= vScale.z;

	WRITE_LOCK
	Set_State(CUseLock_Transform::STATE_RIGHT, vRight);
	Set_State(CUseLock_Transform::STATE_UP, vUp);
	Set_State(CUseLock_Transform::STATE_LOOK, vLook);

}

void CUseLock_Transform::Turn_Speed(Vec3 vAxis, _float fSpeed, _float fTimeDelta)
{
	Quaternion vRot = Quaternion::CreateFromAxisAngle(vAxis, fSpeed * fTimeDelta);
	Matrix matRot = Matrix::CreateFromQuaternion(vRot);

	Matrix matWorld = m_WorldMatrix * matRot;


	WRITE_LOCK
	Set_State(CUseLock_Transform::STATE_RIGHT, Vec3(matWorld.m[0]));
	Set_State(CUseLock_Transform::STATE_UP, Vec3(matWorld.m[1]));
	Set_State(CUseLock_Transform::STATE_LOOK, Vec3(matWorld.m[2]));
}

void CUseLock_Transform::LookAt_Dir(Vec3 vDir)
{
	Vec3		vUp = Get_State(STATE::STATE_UP);
	Vec3		vLook = vDir;
	Vec3		vRight = vUp.Cross(vLook);


	vLook = vRight.Cross(vUp);

	vUp.Normalize();
	vLook.Normalize();
	vRight.Normalize();


	Vec3		vScale = Get_Scale();


	WRITE_LOCK
	Set_State(CUseLock_Transform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CUseLock_Transform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CUseLock_Transform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CUseLock_Transform::LookAt_SetUp(Vec3 vAt)
{
	Vec3		vUp = Get_State(STATE::STATE_UP);
	Vec3		vLook = vAt - Get_State(CUseLock_Transform::STATE_POSITION);

	Vec3		vRight = vUp.Cross(vLook);

	vUp = vLook.Cross(vRight);

	Vec3		vScale = Get_Scale();


	Set_State(CUseLock_Transform::STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(CUseLock_Transform::STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(CUseLock_Transform::STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}
