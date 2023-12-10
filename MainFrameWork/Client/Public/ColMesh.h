#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "ColliderSphere.h"


BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
END


BEGIN(Client)
class CColMesh final : public CGameObject
{
public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		_uint	iLayer;
	}MODELDESC;

private:
	CColMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType);
	CColMesh(const CColMesh& rhs);
	virtual ~CColMesh() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

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

private: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;

private:
	vector<CSphereCollider*> m_StaticColliders;


private:

public:
	static CColMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END

