#include "stdafx.h"
#include "Camera_Lobby.h"
#include "Engine_Defines.h"
#include "GameInstance.h"

CCamera_Lobby::CCamera_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag)
{
}

CCamera_Lobby::CCamera_Lobby(const CCamera_Lobby & rhs, CTransform::TRANSFORMDESC * pArg)
	: CCamera(rhs, pArg)
{
	
}

HRESULT CCamera_Lobby::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Lobby::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	Vec3 vCameraPos = Vec3(0.736f, 0.594f, -0.946f);
	Vec3 vLook = Vec3(0.765f, 0.516f, 0.967f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCameraPos);
	m_pTransformCom->LookAt_SetUp(vLook);
	m_pTransformCom->My_Rotation(Vec3(0.f, 0.f, -0.8f));

	return S_OK;
}

void CCamera_Lobby::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//if (LEVEL_LOBBY != pGameInstance->Get_CurrLevelIndex())
		//pGameInstance->Delete_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, this);

	//if (KEY_HOLD(KEY::UP_ARROW))
	//{
	//	m_pTransformCom->Go_Straight(2.0f, fTimeDelta);
	//}

	//if (KEY_HOLD(KEY::DOWN_ARROW))
	//{
	//	m_pTransformCom->Go_Backward(2.0f, fTimeDelta);
	//}

	//if (KEY_HOLD(KEY::LEFT_ARROW))
	//{
	//	m_pTransformCom->Go_Left(2.0f, fTimeDelta);
	//}

	//if (KEY_HOLD(KEY::RIGHT_ARROW))
	//{
	//	m_pTransformCom->Go_Right(2.0f, fTimeDelta);
	//}

	//if (KEY_TAP(KEY::V))
	//{
	//	cout << m_pTransformCom->Get_State(CTransform::STATE_POSITION).x << endl;
	//	cout << m_pTransformCom->Get_State(CTransform::STATE_POSITION).y << endl;
	//	cout << m_pTransformCom->Get_State(CTransform::STATE_POSITION).z << endl;
	//}
	//
	//	POINT MousePos;

	//	MousePos.x = g_iWinSizeX / 2;
	//	MousePos.y = g_iWinSizeY / 2;

	//	ClientToScreen(g_hWnd, &MousePos);
	//	SetCursorPos(MousePos.x, MousePos.y);

	//	_long	MouseMove = 0;

	//	if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
	//	{
	//		if (pGameInstance->Mouse_Pressing(DIMK_RBUTTON))
	//			m_pTransformCom->Move_Dir(-m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta, MouseMove * 3.f);
	//		else
	//			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * fTimeDelta * 0.05f);
	//	}

	//	if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
	//	{
	//		if (pGameInstance->Mouse_Pressing(DIMK_RBUTTON))
	//			m_pTransformCom->Move_Dir(m_pTransformCom->Get_State(CTransform::STATE_UP), fTimeDelta, MouseMove * 3.f);
	//		else
	//			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMove * fTimeDelta * 0.05f);

	//	}
	Safe_Release(pGameInstance);
}

void CCamera_Lobby::LateTick(_float fTimeDelta)
{
}

HRESULT CCamera_Lobby::Render()
{
	return S_OK;
}

HRESULT CCamera_Lobby::Ready_Components()
{
	__super::Ready_Components();
	return S_OK;
}

CCamera_Lobby * CCamera_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Lobby*		pInstance = new CCamera_Lobby(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Lobby::Clone(void* pArg)
{
	CCamera::CAMERADESC*		pCameraDesc = (CCamera::CAMERADESC*)pArg;

	CCamera_Lobby*		pInstance = new CCamera_Lobby(*this, &pCameraDesc->TransformDesc);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Lobby::Free()
{
	__super::Free();


}
