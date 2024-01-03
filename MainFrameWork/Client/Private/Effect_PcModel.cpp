#include "stdafx.h"
#include "Effect_PcModel.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>
#include "Utils.h"
#include "AsUtils.h"
#include "Transform.h"
#include "Utils.h"
#include "VoidEffect.h"
#include "Camera_Free.h"
#include "tinyxml2.h"
#include "Mannequin.h"
#include "Model.h"
#include "Animation.h"
#include "PartObject.h"
#include "Level_Tool.h"

namespace fs = std::filesystem;

CEffect_PcModel::CEffect_PcModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
	m_pUtils = GET_INSTANCE(CUtils);
}

HRESULT CEffect_PcModel::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	if (FAILED(Ready_PcModels()))
		return E_FAIL;
	if (FAILED(Ready_PartsModels()))
		return E_FAIL;

	m_pMannequin = CMannequin::Create(m_pDevice, m_pContext);
	if (nullptr == m_pMannequin)
		return E_FAIL;

	m_pTransformCom = CLockFree_Transform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_PcModel::Tick(const _float& fTimeDelta)
{
	Input();
	ModelView(fTimeDelta);

	m_pMannequin->Tick(fTimeDelta);

	return S_OK;
}

HRESULT CEffect_PcModel::LateTick(const _float& fTimeDelta)
{
	m_pMannequin->LateTick(fTimeDelta);

	return S_OK;
}

HRESULT CEffect_PcModel::DebugRender()
{
	return S_OK;
}

void CEffect_PcModel::Input()
{
}

HRESULT CEffect_PcModel::ModelView(const _float& fTimeDelta)
{
	ImGui::Begin("Model View");

	InfoView();
	//Categories();
	Class(fTimeDelta);
	Weapon(fTimeDelta);
	Animaition(fTimeDelta);

	ImGui::End();

	return S_OK;
}

void CEffect_PcModel::Class(const _float& fTimeDelta)
{
	if (ImGui::CollapsingHeader("Model Select"))
	{
		ImGui::SeparatorText("Class");
		ImGui::SetNextItemWidth(50);
		ImGui::Checkbox("Slayer", &m_CheckBoxStates[0]);
		ImGui::SameLine();
		ImGui::Checkbox("Gunslinger", &m_CheckBoxStates[1]);
		ImGui::SameLine();
		ImGui::Checkbox("Destroyer", &m_CheckBoxStates[2]);
		ImGui::SameLine();
		ImGui::Checkbox("Bard", &m_CheckBoxStates[3]);

		// 선택된 체크박스 확인
		for (int i = 0; i < 4; ++i)
		{
			if (true == m_CheckBoxStates[i])
			{
				if (-1 == m_iLastCheck)
					m_iLastCheck = i;
				else if (m_iLastCheck != i)
				{
					m_CheckBoxStates[m_iLastCheck] = false;
					m_iLastCheck = i;
				}
			}
		}

		for (size_t i = 0; i < 4; i++)
		{
			if (false == m_CheckBoxStates[i]) continue;

			if (i == 0)
				m_eSelectClass = CLASS::SLAYER;
			else if (i == 1)
				m_eSelectClass = CLASS::GUNSLINGER;
			else if (i == 2)
				m_eSelectClass = CLASS::DESTROYER;
			else if (i == 3)
				m_eSelectClass = CLASS::BARD;
		}

		ImGui::Spacing();

		if (ImGui::Button("Select Class"))
		{
			SelectClass(m_eSelectClass);
		}
		ImGui::SameLine();
		if (ImGui::Button("Class Clear"))
		{
			static_cast<CMannequin*>(m_pMannequin)->Set_ModelCom(nullptr);

			for (int i = 0; i < CLASS_END; ++i)
			{
				m_CheckBoxStates[i] = false;
			}

			ZeroMemory(m_szAnimationName, sizeof(m_szAnimationName));
			m_pCurrentPlayer = nullptr;
		}
		ImGui::SameLine();

		if (ImGui::Button("Set Effect Pivot Player"))
		{
			m_pLevel_Tool->SetPivotObject(m_pCurrentPlayer);
		}
	}
}

void CEffect_PcModel::Weapon(const _float& fTimeDelta)
{
	if (ImGui::CollapsingHeader("Weapon Select"))
	{
		ImGui::SeparatorText("Weapon");
		ImGui::SetNextItemWidth(50);

		ImGui::Text("LR Type :");
		ImGui::SameLine();
		if (ImGui::RadioButton("Left", &m_iSelectPart, PARTTYPE::PART_L));
		ImGui::SameLine();
		if (ImGui::RadioButton("Right", &m_iSelectPart, PARTTYPE::PART_R));

		ImGui::Spacing();

		ImGui::Checkbox("Sword", &m_WPCheckBoxStates[0]);
		ImGui::SameLine();
		ImGui::Checkbox("Hand", &m_WPCheckBoxStates[1]);
		ImGui::SameLine();
		ImGui::Checkbox("Shot", &m_WPCheckBoxStates[2]);
		ImGui::SameLine();
		ImGui::Checkbox("Long", &m_WPCheckBoxStates[3]);
		ImGui::SameLine();
		ImGui::Checkbox("Hammer", &m_WPCheckBoxStates[4]);
		ImGui::SameLine();
		ImGui::Checkbox("Muse", &m_WPCheckBoxStates[5]);
		// 선택된 체크박스 확인
		for (int i = 0; i < WEAPON_END; ++i)
		{
			if (true == m_WPCheckBoxStates[i])
			{
				if (-1 == m_iWPLastCheck)
					m_iWPLastCheck = i;
				else if (m_iWPLastCheck != i)
				{
					m_WPCheckBoxStates[m_iWPLastCheck] = false;
					m_iWPLastCheck = i;
				}
			}
		}

		for (size_t i = 0; i < WEAPON_END; i++)
		{
			if (false == m_WPCheckBoxStates[i]) continue;

			if (i == WEAPON::SWORD)
				m_eSelectWeapon = WEAPON::SWORD;
			else if (i == WEAPON::HAND)
				m_eSelectWeapon = WEAPON::HAND;
			else if (i == WEAPON::SHOT)
				m_eSelectWeapon = WEAPON::SHOT;
			else if (i == WEAPON::LONG)
				m_eSelectWeapon = WEAPON::LONG;
			else if (i == WEAPON::HAMMER)
				m_eSelectWeapon = WEAPON::HAMMER;
			else if (i == WEAPON::MUSE)
				m_eSelectWeapon = WEAPON::MUSE;
		}

		ImGui::Spacing();

		if (ImGui::Button("Select Weapon"))
		{
			if(CLASS::CLASS_END != m_eSelectClass)
				SelectWeapon((PARTTYPE)m_iSelectPart, m_eSelectWeapon);
		}
		ImGui::SameLine();
		if (ImGui::Button("Weapon Clear"))
		{
			static_cast<CMannequin*>(m_pMannequin)->Set_Part(m_iSelectPart, nullptr, XMMatrixIdentity());

			for (int i = 0; i < WEAPON_END; ++i)
			{
				m_WPCheckBoxStates[i] = false;
			}

			m_pCurrentWeapon = nullptr;
		}

		if (ImGui::Button("Set Effect Pivot WP"))
		{
			m_pLevel_Tool->SetPivotObject(m_pCurrentWeapon);
		}

		if (ImGui::Button("Clear Effect Pivot"))
		{
			m_pLevel_Tool->SetPivotObject(nullptr);
		}
	}
}

void CEffect_PcModel::Animaition(const _float& fTimeDelta)
{
	if (ImGui::CollapsingHeader("Animation"))
	{
		ImGui::SeparatorText("MQ Animation");
		
		ImGui::InputText("AnimName", m_szAnimationName, MAX_PATH);

		string StopButtonName;
		if (m_bAnimationPlay)
			StopButtonName = "Stop";
		else
			StopButtonName = "Play";

		if (ImGui::Button(StopButtonName.c_str()))
		{
			if (m_pMannequin->Get_ModelCom())
			{
				m_bAnimationPlay = !m_bAnimationPlay;
				m_pMannequin->Set_AnimationPlay();
			}
		}
		ImGui::SameLine();
		if (m_pMannequin->Get_ModelCom())
		{
			_int iCurrFrame = m_pMannequin->Get_ModelCom()->Get_Anim_Frame(m_pMannequin->Get_ModelCom()->Get_CurrAnim());
			_int iMaxFrame = m_pMannequin->Get_ModelCom()->Get_Anim_MaxFrame(m_pMannequin->Get_ModelCom()->Get_CurrAnim());

			ImGui::SliderInt("Frame", &iCurrFrame, 0, iMaxFrame);

			if (!m_bAnimationPlay)
				m_pMannequin->Get_ModelCom()->Set_CurrAnimFrame(iCurrFrame);
		}

		if (m_pMannequin->Get_ModelCom())
		{
			vector<CAnimation*>& Animations = m_pMannequin->Get_ModelCom()->Get_Animations();

			_int iCurrIndex = m_iCurrAnimation;

			if (ImGui::BeginListBox("##Animations", ImVec2(800, 500)))
			{
				for (size_t i = 0; i < Animations.size(); i++)
				{
					if (ImGui::Selectable(CAsUtils::ToString(Animations[i]->Get_Name()).c_str(), i == iCurrIndex))
					{
						strcpy_s(m_szAnimationName, CAsUtils::ToString(Animations[i]->Get_Name()).c_str());
						m_iCurrAnimation = i;
						m_pMannequin->Get_ModelCom()->Set_CurrAnim(i);
					}
				}
				ImGui::EndListBox();
			}
		}
	}
}

void CEffect_PcModel::InfoView()
{
}

void CEffect_PcModel::SelectClass(CLASS eClass)
{
	switch (eClass)
	{
	case Client::CEffect_PcModel::SLAYER:
		static_cast<CMannequin*>(m_pMannequin)->Set_ModelCom(m_pPcModels[SLAYER]);
		m_pCurrentPlayer = m_pMannequin;
		break;
	case Client::CEffect_PcModel::GUNSLINGER:
		static_cast<CMannequin*>(m_pMannequin)->Set_ModelCom(m_pPcModels[GUNSLINGER]);
		m_pCurrentPlayer = m_pMannequin;
		break;
	case Client::CEffect_PcModel::DESTROYER:
		static_cast<CMannequin*>(m_pMannequin)->Set_ModelCom(m_pPcModels[DESTROYER]);
		m_pCurrentPlayer = m_pMannequin;
		break;
	case Client::CEffect_PcModel::BARD:
		static_cast<CMannequin*>(m_pMannequin)->Set_ModelCom(m_pPcModels[BARD]);
		m_pCurrentPlayer = m_pMannequin;
		break;
	}

	m_iCurrAnimation = 0;
	wstring strAnimName = static_cast<CMannequin*>(m_pMannequin)->Get_ModelCom()->Get_Animations()[m_iCurrAnimation]->Get_Name();
	strcpy_s(m_szAnimationName, CAsUtils::ToString(strAnimName).c_str());
}

void CEffect_PcModel::SelectWeapon(PARTTYPE ePart, WEAPON eWeapon)
{
	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());

	switch (eWeapon)
	{
	case Client::CEffect_PcModel::SWORD:
		m_pTransformCom->Set_Scale(Vec3(100.f, 100.f, 100.f));
		m_pTransformCom->My_Rotation(Vec3(0.f, 90.f, 90.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(5.f, 0.f, 0.f));
		m_pCurrentWeapon = static_cast<CMannequin*>(m_pMannequin)->Set_Part(ePart, m_pWpModels[SWORD], m_pTransformCom->Get_WorldMatrix());
		break;
	case Client::CEffect_PcModel::HAND:
		m_pTransformCom->Set_Scale(Vec3(100.f, 100.f, 100.f));
		m_pTransformCom->My_Rotation(Vec3(-80.f, 45.f, 40.f));
		m_pCurrentWeapon = static_cast<CMannequin*>(m_pMannequin)->Set_Part(ePart, m_pWpModels[HAND], m_pTransformCom->Get_WorldMatrix());
		break;
	case Client::CEffect_PcModel::SHOT:
		m_pTransformCom->Set_Scale(Vec3(100.f, 100.f, 100.f));
		m_pTransformCom->My_Rotation(Vec3(-80.f, 45.f, 40.f));
		m_pCurrentWeapon = static_cast<CMannequin*>(m_pMannequin)->Set_Part(ePart, m_pWpModels[SHOT], m_pTransformCom->Get_WorldMatrix());
		break;
	case Client::CEffect_PcModel::LONG:
		m_pTransformCom->Set_Scale(Vec3(100.f, 100.f, 100.f));
		m_pTransformCom->My_Rotation(Vec3(-80.f, 45.f, 40.f));
		m_pCurrentWeapon = static_cast<CMannequin*>(m_pMannequin)->Set_Part(ePart, m_pWpModels[LONG], m_pTransformCom->Get_WorldMatrix());
		break;
	case Client::CEffect_PcModel::HAMMER:
		m_pTransformCom->Set_Scale(Vec3(100.f, 100.f, 100.f));
		m_pTransformCom->My_Rotation(Vec3(-10.f, 100.f, -90.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(0.f, -3.f, 0.f));
		m_pCurrentWeapon = static_cast<CMannequin*>(m_pMannequin)->Set_Part(ePart, m_pWpModels[HAMMER], m_pTransformCom->Get_WorldMatrix());
		break;
	case Client::CEffect_PcModel::MUSE:
		m_pTransformCom->Set_Scale(Vec3(100.f, 100.f, 100.f));
		m_pTransformCom->My_Rotation(Vec3(0.f, 70.f, 70.f));
		m_pCurrentWeapon = static_cast<CMannequin*>(m_pMannequin)->Set_Part(ePart, m_pWpModels[MUSE], m_pTransformCom->Get_WorldMatrix());
		break;
	}
}

void CEffect_PcModel::SelectAnim()
{
}

HRESULT CEffect_PcModel::Ready_PcModels()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-90.0f));

	CModel* pInstance = nullptr;
	
	{
		wstring strFileName = L"Gunslinger";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pPcModels[GUNSLINGER] = pInstance;
	}

	{
		wstring strFileName = L"WR";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pPcModels[SLAYER] = pInstance;
	}

	{
		wstring strFileName = L"WDR";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pPcModels[DESTROYER] = pInstance;
	}

	{
		wstring strFileName = L"MG";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pPcModels[BARD] = pInstance;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEffect_PcModel::Ready_PartsModels()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-90.0f));

	CModel* pInstance = nullptr;

	{
		wstring strFileName = L"GN_WP_Hand_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pWpModels[HAND] = pInstance;
	}

	{
		wstring strFileName = L"GN_WP_Long_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pWpModels[LONG] = pInstance;
	}

	{
		wstring strFileName = L"GN_WP_Shot_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pWpModels[SHOT] = pInstance;
	}

	{
		wstring strFileName = L"WP_WR_Base";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pWpModels[SWORD] = pInstance;
	}

	{
		wstring strFileName = L"WDR_WP_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pWpModels[HAMMER] = pInstance;
	}

	{
		wstring strFileName = L"MG_WP_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
		if (nullptr == pInstance)
			return E_FAIL;
		pInstance->Initialize(nullptr);

		m_pWpModels[MUSE] = pInstance;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect_PcModel* CEffect_PcModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CLevel_Tool* pLevel_Tool)
{
	CEffect_PcModel* pInstance = new CEffect_PcModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pLevel_Tool)))
	{
		MSG_BOX("Failed to Created : CEffect_PcModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_PcModel::Free()
{
	Super::Free();
	RELEASE_INSTANCE(CUtils);

	for (size_t i = 0; i < CLASS_END; i++)
	{
		Safe_Release(m_pPcModels[i]);
	}
	for (size_t i = 0; i < WEAPON_END; i++)
	{
		Safe_Release(m_pWpModels[i]);
	}

	Safe_Release(m_pTransformCom);
}
