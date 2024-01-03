#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CPipeLine;
END

BEGIN(Client)

class CMannequin final : public CGameObject
{
public:
	enum STATE { STATE_IDLE, STATE_WALK, STATE_RUN, STATE_JUMP, STATE_END };
	enum PARTTYPE { PART_R, PART_L, PART_END };

private:
	CMannequin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMannequin(const CMannequin& rhs);
	virtual ~CMannequin() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	CShader*	Get_ShaderCom() { return m_pShaderCom; }
	CTransform* Get_TransformCom() { return m_pTransformCom; }
	CModel*		Get_ModelCom() { return m_pModelCom; }

public:
	void		Set_ModelCom(CModel* pModel);
	void		Set_Part(_uint PartType, CModel* pModel, Matrix LocalMatrix);

public:
	void		Set_AnimationPlay() { m_bAnimationPlay = !m_bAnimationPlay; }

private:
	virtual HRESULT Ready_Components() override;
	HRESULT			Ready_PlayerPart();

private: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CGameObject*	m_pPart[PART_END] = { nullptr };

private:
	_bool			m_bAnimationPlay = true;

	_bool			m_bRimLight = false;
	_float			m_fRimLightTime = 0.0f;

	std::future<HRESULT>			m_PlayAnimation;

public:
	static CMannequin* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

