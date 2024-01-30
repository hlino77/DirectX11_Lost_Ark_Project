#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CEsther_Part_forCut final : public CPartObject
{
public:
	struct  GlobalDesc
	{
		Matrix ViewMatrix;// = Matrix::Identity;
		Matrix ProjMatrix;// = Matrix::Identity;
		Matrix ViewProj;// = Matrix::Identity;
		Matrix ViewInverse;// = Matrix::Identity;
	};

protected:
	CEsther_Part_forCut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Part_forCut(const CEsther_Part_forCut& rhs);
	virtual ~CEsther_Part_forCut() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	virtual void	Set_EffectPos() override;

public:
	static CEsther_Part_forCut* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END