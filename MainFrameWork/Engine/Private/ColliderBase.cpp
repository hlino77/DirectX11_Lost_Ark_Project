#include "Engine_Defines.h"
#include "ColliderBase.h"
#include "GameObject.h"
#include "PipeLine.h"


	atomic<_uint> CCollider::g_iNextID = 0;

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ColliderType eColliderType)
	: Super(pDevice, pContext)
	, m_iID(g_iNextID++)
	, m_eColliderType(eColliderType)
	, m_IsTrigger(false)
	, m_vOffset(Vec3(0.0f, 0.0f, 0.0f))
	//, m_iCol(0)
{
}

CCollider::CCollider(const CCollider& rhs)
	: Super(rhs)
	, m_iID(g_iNextID++)
	, m_eColliderType(rhs.m_eColliderType)
	, m_IsTrigger(rhs.m_IsTrigger)
	//, m_iCol(rhs.m_iCol)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif
}

HRESULT CCollider::Initialize_Prototype()
{
#ifdef _DEBUG
	if (m_pDevice && m_pContext)
	{
		m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);

		m_pEffect = new BasicEffect(m_pDevice);
		m_pEffect->SetVertexColorEnabled(true);

		const void* pShaderByteCodes = nullptr;
		size_t		iLength = 0;
		m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

		if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		{
			Safe_Delete(m_pBatch);
			Safe_Delete(m_pEffect);
			Safe_Release(m_pInputLayout);
			return E_FAIL;
		}
	}
#endif

	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{


	return S_OK;
}

void CCollider::DebugRender()
{
#ifdef _DEBUG
	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine* pPipeLine = CPipeLine::GetInstance();
	pPipeLine->AddRef();

	m_pEffect->SetView(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

	Safe_Release(pPipeLine);

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);
#endif // DEBUG
}

void CCollider::OnCollisionEnter(CCollider* pOther)
{
	m_pOwner->OnCollisionEnter(m_iColLayer, pOther);
}

void CCollider::OnCollisionExit(CCollider* pOther)
{
	m_pOwner->OnCollisionExit(m_iColLayer, pOther);
}

void CCollider::Set_AttackCollider(_uint iAttack, _uint iAttackType, _bool bSlow)
{
	m_iAttack = iAttack;
	m_iAttackType = iAttackType;
	m_bSlowMotion = bSlow;
}

void CCollider::Reset_Attack()
{
	m_iAttackType = 0;
	m_iAttack = 0;
	m_bSlowMotion = false;
}

void CCollider::Free()
{
	Super::Free();
#ifdef _DEBUG
	if (false == m_isCloned)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}

	Safe_Release(m_pInputLayout);

#endif // _DEBUG
}
