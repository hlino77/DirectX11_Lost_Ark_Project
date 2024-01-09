#include "stdafx.h"
#include "NpcTool.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>
#include "Utils.h"
#include "AsUtils.h"
#include "Transform.h"
#include "Utils.h"
#include "Camera_Free.h"
#include "tinyxml2.h"
#include "Mannequin.h"
#include "Model.h"
#include "Animation.h"
#include "PartObject.h"
#include "Level_Tool.h"
#include "NavigationMgr.h"

namespace fs = std::filesystem;

CNpcTool::CNpcTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
	m_pUtils = GET_INSTANCE(CUtils);
}

HRESULT CNpcTool::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	m_pMannequin = CMannequin::Create(m_pDevice, m_pContext);
	if (nullptr == m_pMannequin)
		return E_FAIL;
	m_pMannequin->Set_CurrLevel((_uint)LEVELID::LEVEL_TOOL_NPC);

	m_pTransformCom = CLockFree_Transform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	m_NpcCreateDesc.iCurLevel = (_uint)LEVELID::LEVEL_TOOL_NPC;

	return S_OK;
}

HRESULT CNpcTool::Tick(const _float& fTimeDelta)
{
	Input();
	ModelView(fTimeDelta);

	m_pMannequin->Tick(fTimeDelta);

	return S_OK;
}

HRESULT CNpcTool::LateTick(const _float& fTimeDelta)
{
	m_pMannequin->LateTick(fTimeDelta);

	return S_OK;
}

HRESULT CNpcTool::DebugRender()
{
	return S_OK;
}

void CNpcTool::Input()
{
}

_bool CNpcTool::Get_CellPos(Vec3& vPos)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	SimpleMath::Vector3 vMousePos;
	vMousePos.x = _float(pt.x / (g_iWinSizeX * .5f) - 1);
	vMousePos.y = _float(pt.y / (g_iWinSizeY * -.5f) + 1);
	vMousePos.z = 0.f;

	Matrix matProjInv = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	matProjInv = matProjInv.Invert();

	vMousePos = XMVector3TransformCoord(vMousePos, matProjInv);

	Vec3 vRayPos = Vec3(0.f, 0.f, 0.f);
	Vec3 vRayDir = vMousePos - vRayPos;

	Matrix matViewInv = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	matViewInv = matViewInv.Invert();

	vRayPos = XMVector3TransformCoord(vRayPos, matViewInv);
	vRayDir = XMVector3TransformNormal(vRayDir, matViewInv);

	return CNavigationMgr::GetInstance()->Picking_Cell(LEVEL_TOOL_NPC, vRayPos, vRayDir, vPos);
}

HRESULT CNpcTool::ModelView(const _float& fTimeDelta)
{
	ImGui::Begin("Npc");

	InfoView();
	//Categories();
	Select_Npc(fTimeDelta);
	Edit_Npc(fTimeDelta);

	ImGui::End();

	return S_OK;
}

void CNpcTool::Select_Npc(const _float& fTimeDelta)
{
	if (ImGui::CollapsingHeader("Select Npc"))
	{
		Start_Pos(fTimeDelta);
		Name(fTimeDelta);
		Type(fTimeDelta);
		Shape(fTimeDelta);

		if(ImGui::Button("Select NPC"))
		{
			Matrix		PivotMatrix = XMMatrixIdentity();
			PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-90.0f));

			CModel* pInstance = nullptr;

			wstring strFileName = m_NpcCreateDesc.strNpcMq;
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			pInstance = CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix);
			if (nullptr == pInstance)
			{
				m_pMannequin->Set_ModelCom(nullptr);
			}
			else
			{
				pInstance->Initialize(nullptr);

				m_pMannequin->Set_ModelCom(pInstance);
				m_pMannequin->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vStartPos);

				if (m_pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix)));
			}
		}
	}
}

void CNpcTool::Start_Pos(const _float& fTimeDelta)
{
	ImGui::SeparatorText("Npc Pos Select");
	ImGui::SetNextItemWidth(50);

	if (KEY_TAP(KEY::RBTN) && false == Is_MouseClickedGUI())
	{
		Vec3 vClickPos;
		if (true == Get_CellPos(vClickPos))
		{
			m_vStartPos = vClickPos;
		}
	}
	ImGui::Text("Pos X : %.2f", m_vStartPos.x);
	ImGui::Text("Pos Y : %.2f", m_vStartPos.y);
	ImGui::Text("Pos Z : %.2f", m_vStartPos.z);
}

void CNpcTool::Name(const _float& fTimeDelta)
{
	ImGui::SeparatorText("Npc Tag Select");
	ImGui::SetNextItemWidth(50);
	ImGui::Text("Npc Tag  :");
	ImGui::SameLine();
	ImGui::InputText("##Tag", m_szNpcTag, MAX_PATH);
	ImGui::Text("Npc Name :");
	ImGui::SameLine();
	ImGui::InputText("##Name", m_szNpcName, MAX_PATH);

	string Nptag = m_szNpcTag;
	string NpName = m_szNpcName;
	m_NpcCreateDesc.strNpcTag = CAsUtils::ToWString(Nptag);
	m_NpcCreateDesc.strNpcName = CAsUtils::ToWString(NpName);
}

void CNpcTool::Type(const _float& fTimeDelta)
{
	ImGui::SeparatorText("Npc Type Select");
	ImGui::SetNextItemWidth(50);
	ImGui::Text("NpcType :");
	ImGui::SameLine();
	if (ImGui::RadioButton("Deco", &m_iSelectNpType, 0));
	ImGui::SameLine();
	if (ImGui::RadioButton("Func", &m_iSelectNpType, 1));

	switch (m_iSelectNpType)
	{
	case 0:
		m_NpcCreateDesc.iNpcType = (_uint)CNpc::NPCTYPE::DECO;
		break;
	case 1:
		m_NpcCreateDesc.iNpcType = (_uint)CNpc::NPCTYPE::FUNCTION;
		break;
	default:
		break;
	}
}

void CNpcTool::Shape(const _float& fTimeDelta)
{
	ImGui::SeparatorText("Npc Shape Select");
	ImGui::SetNextItemWidth(50);
	ImGui::Checkbox("MA01", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::MA01]);
	ImGui::SameLine();
	ImGui::Checkbox("MA02", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::MA02]);
	ImGui::SameLine();
	ImGui::Checkbox("MA03", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::MA03]);
	ImGui::SameLine();
	ImGui::Checkbox("MA04", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::MA04]);
	ImGui::SameLine();
	ImGui::Checkbox("MA05", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::MA05]);

	ImGui::SetNextItemWidth(50);
	ImGui::Checkbox("FA01", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE01]);
	ImGui::SameLine();
	ImGui::Checkbox("FA02", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE02]);
	ImGui::SameLine();
	ImGui::Checkbox("FA03", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE03]);
	ImGui::SameLine();
	ImGui::Checkbox("FA04", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE04]);
	ImGui::SameLine();
	ImGui::Checkbox("FA05", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE05]);

	ImGui::SetNextItemWidth(50);
	ImGui::Checkbox("Sol", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::SOL]);
	ImGui::SameLine();
	ImGui::Checkbox("SP", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::SP]);

	// 선택된 체크박스 확인
	for (int i = 0; i < (_uint)CNpc::NPCSHAPE::_END; ++i)
	{
		if (true == m_Check_NpcShape[i])
		{
			if (-1 == m_iLastCheck)
				m_iLastCheck = i;
			else if (m_iLastCheck != i)
			{
				m_Check_NpcShape[m_iLastCheck] = false;
				m_iLastCheck = i;
			}
		}
	}

	for (size_t i = 0; i < (_uint)CNpc::NPCSHAPE::_END; i++)
	{
		if (false == m_Check_NpcShape[i]) continue;

		m_eNpcShape = (CNpc::NPCSHAPE)i;
	}

	ImGui::Spacing();

	if (CNpc::NPCSHAPE::SOL == m_eNpcShape)
	{
		ImGui::Text("Soldier Type :");
		if (ImGui::RadioButton("MA Soldier", &m_iSelectSol, 0));
		ImGui::SameLine();
		if (ImGui::RadioButton("MA Knight", &m_iSelectSol, 1));
		if (ImGui::RadioButton("FE Soldier", &m_iSelectSol, 2));
		ImGui::SameLine();
		if (ImGui::RadioButton("FE Knight", &m_iSelectSol, 3));
	}
	else
	{
		m_iSelectSol = -1;
	}

	if (CNpc::NPCSHAPE::SP == m_eNpcShape)
	{
		ImGui::Text("Special Type :");
		ImGui::SameLine();
		if (ImGui::RadioButton("Adel", &m_iSelectSP, 0));
		ImGui::SameLine();
		if (ImGui::RadioButton("Lineheart", &m_iSelectSP, 1));
		ImGui::SameLine();
		if (ImGui::RadioButton("Eadalin", &m_iSelectSP, 2));
	}
	else
	{
		m_iSelectSP = -1;
	}

	switch (m_eNpcShape)
	{
	case Client::CNpc::NPCSHAPE::MA01:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA01_MQ");
		break;
	case Client::CNpc::NPCSHAPE::MA02:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA02_MQ");
		break;
	case Client::CNpc::NPCSHAPE::MA03:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA03_MQ");
		break;
	case Client::CNpc::NPCSHAPE::MA04:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA04_MQ");
		break;
	case Client::CNpc::NPCSHAPE::MA05:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA05_MQ");
		break;
	case Client::CNpc::NPCSHAPE::FE01:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE01_MQ");
		break;
	case Client::CNpc::NPCSHAPE::FE02:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE02_MQ");
		break;
	case Client::CNpc::NPCSHAPE::FE03:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE03_MQ");
		break;
	case Client::CNpc::NPCSHAPE::FE04:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE04_MQ");
		break;
	case Client::CNpc::NPCSHAPE::FE05:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE05_MQ");
		break;
	default:
		break;
	}
	switch (m_iSelectSol)
	{
	case 0:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA02_BernSoldier");
		break;
	case 1:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA02_BernKnight");
		break;
	case 2:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE02_BernSoldier");
		break;
	case 3:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE02_BernKnight");
		break;
	default:
		break;
	}
	switch (m_iSelectSP)
	{
	case 0:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_SP_Adel");
		break;
	case 1:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_SP_Lineheart");
		break;
	case 2:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_SP_Eadalin");
		break;
	default:
		break;
	}
}

void CNpcTool::Edit_Npc(const _float& fTimeDelta)
{
}

void CNpcTool::Transform_Matrix(const _float& fTimeDelta)
{
}

void CNpcTool::Move(const _float& fTimeDelta)
{
}

void CNpcTool::Talk(const _float& fTimeDelta)
{
}

void CNpcTool::Animaition(const _float& fTimeDelta)
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
				m_pMannequin->Get_ModelCom()->Set_Enforce_CurrAnimFrame(iCurrFrame);
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

void CNpcTool::Weapon(const _float& fTimeDelta)
{
	/*if (ImGui::CollapsingHeader("Weapon Select"))
	{
		ImGui::SeparatorText("Weapon");
		ImGui::SetNextItemWidth(50);

		ImGui::Text("LR Type :");
		ImGui::SameLine();
		if (ImGui::RadioButton("Left", &m_iSelectPart, PARTTYPE::PART_L));
		ImGui::SameLine();
		if (ImGui::RadioButton("Right", &m_iSelectPart, PARTTYPE::PART_R));

	

		ImGui::Spacing();

		if (ImGui::Button("Select Weapon"))
		{
			if (CLASS::CLASS_END != m_eSelectClass)
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
	}*/
}

void CNpcTool::Create_Npc(const _float& fTimeDelta)
{
}

void CNpcTool::Save_Npc(const _float& fTimeDelta)
{
}

void CNpcTool::InfoView()
{
}

void CNpcTool::SelectAnim()
{
}

CNpcTool* CNpcTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CLevel_Tool_Npc* pLevel_Tool)
{
	CNpcTool* pInstance = new CNpcTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pLevel_Tool)))
	{
		MSG_BOX("Failed to Created : CNpcTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcTool::Free()
{
	Super::Free();
	RELEASE_INSTANCE(CUtils);

	Safe_Release(m_pMannequin);
	Safe_Release(m_pTransformCom);
}
