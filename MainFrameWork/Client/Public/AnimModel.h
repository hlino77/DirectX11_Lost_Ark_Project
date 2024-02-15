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
class CAnimModel final : public CGameObject
{
public:
	typedef struct ModelDesc
	{
		wstring strFilePath = L"";
		wstring strFileName = L"";
		Vec3	vPosition = {};
		
		_bool	IsMapObject = false;
		_uint	iLayer = {};
		_bool	bInstance = false;
	}MODELDESC;


private:
	CAnimModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType);
	CAnimModel(const CAnimModel& rhs);
	virtual ~CAnimModel() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
	virtual HRESULT	Render_Instance(_uint iSize);

	virtual void	Add_InstanceData(_uint iSize, _uint& iIndex) override;
	virtual HRESULT	Render_CascadeShadowDepth(_uint iIndex);

public:
	CShader* Get_ShaderCom() { return m_pShaderCom; }
	CTransform* Get_TransformCom() { return m_pTransformCom; }
	CModel* Get_ModelCom() { return m_pModelCom; }



	HRESULT Add_ModelComponent(const wstring& strComName);


	void	Add_Collider();
	void	Add_ChildCollider(_uint iIndex);

	vector<CSphereCollider*>& Get_StaticColliders() { return m_StaticColliders; }

	CSphereCollider*	Get_StaticCollider(_uint iIndex) { return m_StaticColliders[iIndex]; }

	void				Set_PlayAnim(_bool bPlayAnim) { m_bPlayAnim = bPlayAnim; }
	void				Break_Floor();
	void				Break_OutWall();

protected:
	virtual HRESULT Ready_Components() override;


	virtual HRESULT	Ready_Proto_InstanceBuffer() override;
	virtual HRESULT	Ready_Instance_For_Render(_uint iSize) override;

private:
	void	Creat_NaviCellIndex(_uint MinIndex, _uint MaxIndex);
	void    Creat_NaviCellIndex_ExceptObject();
	void    Send_Collision(_uint iLevel, _bool bActive);

private:
	CRenderer::RENDERGROUP m_eRenderGroup = { CRenderer::RENDERGROUP::RENDER_END };
	vector<CSphereCollider*> m_StaticColliders;

private:
	_bool m_bPlayAnim = false;

	std::future<HRESULT>			m_PlayAnimation;
	_float							m_fAnimationSpeed = 1.0f;
	_float							m_fRootTargetDistance = 0.f;

	vector<_uint>					m_NaviCellIndex;

public:
	static CAnimModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;


};

END

