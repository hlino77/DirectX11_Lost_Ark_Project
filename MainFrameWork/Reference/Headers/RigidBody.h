#pragma once
#include "Component.h"

BEGIN(Engine)

enum class ForceMode
{
	FORCE,
	IMPULSE,
	VELOCITY_CHANGE,
	ACCELERATION
};

enum class Axis
{
	X,
	Y,
	Z,
};

class ENGINE_DLL CRigidBody : public CComponent
{
private:
	CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRigidBody(const CRigidBody& rhs);
	virtual ~CRigidBody() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;

public:

#pragma region Property
	_bool	IsActive() { return m_bActive; }
	void	Set_Active(_bool bActive) { m_bActive = bActive; }

	_bool	IsPhysX() { return m_bPhysX; }
	void	Set_PhysX(_bool bPhysX) { m_bPhysX = bPhysX; }

	_bool	UseGravity()	const						{ return m_UseGravity; }
	void	UseGravity(_bool UseGravity);

	void	Set_Gravity(_bool bGravity) { m_UseGravity = bGravity; }

	_bool	IsKinematic()	const						{ return m_IsKinematic; }
	void	IsKinematic(_bool IsKinematic)				{ m_IsKinematic = IsKinematic; }

	void	UseDrag(_bool bDrag) { m_bDrag = bDrag; }
	_bool	IsDrag() { return m_bDrag; }


	_bool	Is_Gruond() { return m_bGround; }
	void	Set_Ground(_bool bGround) { m_bGround = bGround; }

	void	SetCompareGruond(_bool bCompare);
	_bool	IsCompareGruond() { return m_bCompareGround; }


	_float	GetMass()	const							{ return m_fMass; }
	void	SetMass(_float fMass)						{ m_fMass = fMass; }

	void	Set_GruondDrag(_float fDrag)				{ m_fGroundDrag = fDrag; }
	_float	Get_GruondDrag()							{ return m_fGroundDrag; }



	_float	GetDrag()	const							{ return m_fDrag; }
	_float	GetAngularDrag()	const					{ return m_fAngularDrag; }

	_bool	IsFrozePosition(Axis eAxis)					{ return m_byConstraints & 1 << (_int)eAxis; }
	void	FreezePosition(Axis eAxis)					{ m_byConstraints ^= 1 << (_int)eAxis; }		// Switch On/Off
	_bool	IsFrozeRotation(Axis eAxis)					{ return m_byConstraints & 1 << ((_int)eAxis + 3); }
	void	FreezeRotation(Axis eAxis)					{ m_byConstraints ^= 1 << ((_int)eAxis + 3); }	// Switch On/Off


	Vec3	Get_GravityDir()							{ return m_vGravityDir; }
	void	Set_GravityDir(Vec3 vDir)					{ m_vGravityDir = vDir; m_vGravityDir.Normalize(); }

	// Velocity
	Vec3	GetLinearVelocity()	const					{ return m_vLinearVelocity; }
	void	SetLinearVelocity(const Vec3& vLinearVelocity);
	_float	GetLinearAxisVelocity(Axis eAxis)	const	{ return *((_float*)&m_vLinearVelocity + (_int)eAxis); }
	void	SetLinearAxisVelocity(Axis eAxis, _float fVelocity)
														{ *((_float*)&m_vLinearVelocity + (_int)eAxis) = fVelocity; }

	Vec3	GetAngularVelocity()						{ return m_vAngularVelocity; }
	void	SetAngularVelocity(const Vec3& vAngularVelocity);
	_float	GetAngularAxisVelocity(Axis eAxis)	const	{ return *((_float*)&m_vAngularVelocity + (_int)eAxis); }
	void	SetAngularAxisVelocity(Axis eAxis, _float fVelocity)
														{ *((_float*)&m_vAngularVelocity + (_int)eAxis) = fVelocity; }

	// Force/Torque modifiers
	void	AddForce(const Vec3& vForce, ForceMode eMode);
	void	AddTorque(const Vec3& vTorque, ForceMode eMode);
	void	SetForceAndTorque(const Vec3& vForce, const Vec3& vTorque, ForceMode eMode);

	void	ClearForce(ForceMode eMode);
	void	ClearTorque(ForceMode eMode);
	void	ClearNetPower();

	void	Sleep() { ClearNetPower(); m_IsSleeping = true; }
	void	WakeUp() { m_IsSleeping = false; }
#pragma endregion Property

private:
	void	KineticUpdate(const _float& fTimeDelta);
	void	KinematicUpdate(const _float& fTimeDelta);

	void	UpdateTransform(const _float& fTimeDelta);
	void	UpdateGround(const _float& fTimeDelta);


private:
	_bool	m_IsSleeping;
	const _float m_fSleepThreshold;

	// Description
	_bool	m_UseGravity;
	_bool	m_IsKinematic;
	_bool	m_bGround = true;
	_bool	m_bCompareGround = true;
	_bool	m_bDrag = true;
	_bool	m_bActive = true;
	_bool	m_bPhysX = false;

	_float	m_fMass;
	_float	m_fDrag;
	_float	m_fGroundDrag;
	_float	m_fAngularDrag;

	// Constraints
	_byte	m_byConstraints;

	Vec3	m_vLinearAcceleration;
	Vec3	m_vAngularAcceleration;

	Vec3	m_vLinearVelocity;
	Vec3	m_vAngularVelocity;

	Vec3	m_vGravityDir;
public:
	static CRigidBody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free()						override;
};

END