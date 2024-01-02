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

	virtual void	Add_InstanceData(_uint iSize, _uint& iIndex) override;
public:
	CShader* Get_ShaderCom() { return m_pShaderCom; }
	CTransform* Get_TransformCom() { return m_pTransformCom; }
	CModel* Get_ModelCom() { return m_pModelCom; }



	HRESULT Add_ModelComponent(const wstring& strComName);


	void	Add_Collider();
	void	Add_ChildCollider(_uint iIndex);

	vector<CSphereCollider*>& Get_StaticColliders() { return m_StaticColliders; }

	CSphereCollider* Get_StaticCollider(_uint iIndex) { return m_StaticColliders[iIndex]; }

protected:
	virtual HRESULT Ready_Components() override;


	virtual HRESULT	Ready_Proto_InstanceBuffer() override;
	virtual HRESULT	Ready_Instance_For_Render(_uint iSize) override;

private: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */

private:
	CRenderer::RENDERGROUP m_eRenderGroup;
	vector<CSphereCollider*> m_StaticColliders;

private:

public:
	static CStaticModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END

