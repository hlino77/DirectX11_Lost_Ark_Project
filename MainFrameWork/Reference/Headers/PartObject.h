#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : public CGameObject
{
public:
	enum class PARTS { WEAPON_1, WEAPON_2, WEAPON_3, WEAPON_4, WEAPON_5, HAIR, HEAD, _END };

public:
	typedef struct tagPartDesc
	{
		CGameObject*		pOwner = { nullptr };
		PARTS				ePart = { PARTS::_END };
		class CTransform*	pParentTransform = { nullptr };
		class CModel*		pPartenModel = { nullptr };
		_uint				iSocketBoneIndex = { 0 };
		_uint				iStoreSocketBoneIndex = { 0 };
		Matrix				SocketPivotMatrix;

		Matrix				OffsetMatrix;
		wstring				strModel;

	}PART_DESC;

protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjectTag, _int iObjType);
	CPartObject(const CPartObject& rhs);
	virtual ~CPartObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render()					{ return S_OK; }
	virtual HRESULT			Render_ShadowDepth();

public:
	CGameObject*			Get_PartOwner()				 { return m_pOwner; }
	PARTS					Get_Part_Index()       const { return m_ePart; }
	const Matrix&			Get_Part_WorldMatrix() const { return m_WorldMatrix; }

public:
	virtual void			Store_Socket();
	virtual void			UnStore_Socket();

public:
	void					Set_RenderState(_bool IsRender) { m_IsRender = IsRender; }

	void					Change_ModelCom(class CModel* pModel) { m_pModelCom = pModel; }
	void					Change_ParentModelCom(class CModel* pModel) { m_pParentModel = pModel; }
	void					Change_Pivot(Matrix matPivot) { m_SocketPivotMatrix = matPivot; }
	void					Change_BoneIndex(_uint	iBoneIndex) { m_iSocketBoneIndex = iBoneIndex; }

	void					Set_UpdateState(_bool bUpdate) { m_bStopUpdate = bUpdate; }

protected:
	class CTransform*		m_pParentTransform = { nullptr };
	class CModel*			m_pParentModel = { nullptr };

protected:
	CGameObject*			m_pOwner = { nullptr };
	PARTS					m_ePart = { PARTS::_END };

	Vec3					m_StoreSocketPos;
	Matrix					m_StoreSocketMatrix;
	Matrix					m_SocketPivotMatrix;
	Matrix					m_WorldMatrix;

	Vec4					m_vAnimTargetPos;
	_uint					m_iSocketBoneIndex;
	_uint					m_iStoreSocketBoneIndex;

	_bool					m_IsRender = { true };
	_bool					m_IsStored = { false };
	_bool					m_bStopUpdate = { false };

	Matrix					m_OffSetMatrix;
	wstring					m_strPartModel;

protected:
	HRESULT Compute_RenderMatrix(Matrix ChildMatrix);

public:
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END