#include "stdafx.h"
#include "NpcTool.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>
#include "AsFileUtils.h"
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
#include "Npc.h"
#include "Npc_Part.h"

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

	m_pTransformLeftPartCom = CLockFree_Transform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformLeftPartCom)
		return E_FAIL;

	m_pTransformRIghtPartCom = CLockFree_Transform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformRIghtPartCom)
		return E_FAIL;

	m_NpcCreateDesc.iCurLevel = (_uint)LEVELID::LEVEL_TOOL_NPC;

	if (FAILED(Ready_Sort_Part()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNpcTool::Tick(const _float& fTimeDelta)
{
	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::LBTN))
	{
		Pick_Npc();
	}
	Set_DebugRender_Npc();

	Input();
	ModelView(fTimeDelta);
	InfoView(fTimeDelta);

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

		/*size_t pos = strProtoName.find(L"_");
		while (pos != wstring::npos)
		{
			strProtoName.replace(pos, 1, L" ");
			pos = (pos + 1 < strProtoName.size()) ? strProtoName.find(L"_", pos + 1) : wstring::npos;
		}*/

		wstring search_str = L"WP";
		size_t Wp_pos = strProtoName.find(search_str);

		search_str = L"Head";
		size_t Head_pos = strProtoName.find(search_str);

		search_str = L"Body";
		size_t Body_pos = strProtoName.find(search_str);

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

void CNpcTool::Set_DebugRender_Npc()
{
	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::D))
	{
		m_bDebugRender = !m_bDebugRender;
	}
	for (auto& pNpc : m_vecNpcs)
	{
		static_cast<CNpc*>(pNpc)->Set_DebugRender_State(m_bDebugRender);
	}
}

void CNpcTool::Pick_Npc()
{
	_uint iIndex = 0;
	for (auto& pNpc : m_vecNpcs)
	{
		if (true == static_cast<CNpc*>(pNpc)->Intersect_Mouse())
		{
			m_iCurNpc = iIndex;
		}
		iIndex++;
	}
}

HRESULT CNpcTool::InfoView(const _float& fTimeDelta)
{
	ImGui::Begin("NPC INFO");

	ImGui::SeparatorText("Modul Npc");
	Load_Npc();
	ImGui::SameLine();
	Delete_Npc();

	Npc_List();


	ImGui::End();

	return S_OK;
}

void CNpcTool::Npc_List()
{
	ImGui::SeparatorText("Npc List");

	m_vecNpcs = m_pGameInstance->Find_GameObjects(LEVELID::LEVEL_TOOL_NPC, (_uint)LAYER_TYPE::LAYER_NPC);

	_int iCurrIndex = m_iCurNpc;

	if (ImGui::BeginListBox("##Npcs", ImVec2(300, 300)))
	{
		for (size_t i = 0; i < m_vecNpcs.size(); i++)
		{
			if (ImGui::Selectable(CAsUtils::ToString(static_cast<CNpc*>(m_vecNpcs[i])->Get_NpcTag()).c_str(), i == iCurrIndex))
			{
				m_iCurNpc = i;
			}
		}
		ImGui::EndListBox();
	}
}

HRESULT CNpcTool::Load_Npc()
{
	if (ImGui::Button("Load Npc"))
	{
		wstring strLoadpath = (L"../Bin/Resources/ObjectData/Npc/");

		for (const auto& entry : fs::directory_iterator(strLoadpath))
		{
			if (fs::is_regular_file(entry))
			{
				if (L".dat" == entry.path().extension())
				{
					if (FAILED(Start_Load_Npc(entry.path())))
					{
						MessageBox(g_hWnd, L"Npc 불러오기 실패", L"확인", MB_OK);
						return E_FAIL;
					}
						
				}
			}
		}

		MessageBox(g_hWnd, L"Npc 불러오기 성공", L"확인", MB_OK);

	}

	return S_OK;
}

HRESULT CNpcTool::Start_Load_Npc(const wstring& strPath)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CNpc::NPCDESC	NpcCreateDesc;

	shared_ptr<CAsFileUtils> LoadObject = make_shared<CAsFileUtils>();
	LoadObject->Open(strPath, FileMode::Read);

	NpcCreateDesc.iCurLevel = LEVELID::LEVEL_TOOL_NPC;

	string strData;
	LoadObject->Read<_uint>(NpcCreateDesc.iNpcType);
	LoadObject->Read(strData);
	NpcCreateDesc.strNpcTag = CAsUtils::S2W(strData);
	LoadObject->Read<Matrix>(NpcCreateDesc.matStart);

	LoadObject->Read<_uint>(NpcCreateDesc.iNpcShape);
	LoadObject->Read(strData);
	NpcCreateDesc.strNpcMq = CAsUtils::S2W(strData);
	LoadObject->Read(strData);
	NpcCreateDesc.strNpcHead = CAsUtils::S2W(strData);
	LoadObject->Read(strData);
	NpcCreateDesc.strNpcBody = CAsUtils::S2W(strData);

	_uint	iSize;
	LoadObject->Read<_bool>(NpcCreateDesc.IsMove);
	LoadObject->Read<_uint>(iSize);
	for (size_t i = 0; i < iSize; i++)
	{
		Vec3 vPos;
		LoadObject->Read<Vec3>(vPos);
		NpcCreateDesc.vecMovePos.push_back(vPos);
	}

	LoadObject->Read<_bool>(NpcCreateDesc.bUseWeaponPart);
	LoadObject->Read(strData);
	NpcCreateDesc.strLeftPart = CAsUtils::S2W(strData);
	LoadObject->Read<Matrix>(NpcCreateDesc.Left_OffsetMatrix);
	LoadObject->Read(strData);
	NpcCreateDesc.strRightPart = CAsUtils::S2W(strData);
	LoadObject->Read<Matrix>(NpcCreateDesc.Right_OffsetMatrix);

	LoadObject->Read<_bool>(NpcCreateDesc.IsTalk);
	LoadObject->Read<_float>(NpcCreateDesc.fTalkStartTime);

	{
		fs::path strPath = fs::path("../Bin/Resources/ObjectData/Npc/" + CAsUtils::ToString(NpcCreateDesc.strNpcTag) + ".xml");

		shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
		tinyxml2::XMLError error = document->LoadFile(strPath.generic_string().c_str());
		if (error != tinyxml2::XML_SUCCESS)
		{
			return E_FAIL;
		}

		tinyxml2::XMLElement* root = nullptr;
		root = document->FirstChildElement();
		tinyxml2::XMLElement* node = nullptr;
		node = root->FirstChildElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			element = node->FirstChildElement();
			if (element->GetText())
			{
				wstring strInfo = CAsUtils::S2W(element->GetText());
				if (strInfo.length() > 0)
				{
					NpcCreateDesc.strNpcName = strInfo;
				}
			}

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strInfo = CAsUtils::S2W(element->GetText());
				if (strInfo.length() > 0)
				{
					NpcCreateDesc.strIdleAnim = strInfo;
				}
			}

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strInfo = CAsUtils::S2W(element->GetText());
				if (strInfo.length() > 0)
				{
					NpcCreateDesc.strActAnim = strInfo;
				}
			}

			element = element->NextSiblingElement();
			NpcCreateDesc.fChangeAnimTime = element->FloatAttribute("Time");
		}

		node = node->NextSiblingElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			element = node->FirstChildElement();
			NpcCreateDesc.fTalkStartTime = element->FloatAttribute("Time");

			_uint iSize;
			element = element->NextSiblingElement();
			iSize = element->IntAttribute("Size");

			for (size_t i = 0; i < iSize; i++)
			{
				element = element->NextSiblingElement();
				if (element->GetText())
				{
					wstring strTalk = CAsUtils::S2W(element->GetText());
					if (strTalk.length() > 0)
					{
						NpcCreateDesc.vecTalks.push_back(strTalk);
					}
				}
			}
		}

		node = node->NextSiblingElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			_uint iSize;
			element = node->FirstChildElement();
			iSize = element->IntAttribute("Size");

			for (size_t i = 0; i < iSize; i++)
			{
				element = element->NextSiblingElement();
				if (element->GetText())
				{
					wstring strTalk = CAsUtils::S2W(element->GetText());
					if (strTalk.length() > 0)
					{
						NpcCreateDesc.vecTalkSound.push_back(strTalk);
					}
				}
			}
		}
	}

	if ((_uint)CNpc::NPCTYPE::DECO == NpcCreateDesc.iNpcType)
	{
		CGameObject* pInstance = m_pGameInstance->Add_GameObject((_uint)LEVELID::LEVEL_TOOL_NPC, (_uint)LAYER_TYPE::LAYER_NPC,
			TEXT("Prototype_GameObject_DecoNpc"), &NpcCreateDesc);
		if (nullptr == pInstance)
			return E_FAIL;
	}
	else if ((_uint)CNpc::NPCTYPE::FUNCTION == NpcCreateDesc.iNpcType)
	{



	}
	

	m_vecNpcDesc.push_back(NpcCreateDesc);
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CNpcTool::Delete_Npc()
{
	if (ImGui::Button("Delete Npc"))
	{
		_uint iIndex = 0;
		for (auto& pDesc : m_vecNpcDesc)
		{
			if (pDesc.strNpcTag == static_cast<CNpc*>(m_vecNpcs[m_iCurNpc])->Get_NpcTag())
			{
				wstring strLoadpath = (L"../Bin/Resources/ObjectData/Npc/");
				wstring strCopypath = (L"../Bin/Resources/ObjectData/Npc/BackUp/");

				_uint iFileIndex = 0;
				for (const auto& entry : fs::directory_iterator(strLoadpath))
				{
					if (fs::is_regular_file(entry))
					{
						if (pDesc.strNpcTag == entry.path().stem())
						{
							fs::path destinationPath = strCopypath / entry.path().filename();
							fs::copy(entry.path(), destinationPath, fs::copy_options::overwrite_existing);
							std::remove(entry.path().string().c_str());
							iFileIndex++;
						}
						if (2 == iFileIndex) break;
					}
				}

				m_vecNpcDesc.erase(m_vecNpcDesc.begin() + iIndex);
				break;
			}
			iIndex++;
		}

		m_pGameInstance->Delete_GameObject(LEVELID::LEVEL_TOOL_NPC, (_uint)LAYER_TYPE::LAYER_NPC, m_vecNpcs[m_iCurNpc]);
		m_vecNpcs.erase(m_vecNpcs.begin() + m_iCurNpc);
	}	
}

HRESULT CNpcTool::ModelView(const _float& fTimeDelta)
{
	ImGui::Begin("NPC MAKE");

	Select_Npc(fTimeDelta);
	Edit_Npc(fTimeDelta);

	ImGui::Spacing();
	ImGui::SeparatorText("Create And Save");
	if (ImGui::Button("Create NPC"))
	{
		Create_Npc(fTimeDelta);
	}
	ImGui::SameLine();
	if (ImGui::Button("Save NPC"))
	{
		Save_Npc(fTimeDelta);
	}

	ImGui::End();

	return S_OK;
}

void CNpcTool::Select_Npc(const _float& fTimeDelta)
{
	Start_Pos(fTimeDelta);
	Name(fTimeDelta);
	Type(fTimeDelta);
	if (0 == m_iSelectNpType || 1 == m_iSelectNpType)
	{
		Shape(fTimeDelta);

		if (ImGui::Button("Select NPC"))
		{
			m_NpcCreateDesc.strNpcTag = m_strGroup + TEXT("_") + to_wstring(m_pGameInstance->GenerateUniqueID());
			string strID = CAsUtils::ToString(m_NpcCreateDesc.strNpcTag);
			strcat_s(m_szNpcTag, strID.c_str());

			CModel* pInstance = nullptr;

			wstring strFileName = m_NpcCreateDesc.strNpcMq;
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			pInstance = static_cast<CModel*>(m_pGameInstance->Clone_Component(m_pMannequin, LEVELID::LEVEL_TOOL_NPC, strComponentName));
			if (nullptr == pInstance)
			{
				m_pMannequin->Set_ModelCom(nullptr);
				m_bSelected = false;
			}
			else
			{
				m_pMannequin->Set_ModelCom(pInstance);
				m_pMannequin->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vStartPos);
				m_pMannequin->Set_StartPos(m_vStartPos);

				m_vNpcScale = m_pMannequin->Get_TransformCom()->Get_Scale();
				m_vNpcRot = Vec3().Zero;
				m_vNpcPos = m_vStartPos;

				m_bSelected = true;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear NPC"))
		{
			Clear_Info();
			m_pMannequin->Clear_MQ();
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
	ImGui::SeparatorText("Npc Tag & Name");
	ImGui::Text("Npc Tag    :");
	ImGui::SameLine();
	ImGui::InputText("##Tag", m_szNpcTag, MAX_PATH);
	ImGui::Text("Npc Name :");
	ImGui::SameLine();
	ImGui::InputText("##Name", m_szNpcName, MAX_PATH);

	_int w_len = MultiByteToWideChar(CP_UTF8, 0, m_szNpcName, -1, nullptr, 0);
	wchar_t* w_buffer = new wchar_t[w_len];

	if (0 == MultiByteToWideChar(CP_UTF8, 0, m_szNpcName, -1, w_buffer, w_len))
	{
		delete[] w_buffer;
	}
	else
	{
		wstring strNpcName = w_buffer;
		delete[] w_buffer;
		m_NpcCreateDesc.strNpcName = strNpcName;
	}
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
	ImGui::Spacing();

	ImGui::Checkbox("FA01", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE01]);
	ImGui::SameLine();
	ImGui::Checkbox("FA02", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE02]);
	ImGui::SameLine();
	ImGui::Checkbox("FA03", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE03]);
	ImGui::SameLine();
	ImGui::Checkbox("FA04", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE04]);
	ImGui::SameLine();
	ImGui::Checkbox("FA05", &m_Check_NpcShape[(_uint)CNpc::NPCSHAPE::FE05]);
	ImGui::Spacing();

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
		m_NpcCreateDesc.iNpcShape = (_uint)m_eNpcShape;
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
		m_strGroup = TEXT("MA01");
		break;
	case Client::CNpc::NPCSHAPE::MA02:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA02_MQ");
		m_strGroup = TEXT("MA02");
		break;
	case Client::CNpc::NPCSHAPE::MA03:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA03_MQ");
		m_strGroup = TEXT("MA03");
		break;
	case Client::CNpc::NPCSHAPE::MA04:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA04_MQ");
		m_strGroup = TEXT("MA04");
		break;
	case Client::CNpc::NPCSHAPE::MA05:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA05_MQ");
		m_strGroup = TEXT("MA05");
		break;
	case Client::CNpc::NPCSHAPE::FE01:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE01_MQ");
		m_strGroup = TEXT("FE01");
		break;
	case Client::CNpc::NPCSHAPE::FE02:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE02_MQ");
		m_strGroup = TEXT("FE02");
		break;
	case Client::CNpc::NPCSHAPE::FE03:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE03_MQ");
		m_strGroup = TEXT("FE03");
		break;
	case Client::CNpc::NPCSHAPE::FE04:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE04_MQ");
		m_strGroup = TEXT("FE04");
		break;
	case Client::CNpc::NPCSHAPE::FE05:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE05_MQ");
		m_strGroup = TEXT("FE05");
		break;
	default:
		break;
	}
	switch (m_iSelectSol)
	{
	case 0:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA02_BernSoldier");
		m_strGroup = TEXT("MASol");
		break;
	case 1:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_MA02_BernKnight");
		m_strGroup = TEXT("MAKn");
		break;
	case 2:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE02_BernSoldier");
		m_strGroup = TEXT("FESol");
		break;
	case 3:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_FE02_BernKnight");
		m_strGroup = TEXT("FEKn");
		break;
	default:
		break;
	}
	switch (m_iSelectSP)
	{
	case 0:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_SP_Adel");
		m_strGroup = TEXT("Adel");
		break;
	case 1:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_SP_Lineheart");
		m_strGroup = TEXT("Lineheart");
		break;
	case 2:
		m_NpcCreateDesc.strNpcMq = TEXT("NP_SP_Eadalin");
		m_strGroup = TEXT("Eadalin");
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
		HeadBody(fTimeDelta);
		Weapon(fTimeDelta);
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

		if (ImGui::Checkbox("Move", &m_IsMove))
		{
			if (true == m_IsMove)
			{
				m_pMannequin->Set_Move_State(m_IsMove);

				m_NpcCreateDesc.IsMove = m_IsMove;
				m_NpcCreateDesc.vecMovePos = m_vecMovePos;
				
			}
			else if (false == m_IsMove)
			{
				
				m_pMannequin->Set_Move_State(m_IsMove);
				m_pMannequin->Clear_MovePos();

				m_NpcCreateDesc.IsMove = m_IsMove;
				m_NpcCreateDesc.vecMovePos.clear();
				m_vecforMoveList.clear();
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
			m_vecMovePos.push_back(m_vMovePos);
		}
		ImGui::SameLine();
		if (ImGui::Button("Pop Move Pos"))
		{
			if (0 != m_vecMovePos.size())
			{
				m_pMannequin->Pop_MovePos();
				m_vecMovePos.pop_back();
			}
		}
		
		m_vecforMoveList.clear();
		for (size_t i = 0; i < m_vecMovePos.size(); i++)
		{
			string strMovePos = "X: " + to_string(m_vecMovePos[i].x) + " Y: " + to_string(m_vecMovePos[i].y) + " Z: " + to_string(m_vecMovePos[i].z);
			m_vecforMoveList.push_back(strMovePos);
		}

		ImGui::SeparatorText("Move List");
		_int iCurrIndex = m_iCurMove;
		if (ImGui::BeginListBox("##MovePos", ImVec2(300, 100)))
		{
			for (size_t i = 0; i < m_vecMovePos.size(); i++)
			{
				if (ImGui::Selectable(m_vecforMoveList[i].c_str(), i == iCurrIndex))
				{
					m_iCurMove = i;
				}
			}
			ImGui::EndListBox();
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

			if (0 == MultiByteToWideChar(CP_UTF8, 0, m_szTalk, -1, w_buffer, w_len)) 
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
			if (0 != m_vecTalks.size())
			{
				m_vecTalks.erase(m_vecTalks.begin() + m_iCurTalk);
				m_vecSelectTalk.erase(m_vecSelectTalk.begin() + m_iCurTalk);
			}
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

		if (ImGui::BeginListBox("##Talk", ImVec2(300, 200)))
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
	if (ImGui::BeginTabItem("Animation"))
	{
		ImGui::SeparatorText("Setting Animation");
		if(ImGui::Button("Set Idle Anim"))
		{
			string strAnimName = m_szAnimationName;
			m_NpcCreateDesc.strIdleAnim = CAsUtils::ToWString(strAnimName);
			MessageBox(g_hWnd, L"Idle Anim 지정", L"완료", MB_OK);
		}
		ImGui::SameLine();
		if (ImGui::Button("Set Act Anim"))
		{
			string strAnimName = m_szAnimationName;
			m_NpcCreateDesc.strActAnim = CAsUtils::ToWString(strAnimName);
			MessageBox(g_hWnd, L"Act Anim 지정", L"완료", MB_OK);
		}
		ImGui::Spacing();
		if (ImGui::DragFloat("ChangeAnimTime", &m_fDuration, 0.1f))
		{
			m_NpcCreateDesc.fChangeAnimTime = m_fDuration;
		}


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

			if (ImGui::BeginListBox("##Animations", ImVec2(300, 200)))
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

		switch (m_iSelectPart)
		{
		case (_uint)CNpc::WEAPON_PART::LEFT:
			LeftWeapon(fTimeDelta);
			break;
		case (_uint)CNpc::WEAPON_PART::RIGHT:
			RightWeapon(fTimeDelta);
			break;
		default:
			break;
		}

		if (TEXT("") == m_NpcCreateDesc.strLeftPart && TEXT("") == m_NpcCreateDesc.strRightPart)
		{
			m_NpcCreateDesc.bUseWeaponPart = false;
		}

		ImGui::EndTabItem();
	}
}

void CNpcTool::LeftWeapon(const _float& fTimeDelta)
{
	ImGui::SeparatorText("Left Part Name");
	ImGui::InputText("##Left WP", m_szLeftWpName, MAX_PATH);
	ImGui::SameLine();
	string StopButtonName;
	if (m_bSetWpLeftPart)
		StopButtonName = "Unset";
	else
		StopButtonName = "Set";

	if (ImGui::Button(StopButtonName.c_str()))
	{
		m_bSetWpLeftPart = !m_bSetWpLeftPart;
	}

	if (true == m_bSetWpLeftPart)
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
	if (false == m_bSetWpLeftPart)
	{
		m_pMannequin->Set_Part((_uint)CNpc::WEAPON_PART::LEFT, nullptr, XMMatrixIdentity());
		m_NpcCreateDesc.strLeftPart = TEXT("");
		m_NpcCreateDesc.Left_OffsetMatrix = XMMatrixIdentity();
		m_iCurWpLeftIndex = -1;
	}
	ImGui::Spacing();

	ImGui::SeparatorText("Select Left Wp Part");
	_int iCurrIndex = m_iCurWpLeftIndex;
	if (ImGui::BeginListBox("##LeftPart", ImVec2(300, 150)))
	{
		_int index = 0;
		for (auto iter = m_mapWp.begin(); iter != m_mapWp.end(); iter++, index++)
		{
			if (ImGui::Selectable(CAsUtils::ToString(iter->first).c_str(), index == iCurrIndex))
			{
				strcpy_s(m_szLeftWpName, CAsUtils::ToString(iter->first).c_str());
				m_iCurWpLeftIndex = index;
			}
		}
		ImGui::EndListBox();
	}

	ImGui::SeparatorText("Left Offset");

	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("LScaleX", &m_vWpLeftScale.x, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("LScaleY", &m_vWpLeftScale.y, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("LScaleZ", &m_vWpLeftScale.z, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::Spacing();
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("LRotX", &m_vWpLeftRot.x, 0.1f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("LRotY", &m_vWpLeftRot.y, 0.1f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("LRotZ", &m_vWpLeftRot.z, 0.1f);
	ImGui::Spacing();
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("LPosX", &m_vWpLeftPos.x, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("LPosY", &m_vWpLeftPos.y, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("LPosZ", &m_vWpLeftPos.z, 0.01f);

	m_pTransformLeftPartCom->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransformLeftPartCom->Set_Scale(m_vWpLeftScale);
	m_pTransformLeftPartCom->My_Rotation(m_vWpLeftRot);
	m_pTransformLeftPartCom->Set_State(CTransform::STATE_POSITION, m_vWpLeftPos);

	m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::LEFT] = m_pTransformLeftPartCom->Get_WorldMatrix();
}

void CNpcTool::RightWeapon(const _float& fTimeDelta)
{
	ImGui::SeparatorText("Right Part Name");
	ImGui::InputText("##Right WP", m_szRightWpName, MAX_PATH);
	ImGui::SameLine();
	string StopButtonName;
	if (m_bSetWpRightPart)
		StopButtonName = "Unset";
	else
		StopButtonName = "Set";

	if (ImGui::Button(StopButtonName.c_str()))
	{
		m_bSetWpRightPart = !m_bSetWpRightPart;
	}

	if (true == m_bSetWpRightPart)
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
	if (false == m_bSetWpRightPart)
	{
		m_pMannequin->Set_Part((_uint)CNpc::WEAPON_PART::RIGHT, nullptr, XMMatrixIdentity());
		m_NpcCreateDesc.strRightPart = TEXT("");
		m_NpcCreateDesc.Right_OffsetMatrix = XMMatrixIdentity();
		m_iCurWpRightIndex = -1;
	}
	ImGui::Spacing();



	ImGui::SeparatorText("Select Right Wp Part");
	_int iCurrIndex = m_iCurWpRightIndex;
	if (ImGui::BeginListBox("##RightPart", ImVec2(300, 150)))
	{
		_int index = 0;
		for (auto iter = m_mapWp.begin(); iter != m_mapWp.end(); iter++, index++)
		{
			if (ImGui::Selectable(CAsUtils::ToString(iter->first).c_str(), index == iCurrIndex))
			{
				strcpy_s(m_szRightWpName, CAsUtils::ToString(iter->first).c_str());
				m_iCurWpRightIndex = index;
			}
		}
		ImGui::EndListBox();
	}

	ImGui::SeparatorText("Right Offset");

	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("RScaleX", &m_vWpRightScale.x, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("RScaleY", &m_vWpRightScale.y, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("RScaleZ", &m_vWpRightScale.z, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::Spacing();
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("RRotX", &m_vWpRightRot.x, 0.1f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("RRotY", &m_vWpRightRot.y, 0.1f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("RRotZ", &m_vWpRightRot.z, 0.1f);
	ImGui::Spacing();
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("RPosX", &m_vWpRightPos.x, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("RPosY", &m_vWpRightPos.y, 0.01f);
	ImGui::SetNextItemWidth(200);
	ImGui::DragFloat("RPosZ", &m_vWpRightPos.z, 0.01f);

	m_pTransformRIghtPartCom->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransformRIghtPartCom->Set_Scale(m_vWpRightScale);
	m_pTransformRIghtPartCom->My_Rotation(m_vWpRightRot);
	m_pTransformRIghtPartCom->Set_State(CTransform::STATE_POSITION, m_vWpRightPos);

	m_WpOffsetMatrix[(_uint)CNpc::WEAPON_PART::RIGHT] = m_pTransformRIghtPartCom->Get_WorldMatrix();
}

void CNpcTool::HeadBody(const _float& fTimeDelta)
{
	if (CNpc::NPCSHAPE::SOL != m_eNpcShape && CNpc::NPCSHAPE::SP != m_eNpcShape)
	{
		if (ImGui::BeginTabItem("Model Part"))
		{
			ImGui::SeparatorText("Model Part Type");
			ImGui::Text("Model Type :");
			ImGui::SameLine();
			if (ImGui::RadioButton("Head", &m_iSelectModelPart, (_uint)CNpc::PART::FACE));
			ImGui::SameLine();
			if (ImGui::RadioButton("Body", &m_iSelectModelPart, (_uint)CNpc::PART::BODY));


			switch (m_iSelectModelPart)
			{
			case (_uint)CNpc::PART::FACE:
				Head(fTimeDelta);
				break;
			case (_uint)CNpc::PART::BODY:
				Body(fTimeDelta);
				break;
			default:
				break;
			}


			ImGui::EndTabItem();
		}
	}
}

void CNpcTool::Head(const _float& fTimeDelta)
{
	ImGui::SeparatorText("Select Head Part");

	_int iCurrIndex = m_iCurHeadIndex;
	if (ImGui::BeginListBox("##HeadPart", ImVec2(300, 150)))
	{
		_int index = 0;
		for (auto iter = m_mapHead.begin(); iter != m_mapHead.end(); iter++, index++)
		{
			size_t find_pos = iter->first.find(m_strGroup);
			if (find_pos == std::wstring::npos) continue;

			if (ImGui::Selectable(CAsUtils::ToString(iter->first).c_str(), index == iCurrIndex))
			{
				strcpy_s(m_szHeadPartName, CAsUtils::ToString(iter->first).c_str());
				m_iCurHeadIndex = index;
			}
		}
		ImGui::EndListBox();
	}

	string strPart = m_szHeadPartName;
	wstring strFind = CAsUtils::ToWString(strPart);
	if (TEXT("") != strFind)
	{
		CComponent* pModel = static_cast<CModel*>(m_mapHead.find(strFind)->second)->Clone(nullptr);
		m_pMannequin->Set_ModelPart(CMannequin::MODELTYPE::HEAD, static_cast<CModel*>(pModel));
		m_NpcCreateDesc.strNpcHead = strFind;
	}
}

void CNpcTool::Body(const _float& fTimeDelta)
{
	ImGui::SeparatorText("Select Body Part");

	_int iCurrIndex = m_iCurBodyIndex;
	if (ImGui::BeginListBox("##BodyPart", ImVec2(300, 150)))
	{
		_int index = 0;
		for (auto iter = m_mapBody.begin(); iter != m_mapBody.end(); iter++, index++)
		{
			size_t find_pos = iter->first.find(m_strGroup);
			if (find_pos == std::wstring::npos) continue;

			if (ImGui::Selectable(CAsUtils::ToString(iter->first).c_str(), index == iCurrIndex))
			{
				strcpy_s(m_szBodyPartName, CAsUtils::ToString(iter->first).c_str());
				m_iCurBodyIndex = index;
			}
		}
		ImGui::EndListBox();
	}

	string strPart = m_szBodyPartName;
	wstring strFind = CAsUtils::ToWString(strPart);
	if (TEXT("") != strFind)
	{
		CComponent* pModel = static_cast<CModel*>(m_mapBody.find(strFind)->second)->Clone(nullptr);

		m_pMannequin->Set_ModelPart(CMannequin::MODELTYPE::BODY, static_cast<CModel*>(pModel));
		m_NpcCreateDesc.strNpcBody = strFind;
	}
}

void CNpcTool::Create_Npc(const _float& fTimeDelta)
{
	if (TEXT("") == m_NpcCreateDesc.strIdleAnim || (TEXT("None") == m_NpcCreateDesc.strIdleAnim))
	{
		MessageBox(g_hWnd, L"IdleAnim 지정필요", L"확인", MB_OK);
		return;
	}
	if ((CNpc::NPCSHAPE::SOL != (CNpc::NPCSHAPE)m_NpcCreateDesc.iNpcShape && CNpc::NPCSHAPE::SP != (CNpc::NPCSHAPE)m_NpcCreateDesc.iNpcShape) &&
		(TEXT("") == m_NpcCreateDesc.strNpcHead || (TEXT("None") == m_NpcCreateDesc.strNpcHead || 
		(TEXT("") == m_NpcCreateDesc.strNpcBody) || (TEXT("None") == m_NpcCreateDesc.strNpcBody))))
	{
		MessageBox(g_hWnd, L"Part 지정필요", L"확인", MB_OK);
		return;
	}

	if ((_uint)CNpc::NPCTYPE::DECO == m_NpcCreateDesc.iNpcType)
	{
		m_pMannequin->Get_TransformCom()->Set_WorldMatrix(XMMatrixIdentity());
		m_pMannequin->Get_TransformCom()->Set_Scale(m_vNpcScale);
		m_pMannequin->Get_TransformCom()->My_Rotation(m_vNpcRot);
		m_pMannequin->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vNpcPos);
		m_NpcCreateDesc.matStart = m_pMannequin->Get_TransformCom()->Get_WorldMatrix();

		if (TEXT("") == m_NpcCreateDesc.strNpcName)
		{
			m_NpcCreateDesc.strNpcName = TEXT("None");
		}

		CGameObject* pInstance = m_pGameInstance->Add_GameObject((_uint)LEVELID::LEVEL_TOOL_NPC, (_uint)LAYER_TYPE::LAYER_NPC,
			TEXT("Prototype_GameObject_DecoNpc"), &m_NpcCreateDesc);

		m_vecNpcDesc.push_back(m_NpcCreateDesc);
		Clear_Info();
		m_pMannequin->Clear_MQ();
	}
	else
	{

	}
}

HRESULT CNpcTool::Save_Npc(const _float& fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (auto& pDesc : m_vecNpcDesc)
	{
		wstring strPath = fs::path("../Bin/Resources/ObjectData/Npc/" + CAsUtils::ToString(pDesc.strNpcTag )+ ".dat");

		shared_ptr<CAsFileUtils> SaveObject = make_shared<CAsFileUtils>();
		SaveObject->Open(strPath, FileMode::Write);

		SaveObject->Write<_uint>(pDesc.iNpcType);
		SaveObject->Write<string>(CAsUtils::W2S(pDesc.strNpcTag));
		SaveObject->Write<Matrix>(pDesc.matStart);

		SaveObject->Write<_uint>(pDesc.iNpcShape);
		SaveObject->Write<string>(CAsUtils::W2S(pDesc.strNpcMq));
		SaveObject->Write<string>(CAsUtils::W2S(pDesc.strNpcHead));
		SaveObject->Write<string>(CAsUtils::W2S(pDesc.strNpcBody));

		SaveObject->Write<_bool>(pDesc.IsMove);
		SaveObject->Write<_uint>(pDesc.vecMovePos.size());
		for (size_t i = 0; i < pDesc.vecMovePos.size(); i++)
		{
			SaveObject->Write<Vec3>(pDesc.vecMovePos[i]);
		}

		SaveObject->Write<_bool>(pDesc.bUseWeaponPart);
		SaveObject->Write<string>(CAsUtils::W2S(pDesc.strLeftPart));
		SaveObject->Write<Matrix>(pDesc.Left_OffsetMatrix);
		SaveObject->Write<string>(CAsUtils::W2S(pDesc.strRightPart));
		SaveObject->Write<Matrix>(pDesc.Right_OffsetMatrix);

		SaveObject->Write<_bool>(pDesc.IsTalk);
		{
			shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();

			fs::path strPath = fs::path("../Bin/Resources/ObjectData/Npc/" + CAsUtils::ToString(pDesc.strNpcTag) + ".xml");

			tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
			document->LinkEndChild(decl);

			tinyxml2::XMLElement* root = document->NewElement("NPCDESC");
			document->LinkEndChild(root);

			tinyxml2::XMLElement* node = document->NewElement("Info");
			tinyxml2::XMLElement* element = nullptr;

			root->LinkEndChild(node);
			{
				element = document->NewElement("Name");
				element->SetText(CAsUtils::W2S(pDesc.strNpcName).c_str());
				node->LinkEndChild(element);

				element = document->NewElement("IdleAnim");
				element->SetText(CAsUtils::W2S(pDesc.strIdleAnim).c_str());
				node->LinkEndChild(element);

				element = document->NewElement("ActAnim");
				element->SetText(CAsUtils::W2S(pDesc.strActAnim).c_str());
				node->LinkEndChild(element);

				element = document->NewElement("ChangeAnim");
				element->SetAttribute("Time", pDesc.fChangeAnimTime);
				node->LinkEndChild(element);
			}

			node = document->NewElement("Script");
			root->LinkEndChild(node);
			{
				element = document->NewElement("Script");
				element->SetAttribute("Time", pDesc.fTalkStartTime);
				node->LinkEndChild(element);

				element = document->NewElement("Talk");
				element->SetAttribute("Size", (_uint)pDesc.vecTalks.size());
				node->LinkEndChild(element);

				_uint iIndex = 1;
				for (size_t i = 0; i < pDesc.vecTalks.size(); i++)
				{
					string elementName = "Talk" + to_string(iIndex);
					element = document->NewElement(elementName.c_str());
					element->SetText(CAsUtils::W2S(pDesc.vecTalks[i]).c_str());
					node->LinkEndChild(element);
					
					iIndex++;
				}
			}

			node = document->NewElement("Sound");
			root->LinkEndChild(node);
			{
				element = document->NewElement("Sound");
				element->SetAttribute("Size", (_uint)pDesc.vecTalkSound.size());
				node->LinkEndChild(element);

				_uint iIndex = 1;
				for (size_t i = 0; i < pDesc.vecTalkSound.size(); i++)
				{
					string elementName = "Sound" + to_string(iIndex);
					element = document->NewElement(elementName.c_str());
					element->SetText(CAsUtils::W2S(pDesc.vecTalkSound[i]).c_str());
					node->LinkEndChild(element);

					iIndex++;
				}
			}
		
			tinyxml2::XMLError error = document->SaveFile(m_pUtils->ToString(strPath).c_str());
			if (error != tinyxml2::XML_SUCCESS)
			{
				MessageBox(g_hWnd, L"Npc 저장 실패", L"확인", MB_OK);
				return E_FAIL;
			}
		}
	}

	MessageBox(g_hWnd, L"Npc 저장 성공", L"확인", MB_OK);

	RELEASE_INSTANCE(CGameInstance);
}

void CNpcTool::Clear_Info()
{
	m_bSelected = false;
	m_iSelectNpType = -1;
	ZeroMemory(&m_szNpcTag, sizeof(MAX_PATH));
	ZeroMemory(&m_szNpcName, sizeof(MAX_PATH));

	m_vNpcScale = Vec3().Zero;
	m_vNpcRot = Vec3().Zero;
	m_vNpcPos = Vec3().Zero;

	m_iSelectPart = -1;
	m_vWpRightScale = Vec3(100.f, 100.f, 100.f);
	m_vWpRightPos = Vec3().Zero;
	m_vWpRightRot = Vec3().Zero;
	m_vWpLeftScale = Vec3(100.f, 100.f, 100.f);
	m_vWpLeftPos = Vec3().Zero;
	m_vWpLeftRot = Vec3().Zero;
	m_bSetWpLeftPart = false;
	m_bSetWpRightPart = false;
	ZeroMemory(&m_szRightWpName, sizeof(MAX_PATH));
	ZeroMemory(&m_szLeftWpName, sizeof(MAX_PATH));
	m_iCurWpLeftIndex = -1;
	m_iCurWpRightIndex = -1;

	ZeroMemory(&m_szHeadPartName, sizeof(MAX_PATH));
	m_iCurHeadIndex = -1;
	ZeroMemory(&m_szBodyPartName, sizeof(MAX_PATH));
	m_iCurBodyIndex = -1;

	m_iCurrAnimation = 0;


	m_IsMove = { false };
	m_vMovePos = Vec3().Zero;
	m_vecMovePos.clear();

	

	m_IsTalk = false;
	m_fTalkStartTime = 0.f;
	m_iCurTalk = -1;
	ZeroMemory(&m_szTalk, sizeof(MAX_PATH));
	m_vecTalks.clear();
	m_vecSelectTalk.clear();


	m_NpcCreateDesc.IsTalk = false;
	m_NpcCreateDesc.vecTalks.clear();
	m_NpcCreateDesc.vecTalkSound.clear();

	m_NpcCreateDesc.IsMove = false;
	m_NpcCreateDesc.vecMovePos.clear();

	m_NpcCreateDesc.bUseWeaponPart = false;
	m_NpcCreateDesc.fChangeAnimTime = 0.f;

	m_NpcCreateDesc.IsMove = false;
	m_NpcCreateDesc.vecMovePos.clear();
	m_NpcCreateDesc.strNpcTag = { TEXT("None") };
	m_NpcCreateDesc.strNpcName = { TEXT("None") };
	m_NpcCreateDesc.strNpcMq = { TEXT("None") };
	m_NpcCreateDesc.strNpcHead = { TEXT("None") };
	m_NpcCreateDesc.strNpcBody = { TEXT("None") };
	m_NpcCreateDesc.strIdleAnim = { TEXT("None") };
	m_NpcCreateDesc.strActAnim = { TEXT("None") };
	m_NpcCreateDesc.strLeftPart = { TEXT("None") };
	m_NpcCreateDesc.strRightPart = { TEXT("None") };
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
	Safe_Release(m_pTransformLeftPartCom);
	Safe_Release(m_pTransformRIghtPartCom);
}
