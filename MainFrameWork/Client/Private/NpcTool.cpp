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

	m_pTransformPartCom = CLockFree_Transform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformPartCom)
		return E_FAIL;

	m_NpcCreateDesc.iCurLevel = (_uint)LEVELID::LEVEL_TOOL_NPC;

	if (FAILED(Ready_Sort_Part()))
		return E_FAIL;

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

HRESULT CNpcTool::Ready_Sort_Part()
{
	for (auto& pProto : *m_pGameInstance->Get_Component_Prototype(LEVELID::LEVEL_TOOL_NPC))
	{
		wstring strToRemove = L"Prototype_Component_Model_";
		wstring strProtoName = pProto.first;

		size_t remove_pos = strProtoName.find(strToRemove);
		if (remove_pos != std::wstring::npos)
			strProtoName.erase(remove_pos, strToRemove.length());

		size_t pos = strProtoName.find(L"_");
		while (pos != wstring::npos)
		{
			strProtoName.replace(pos, 1, L" ");
			pos = (pos + 1 < strProtoName.size()) ? strProtoName.find(L"_", pos + 1) : wstring::npos;
		}

		wstring search_str = L"WP";
		size_t Wp_pos = strProtoName.find(search_str);

		search_str = L"Head";
		size_t Head_pos = strProtoName.find(search_str);

		search_str = L"Body";
		size_t Body_pos = strProtoName.find(search_str);

		size_t repos = strProtoName.find(L" ");
		while (repos != wstring::npos)
		{
			strProtoName.replace(repos, 1, L"_");
			repos = (repos + 1 < strProtoName.size()) ? strProtoName.find(L" ", repos + 1) : wstring::npos;
		}

		if (Wp_pos != string::npos)
		{
			m_mapWp.emplace(strProtoName, pProto.second);
		}
		else if (Head_pos != string::npos)
		{
			m_mapHead.emplace(strProtoName, pProto.second);
		}
		else if (Body_pos != string::npos)
		{
			m_mapBody.emplace(strProtoName, pProto.second);
		}
		
	}

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

	ImGui::Spacing();
	ImGui::SeparatorText("Create And Save");
	if (ImGui::Button("Create NPC"))
	{

	}
	ImGui::SameLine();
	if (ImGui::Button("Save NPC"))
	{

	}

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
				m_bSelected = false;
			}
			else
			{
				pInstance->Initialize(nullptr);

				m_pMannequin->Set_ModelCom(pInstance);
				m_pMannequin->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vStartPos);
				m_pMannequin->Set_StartPos(m_vStartPos);

				m_vNpcScale = m_pMannequin->Get_TransformCom()->Get_Scale();
				m_vNpcRot = Vec3().Zero;
				m_vNpcPos = m_vStartPos;

				if (m_pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix)));

				m_bSelected = true;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear NPC"))
		{
			m_vNpcScale = Vec3().Zero;
			m_vNpcRot = Vec3().Zero;
			m_vNpcPos = Vec3().Zero;

			m_pMannequin->Clear_MQ();
			m_bSelected = false;
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
	ImGui::SameLine();
	ImGui::Text("Pos Y : %.2f", m_vStartPos.y);
	ImGui::SameLine();
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
	if (ImGui::BeginTabBar("ObjectTabs") && true == m_bSelected)
	{
		Transform_Matrix(fTimeDelta);
		Animaition(fTimeDelta);
		Weapon(fTimeDelta);
		HeadBody(fTimeDelta);
		Move(fTimeDelta);
		Talk(fTimeDelta);
	}

	ImGui::EndTabBar();
}

void CNpcTool::Transform_Matrix(const _float& fTimeDelta)
{
	if (ImGui::BeginTabItem("Transform"))
	{
		ImGui::SeparatorText("Npc Transform Edit");

		ImGui::Checkbox("Change Transform", &m_bTransformChange);
		ImGui::Spacing();
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("NpScaleX", &m_vNpcScale.x, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("NpScaleY", &m_vNpcScale.y, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("NpScaleZ", &m_vNpcScale.z, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::Spacing();
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("NpRotX", &m_vNpcRot.x, 0.1f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("NpRotY", &m_vNpcRot.y, 0.1f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("NpRotZ", &m_vNpcRot.z, 0.1f);
		ImGui::Spacing();
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("NpPosX", &m_vNpcPos.x, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("NpPosY", &m_vNpcPos.y, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("NpPosZ", &m_vNpcPos.z, 0.01f);

		if (true == m_bTransformChange)
		{
			m_pMannequin->Get_TransformCom()->Set_WorldMatrix(XMMatrixIdentity());
			m_pMannequin->Get_TransformCom()->Set_Scale(m_vNpcScale);
			m_pMannequin->Get_TransformCom()->My_Rotation(m_vNpcRot);
			m_pMannequin->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vNpcPos);
		}

		ImGui::EndTabItem();
	}
	
}

void CNpcTool::Move(const _float& fTimeDelta)
{
	if (ImGui::BeginTabItem("Move"))
	{
		if (KEY_TAP(KEY::LBTN) && false == Is_MouseClickedGUI())
		{
			Vec3 vClickPos;
			if (true == Get_CellPos(vClickPos))
			{
				m_vMovePos = vClickPos;
			}
		}

		ImGui::SeparatorText("Npc Transform Edit");

		if (ImGui::Checkbox("Move", &m_NpcCreateDesc.IsMove))
		{
			m_pMannequin->Set_Move_State(m_NpcCreateDesc.IsMove);

			if (false == m_NpcCreateDesc.IsMove)
			{
				m_NpcCreateDesc.vecMovePos.clear();
				m_pMannequin->Clear_MovePos();
			}
			
		}
		ImGui::Spacing();
		ImGui::Text("Move X : %.2f", m_vMovePos.x);
		ImGui::SameLine();
		ImGui::Text("Move Y : %.2f", m_vMovePos.y);
		ImGui::SameLine();
		ImGui::Text("Move Z : %.2f", m_vMovePos.z);

		if (ImGui::Button("Push Move Pos"))
		{
			m_pMannequin->Push_MovePos(m_vMovePos);
		}
		ImGui::SameLine();
		if (ImGui::Button("Pop Move Pos"))
		{
			m_pMannequin->Pop_MovePos();
		}
		
		ImGui::EndTabItem();
	}
	
}

void CNpcTool::Talk(const _float& fTimeDelta)
{
	if (ImGui::BeginTabItem("Npc Talk"))
	{
		ImGui::SeparatorText("Talk Setting");
		if (ImGui::Button("Set TalkBubble"))
		{
			m_pMannequin->Set_Talk_State(true, m_vecTalks, m_fTalkStartTime);
			m_NpcCreateDesc.IsTalk = true;
			m_NpcCreateDesc.vecTalks = m_vecTalks;
			m_NpcCreateDesc.fTalkStartTime = m_fTalkStartTime;
		}
		ImGui::SameLine();
		if (ImGui::Button("UnSet TalkBubble"))
		{
			m_pMannequin->Set_Talk_State(false, m_vecTalks, m_fTalkStartTime);
			m_NpcCreateDesc.IsTalk = false;
			m_NpcCreateDesc.vecTalks.clear();
			m_NpcCreateDesc.fTalkStartTime = 0.f;
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(50);
		ImGui::DragFloat("StartTime", &m_fTalkStartTime, 0.01f);
		ImGui::Spacing();

		ImGui::SeparatorText("Talk Setting");
		ImGui::InputText("Talk", m_szTalk, MAX_PATH);
		if(ImGui::Button("Push"))
		{
			string strName = m_szTalk;
			m_vecSelectTalk.push_back(strName);

			_int w_len = MultiByteToWideChar(CP_UTF8, 0, m_szTalk, -1, nullptr, 0);
			wchar_t* w_buffer = new wchar_t[w_len];

			// 멀티바이트에서 와이드로 변환
			if (MultiByteToWideChar(CP_UTF8, 0, m_szTalk, -1, w_buffer, w_len) == 0) 
			{
				delete[] w_buffer;
			}
			else
			{
				wstring strTalkScript = w_buffer;
				delete[] w_buffer;
				m_vecTalks.push_back(strTalkScript);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Pop"))
		{
			m_vecTalks.pop_back();
			m_vecSelectTalk.pop_back();
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear"))
		{
			m_vecTalks.clear();
			m_vecSelectTalk.clear();
		}
		ImGui::Spacing();

		ImGui::SeparatorText("Talk Scipt");
		_int iCurrIndex = m_iCurTalk;

		if (ImGui::BeginListBox("##Talk", ImVec2(300, 300)))
		{
			for (size_t i = 0; i < m_vecTalks.size(); i++)
			{
				if (ImGui::Selectable(m_vecSelectTalk[i].c_str(), i == iCurrIndex))
				{
					strcpy_s(m_szTalk, m_vecSelectTalk[i].c_str());
					m_iCurTalk = i;
				}
			}
			ImGui::EndListBox();
		}

		ImGui::EndTabItem();
	}
}

void CNpcTool::Animaition(const _float& fTimeDelta)
{
	if (ImGui::BeginTabItem("Save Animation"))
	{
		ImGui::SeparatorText("Npc Animation");
		if(ImGui::Button("Set Idle Anim"))
		{
			string strAnimName = m_szAnimationName;
			m_NpcCreateDesc.strIdleAnim = CAsUtils::ToWString(strAnimName);
		}
		ImGui::SameLine();
		if (ImGui::Button("Set Act Anim"))
		{
			string strAnimName = m_szAnimationName;
			m_NpcCreateDesc.strActAnim = CAsUtils::ToWString(strAnimName);
		}
		ImGui::Spacing();
		if (ImGui::Button("Set Duration"))
		{
			m_NpcCreateDesc.fChangeAnimTime = m_fDuration;
		}
		ImGui::SameLine();
		ImGui::DragFloat("##ChangeAnimDuration", &m_fDuration, 0.1f);


		ImGui::Spacing();
		ImGui::SeparatorText("Npc Animation");

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

			if (ImGui::BeginListBox("##Animations", ImVec2(300, 300)))
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

		ImGui::EndTabItem();
	}
}

void CNpcTool::Weapon(const _float& fTimeDelta)
{
	if (ImGui::BeginTabItem("Weapon Part"))
	{

		ImGui::SeparatorText("LR Type");
		ImGui::Text("LR Type :");
		ImGui::SameLine();
		if (ImGui::RadioButton("Left", &m_iSelectPart, (_uint)CNpc::WEAPON_PART::LEFT));
		ImGui::SameLine();
		if (ImGui::RadioButton("Right", &m_iSelectPart, (_uint)CNpc::WEAPON_PART::RIGHT));
		ImGui::SameLine();

		string StopButtonName;
		if (m_bSetWpPart)
			StopButtonName = "Unset";
		else
			StopButtonName = "Set";

		if (ImGui::Button(StopButtonName.c_str()))
		{
			m_bSetWpPart = !m_bSetWpPart;
		}

		if (true == m_bSetWpPart)
		{
			if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::LEFT)
			{
				string strWpPart = m_szLeftWpName;
				wstring strFind = CAsUtils::ToWString(strWpPart);

				if (TEXT("") != strFind)
				{
					CComponent* pModel = static_cast<CModel*>(m_mapWp.find(strFind)->second)->Clone(nullptr);

					m_pMannequin->Set_Part((_uint)CNpc::WEAPON_PART::LEFT, static_cast<CModel*>(pModel), m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::LEFT]);

					m_NpcCreateDesc.bUseWeaponPart = true;
					m_NpcCreateDesc.strLeftPart = strFind;
					m_NpcCreateDesc.Left_OffsetMatrix = m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::LEFT];
				}
			}
			else if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::RIGHT)
			{
				string strWpPart = m_szRightWpName;
				wstring strFind = CAsUtils::ToWString(strWpPart);

				if (TEXT("") != strFind)
				{
					CComponent* pModel = static_cast<CModel*>(m_mapWp.find(strFind)->second)->Clone(nullptr);

					m_pMannequin->Set_Part((_uint)CNpc::WEAPON_PART::RIGHT, static_cast<CModel*>(pModel), m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::RIGHT]);

					m_NpcCreateDesc.bUseWeaponPart = true;
					m_NpcCreateDesc.strRightPart = strFind;
					m_NpcCreateDesc.Right_OffsetMatrix = m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::RIGHT];
				}
			}
		}
		if (false == m_bSetWpPart)
		{
			if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::LEFT)
			{
				m_pMannequin->Set_Part((_uint)CNpc::WEAPON_PART::LEFT, nullptr, XMMatrixIdentity());
				m_NpcCreateDesc.strLeftPart = TEXT("");
				m_NpcCreateDesc.Left_OffsetMatrix = XMMatrixIdentity();
			}
			else if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::RIGHT)
			{
				m_pMannequin->Set_Part((_uint)CNpc::WEAPON_PART::RIGHT, nullptr, XMMatrixIdentity());
				m_NpcCreateDesc.strRightPart = TEXT("");
				m_NpcCreateDesc.Right_OffsetMatrix = XMMatrixIdentity();
			}

			if (TEXT("") == m_NpcCreateDesc.strLeftPart && TEXT("") == m_NpcCreateDesc.strRightPart)
			{
				m_NpcCreateDesc.bUseWeaponPart = false;
			}
		}
		ImGui::Spacing();

		ImGui::SeparatorText("Wp Part Name");
		if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::LEFT)
		{
			ImGui::InputText("Left", m_szLeftWpName, MAX_PATH);
		}
		else if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::RIGHT)
		{
			ImGui::InputText("Right", m_szRightWpName, MAX_PATH);
		}

		ImGui::SeparatorText("Select Wp Part");
		_int iCurrIndex = m_iCurWpIndex;
		if (ImGui::BeginListBox("##WpPart", ImVec2(300, 150)))
		{
			_int index = 0;
			for (auto iter = m_mapWp.begin(); iter != m_mapWp.end(); iter++, index++)
			{
				if (ImGui::Selectable(CAsUtils::ToString(iter->first).c_str(), index == iCurrIndex))
				{
					if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::LEFT)
					{
						strcpy_s(m_szLeftWpName, CAsUtils::ToString(iter->first).c_str());
					}
					else if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::RIGHT)
					{
						strcpy_s(m_szRightWpName, CAsUtils::ToString(iter->first).c_str());
					}

					m_iCurWpIndex = index;
				}
			}
			ImGui::EndListBox();
		}
		
		string TransformOffsetName;
		if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::LEFT)
		{
			TransformOffsetName = "Left Offset";
		}
		else if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::RIGHT)
		{
			TransformOffsetName = "Right Offset";
		}
		ImGui::SeparatorText(TransformOffsetName.c_str());
		
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("WpScaleX", &m_vWpScale.x, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("WpScaleY", &m_vWpScale.y, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("WpScaleZ", &m_vWpScale.z, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::Spacing();
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("WpRotX", &m_vWpRot.x, 0.1f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("WpRotY", &m_vWpRot.y, 0.1f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("WpRotZ", &m_vWpRot.z, 0.1f);
		ImGui::Spacing();
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("WpPosX", &m_vWpPos.x, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("WpPosY", &m_vWpPos.y, 0.01f);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("WpPosZ", &m_vWpPos.z, 0.01f);

		m_pTransformPartCom->Set_WorldMatrix(XMMatrixIdentity());
		m_pTransformPartCom->Set_Scale(m_vWpScale);
		m_pTransformPartCom->My_Rotation(m_vWpRot);
		m_pTransformPartCom->Set_State(CTransform::STATE_POSITION, m_vWpPos);

		if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::LEFT)
		{
			m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::LEFT] = m_pTransformPartCom->Get_WorldMatrix();
		}
		else if (m_iSelectPart == (_uint)CNpc::WEAPON_PART::RIGHT)
		{
			m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::RIGHT] = m_pTransformPartCom->Get_WorldMatrix();
		}

		ImGui::EndTabItem();
	}
}

void CNpcTool::HeadBody(const _float& fTimeDelta)
{
	if (ImGui::BeginTabItem("HeadBody Part"))
	{
		ImGui::SeparatorText("Model Part Type");
		ImGui::Text("Model Type :");
		ImGui::SameLine();
		if (ImGui::RadioButton("Left", &m_iSelectPart, (_uint)CNpc::WEAPON_PART::LEFT));
		ImGui::SameLine();
		if (ImGui::RadioButton("Right", &m_iSelectPart, (_uint)CNpc::WEAPON_PART::RIGHT));
		ImGui::SameLine();


		ImGui::EndTabItem();
	}
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
	Safe_Release(m_pTransformPartCom);
}
