#include "stdafx.h"
#include "GameInstance.h"
#include "AnimModel.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"

#include "NavigationMgr.h"
#include "ServerSessionManager.h"


CAnimModel::CAnimModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType)
	: CGameObject(pDevice, pContext, L"AnimModel", eObjType)
{
}

CAnimModel::CAnimModel(const CAnimModel& rhs)
	: CGameObject(rhs)
{
}

HRESULT CAnimModel::Initialize_Prototype()
{
	__super::Initialize_Prototype();


    return S_OK;
}

HRESULT CAnimModel::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_szModelName = Desc->strFileName;
	m_iLayer = Desc->iLayer;
	m_IsMapObject = Desc->IsMapObject;
	m_bInstance = Desc->bInstance;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPosition);



	m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;

	if (m_bInstance)
	{
		if (m_pInstaceData->find(m_szModelName) == m_pInstaceData->end())
		{
			if (FAILED(Ready_Proto_InstanceBuffer()))
				return E_FAIL;
		}
	}
	
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(Desc->vPosition.x, Desc->vPosition.y, Desc->vPosition.z, 1.f));
	//m_pTransformCom->My_Rotation(Vec3(90.f,180.f, 0.f));

	m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;


	m_pModelCom->Set_CurrAnim(0);

	m_pModelCom->Play_Animation(0.0f);


	if (m_szModelName == TEXT("Floor_All_R01"))
	{
		_uint MinCellIndex = 362;
		_uint MaxCellIndex = 614;

		Creat_NaviCellIndex(MinCellIndex, MaxCellIndex);
		m_NaviCellIndex.push_back(926); // Except
	}

	if (m_szModelName == TEXT("Floor_All_L01"))
	{
		_uint MinCellIndex = 615;
		_uint MaxCellIndex = 925;

		Creat_NaviCellIndex(MinCellIndex, MaxCellIndex);
		
	}



    return S_OK;
}

void CAnimModel::Tick(_float fTimeDelta)
{

	if (m_szModelName == TEXT("Chain"))
	{
		m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
	}

	//// All Object Animation Play
	if (KEY_HOLD(KEY::CTRL) &&  KEY_AWAY(KEY::J))
	{
		m_bPlayAnim = !m_bPlayAnim;
	}

	if (true == m_bPlayAnim)
	{
		//m_pModelCom->Play_Animation(fTimeDelta);
		m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
	}

	// All Object Animation First Frame
	if (KEY_HOLD(KEY::CTRL) && KEY_AWAY(KEY::K))
	{
		m_pModelCom->Set_CurrAnimFrame(0);
		m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed); 
		m_bPlayAnim = false;
		
	}

	// Wall Object Animation Play
	if (KEY_HOLD(KEY::CTRL) && KEY_AWAY(KEY::L))
	{
		if (m_szModelName == TEXT("Wall01") ||
			m_szModelName == TEXT("Wall02") ||
			m_szModelName == TEXT("Wall03") ||
			m_szModelName == TEXT("Wall04"))
		{
			m_bPlayAnim = !m_bPlayAnim;
		}
	}

	// Left Ground Break
	if (KEY_HOLD(KEY::CTRL) && KEY_AWAY(KEY::M))
	{
		if (m_szModelName == TEXT("Floor_All_R01"))
		{
			m_bPlayAnim = !m_bPlayAnim;

			Send_Collision(LEVEL_VALTANMAIN, false);

			for (auto& CellIndex : m_NaviCellIndex)
			{
				CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, CellIndex, false);
			}

		}
	}

	// Right Ground Break
	if (KEY_HOLD(KEY::CTRL) && KEY_AWAY(KEY::N))
	{
		if (m_szModelName == TEXT("Floor_All_L01"))
		{
			m_bPlayAnim = !m_bPlayAnim;

			Send_Collision(LEVEL_VALTANMAIN, false);

			for (auto& CellIndex : m_NaviCellIndex)
			{
				CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, CellIndex, false);
			}
		}
	}
	
}

void CAnimModel::LateTick(_float fTimeDelta)
{

	if (nullptr == m_pRendererCom)	
		return;

	if (m_PlayAnimation.valid())
	{
		m_PlayAnimation.get();
		
	}
	m_pModelCom->Set_ToRootPos(m_pTransformCom);



	if (m_szModelName != TEXT("Chain"))
	{
		if (m_pModelCom->Get_Animations()[0]->Is_End() == true)
		{
			Set_Dead(true);
		}
	}


	if (m_bRender)
	{
		//m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;
		//if (m_bInstance)
		//	m_pRendererCom->Add_InstanceRenderGroup(m_eRenderGroup, this);
		//else
			m_pRendererCom->Add_RenderGroup(m_eRenderGroup, this);
	}

}




HRESULT CAnimModel::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

    return S_OK;
}

HRESULT CAnimModel::Render_ShadowDepth()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	m_pShaderCom->Push_StaticShadowWVP();

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);


	//_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	//for (_uint i = 0; i < iNumMeshes; ++i)
	//{
	//	if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
	//		return S_OK;

	//	/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
	//		return E_FAIL;*/


	//	if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 2)))
	//		return S_OK;
	//}

	return S_OK;
}

HRESULT CAnimModel::Render_Instance(_uint iSize)
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	{
		(*m_pInstaceData)[m_szModelName].Future_Instance.wait();
		if (FAILED((*m_pInstaceData)[m_szModelName].Future_Instance.get()))
			return E_FAIL;

		CGameInstance::GetInstance()->Execute_BeforeRenderCommandList((*m_pInstaceData)[m_szModelName].pInstanceContext);
		(*m_pInstaceData)[m_szModelName].pInstanceContext = nullptr;
	}

	if (FAILED((*m_pInstaceData)[m_szModelName].pInstanceShader->Push_GlobalVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render_Instance((*m_pInstaceData)[m_szModelName].pInstanceBuffer, iSize, (*m_pInstaceData)[m_szModelName].pInstanceShader, sizeof(Matrix))))
		return E_FAIL;


	return S_OK;
}

HRESULT CAnimModel::Add_ModelComponent(const wstring& strComName)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

void CAnimModel::Add_Collider()
{
	CSphereCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CSphereCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<CSphereCollider*>(pComponent);
	if (nullptr == pCollider)
		return;

	m_StaticColliders.push_back(pCollider);

	Safe_Release(pGameInstance);
}

void CAnimModel::Add_ChildCollider(_uint iIndex)
{
	COBBCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<COBBCollider*>(pComponent);
	if (nullptr == pCollider)
		return;


	m_StaticColliders[iIndex]->Set_Child(pCollider);

	Safe_Release(pGameInstance);
}

HRESULT CAnimModel::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	// Shader -> AnimModel
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	Safe_Release(pGameInstance);

	Vec3 vScale = {};
	vScale.x = 100.f;
	vScale.y = 100.f;
	vScale.z = 100.f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}

void CAnimModel::Add_InstanceData(_uint iSize, _uint& iIndex)
{
	vector<Matrix>* pInstanceValue = static_cast<vector<Matrix>*>(((*m_pInstaceData)[m_szModelName].pInstanceValue)->GetValue());

	(*pInstanceValue)[iIndex] = m_pTransformCom->Get_WorldMatrix();

	if (iSize - 1 == iIndex)
	{

		//m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);

		(*m_pInstaceData)[m_szModelName].Future_Instance = std::async(&CAnimModel::Ready_Instance_For_Render, this, iSize);

		//ThreadManager::GetInstance()->EnqueueJob([=]()
		//	{
		//		promise<HRESULT> PromiseInstance;
		//		m_pInstaceData->Future_Instance = PromiseInstance.get_future();
		//		PromiseInstance.set_value(Ready_Instance_For_Render(iSize));
		//	});
	}
	else
		++iIndex;
}



HRESULT CAnimModel::Ready_Proto_InstanceBuffer()
{
	(*m_pInstaceData)[m_szModelName].iMaxInstanceCount = 200;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_StaticModelInstace"),
		TEXT("Com_InstanceShader"), (CComponent**)&(*m_pInstaceData)[m_szModelName].pInstanceShader)))
		return E_FAIL;

	(*m_pInstaceData)[m_szModelName].pInstanceValue = new tagTypeLessData<vector<Matrix>>();

	vector<Matrix>* pInstanceValue = static_cast<vector<Matrix>*>(((*m_pInstaceData)[m_szModelName].pInstanceValue)->GetValue());
	
	pInstanceValue->resize((*m_pInstaceData)[m_szModelName].iMaxInstanceCount, XMMatrixIdentity());

	{
		D3D11_BUFFER_DESC			BufferDesc;

		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

		// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
		BufferDesc.ByteWidth = sizeof(Matrix) * ((*m_pInstaceData)[m_szModelName].iMaxInstanceCount);
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = sizeof(Matrix);

		D3D11_SUBRESOURCE_DATA		InitialData;

		InitialData.pSysMem = pInstanceValue->data();

		if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &InitialData, &(*m_pInstaceData)[m_szModelName].pInstanceBuffer)))
			return E_FAIL;
	}
}

HRESULT CAnimModel::Ready_Instance_For_Render(_uint iSize)
{
	vector<Matrix>* pInstanceValue = static_cast<vector<Matrix>*>(((*m_pInstaceData)[m_szModelName].pInstanceValue)->GetValue());

	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	(*m_pInstaceData)[m_szModelName].pInstanceContext = CGameInstance::GetInstance()->Get_BeforeRenderContext();

	if ((*m_pInstaceData)[m_szModelName].pInstanceContext == nullptr)
		return E_FAIL;

	if(FAILED((*m_pInstaceData)[m_szModelName].pInstanceContext->Map(((*m_pInstaceData)[m_szModelName].pInstanceBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
		return E_FAIL;

	memcpy(SubResource.pData, pInstanceValue->data(), sizeof(Matrix) * iSize);

	(*m_pInstaceData)[m_szModelName].pInstanceContext->Unmap((*m_pInstaceData)[m_szModelName].pInstanceBuffer, 0);

	return S_OK;
}

void CAnimModel::Creat_NaviCellIndex(_uint MinIndex, _uint MaxIndex)
{
	_uint MinCellIndex = MinIndex;
	_uint MaxCellIndex = MaxIndex;

	_uint CellIndexSize = MaxCellIndex - MinCellIndex;


	for (size_t i = MinCellIndex; i <= MaxCellIndex; i++)
	{
		m_NaviCellIndex.push_back(i);
	}

}

void CAnimModel::Send_Collision(_uint iLevel, _bool bActive)
{
	Protocol::S_NAVIGATION pkt;

	pkt.set_ilevel(iLevel);
	pkt.set_bactive(bActive);

	auto Indices = pkt.mutable_iindex();
	Indices->Resize(m_NaviCellIndex.size(), -1);
	memcpy(Indices->mutable_data(), m_NaviCellIndex.data(), sizeof(_uint) * m_NaviCellIndex.size());

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);

}



CAnimModel* CAnimModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType)
{
	CAnimModel* pInstance = new CAnimModel(pDevice, pContext, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CAnimModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAnimModel::Clone(void* pArg)
{
	CAnimModel* pInstance = new CAnimModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CAnimModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimModel::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
