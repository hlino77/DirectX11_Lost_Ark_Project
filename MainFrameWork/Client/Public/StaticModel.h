#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Renderer.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CTransform;
class CPipeLine;
class CSphereCollider;
END


BEGIN(Client)
class CStaticModel final : public CGameObject
{
public:
	typedef struct ModelDesc
	{
		wstring strFilePath;
		wstring strFileName;
		Vec3	vPosition;
		
		Vec3	BloomColor;

		_bool	IsMapObject = false;
		_uint	iLayer;
		_bool	bInstance = false;
	}MODELDESC;

public:
	enum STATE { STATE_IDLE, STATE_WALK, STATE_RUN, STATE_JUMP, STATE_END };
	enum PARTTYPE { PART_WEAPON, PART_END };

private:
	CStaticModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType);
	CStaticModel(const CStaticModel& rhs);
	virtual ~CStaticModel() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
	virtual HRESULT	Render_Instance(_uint iSize);
	virtual HRESULT	Render_Debug();

	virtual void	Add_InstanceData(_uint iSize, _uint& iIndex) override;
public:
	CShader* Get_ShaderCom() { return m_pShaderCom; }
	CTransform* Get_TransformCom() { return m_pTransformCom; }
	CModel* Get_ModelCom() { return m_pModelCom; }



	HRESULT Add_ModelComponent(const wstring& strComName);

	virtual	void OnCollisionEnter(const _uint iColLayer, CCollider* pOther);

	void Disable_NaviCells();


	void	Add_Collider();
	void	Add_ChildCollider(_uint iIndex);

	void Add_CollidersToManager();

	vector<CSphereCollider*>& Get_StaticColliders() { return m_StaticColliders; }

	CSphereCollider* Get_StaticCollider(_uint iIndex) { return m_StaticColliders[iIndex]; }

	void Add_NaviCellIndex(_uint CellIndex) { m_NaviCellIndex.push_back(CellIndex); }

	void Send_Collision(_uint iLevel, _bool bActive);
	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }
	_bool					Get_RimLight() { return m_bRimLight; }



protected:

	virtual HRESULT Ready_Components() override;
	virtual HRESULT	Ready_Proto_InstanceBuffer() override;
	virtual HRESULT	Ready_Instance_For_Render(_uint iSize) override;


private:

	CRenderer::RENDERGROUP			m_eRenderGroup = { CRenderer::RENDERGROUP::RENDER_END };

	vector<CSphereCollider*>		m_StaticColliders;
	vector<_uint>					m_NaviCellIndex;

	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

	Vec3							m_BloomColor = {};

public:
	static CStaticModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END

