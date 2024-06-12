#include "stdafx.h"
#include "CameraTool.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>
#include "AsFileUtils.h"
#include "Utils.h"
#include "AsUtils.h"
#include "Transform.h"
#include "Camera_Free.h"
#include "Level_Tool.h"
#include "DebugDraw.h"
#include "ColliderSphere.h"



namespace fs = std::filesystem;

CCameraTool::CCameraTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
}

HRESULT CCameraTool::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	m_pCamera = dynamic_cast<CCamera_Free*>(CGameInstance::GetInstance()->Find_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, L"Prototype_GameObject_Camera_Free"));

	m_pDummyCollider = CSphereCollider::Create(m_pDevice, m_pContext);

	return S_OK;
}

HRESULT CCameraTool::Tick(_float fTimeDelta)
{
	Update_Tool(fTimeDelta);

	return S_OK;
}

HRESULT CCameraTool::LateTick(_float fTimeDelta)
{

	return S_OK;
}

HRESULT CCameraTool::DebugRender()
{	
	Render_DummyCollider();

	return S_OK;
}

void CCameraTool::Update_CameraTool(_float fTimeDelta)
{
	ImGui::Begin("Camera CutList");

	ImGui::InputText("CutName", m_szText, MAX_PATH);


	if (ImGui::Button("New Cut"))
	{
		wstring szName = CAsUtils::ToWString(string(m_szText));
		m_CutList.emplace(szName, CameraScene());
	}

	if (ImGui::Button("Delete"))
	{
		if (m_pCurrCameraPos)
		{
			_uint iIndex = 0;
			for (auto& iter = m_CutList.begin(); iter != m_CutList.end();)
			{
				if (&(*iter).second == m_pCurrCameraPos)
				{
					iter = m_CutList.erase(iter);
					if (iter == m_CutList.end())
					{
						m_pCurrCameraPos = nullptr;
					}
					else
					{
						m_pCurrCameraPos = &(*iter).second;
					}
					break;
				}
				else
				{
					++iter;
				}
			}
		}
	}


	if (ImGui::BeginListBox("CutList", ImVec2(200, 300)))
	{
		for (auto& Cut : m_CutList)
		{
			_bool IsSelected = (&Cut.second == m_pCurrCameraPos);
			if (ImGui::Selectable(CAsUtils::ToString(Cut.first).c_str(), IsSelected))
			{
				m_pCurrCameraPos = &Cut.second;
			}
		}
		ImGui::EndListBox();
	}

	ImGui::End();
}

void CCameraTool::Update_CameraPosTool(_float fTimeDelta)
{
	if (m_pCurrCameraPos)
	{
		ImGui::Begin("Camera PosList");

		ImGui::Text("DummyCollider");

		{
			Vec3 vDummyPos = m_pDummyCollider->GetBoundingSphere().Center;
			ImGui::DragFloat3("DummyPos", &vDummyPos.x, 0.01f);
			m_pDummyCollider->GetBoundingSphere().Center = vDummyPos;
			ImGui::DragFloat("DummyRadius", &m_pDummyCollider->GetBoundingSphere().Radius, 0.1f);
		}
		

		

		if (ImGui::Button("LookAt Dummy"))
		{
			m_pCamera->Get_TransformCom()->LookAt(m_pDummyCollider->GetBoundingSphere().Center);
		}

		if (ImGui::Button("Capture"))
		{
			CameraPos tDesc;
			tDesc.vPos = m_pCamera->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			tDesc.vLook = m_pCamera->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			m_pCurrCameraPos->PosList.push_back(tDesc);
		}

		if (KEY_HOLD(KEY::TAB) && KEY_TAP(KEY::W))
		{
			CameraPos tDesc;
			tDesc.vPos = m_pCamera->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			tDesc.vLook = m_pCamera->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			m_pCurrCameraPos->PosList.push_back(tDesc);
		}

		if (ImGui::Button("Delete"))
		{
			if (m_pCurrPos)
			{
				_uint iIndex = 0;
				for (auto& iter = m_pCurrCameraPos->PosList.begin(); iter != m_pCurrCameraPos->PosList.end();)
				{
					if (&(*iter) == m_pCurrPos)
					{
						iter = m_pCurrCameraPos->PosList.erase(iter);
						if (iter == m_pCurrCameraPos->PosList.end())
						{
							m_iCurrPosIndex = 0;
							m_pCurrPos = nullptr;
						}
						else
						{
							m_pCurrPos = &(*iter);
							m_iCurrPosIndex = iIndex;
						}

						break;
					}
					else
					{
						++iter;
						++iIndex;
					}
				}
			}
		}


		if (ImGui::BeginListBox("PosList", ImVec2(200, 300)))
		{
			_uint iIndex = 0;
			for (auto& Pos : m_pCurrCameraPos->PosList)
			{
				_bool IsSelected = (&Pos == m_pCurrPos);
				if (ImGui::Selectable(to_string(iIndex).c_str(), IsSelected))
				{
					m_pCurrPos = &Pos;
					m_iCurrPosIndex = iIndex;
					m_pCamera->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_pCurrPos->vPos);
					m_pCamera->Get_TransformCom()->LookAt(m_pCurrPos->vPos + m_pCurrPos->vLook);
				}

				++iIndex;
			}
			ImGui::EndListBox();
		}

		if (m_pCurrPos)
		{
			if (ImGui::Button("Set LookAt Dummy"))
			{
				Vec3 vTargetPos = m_pDummyCollider->GetBoundingSphere().Center;
				Vec3 vLook = vTargetPos - m_pCurrPos->vPos;
				vLook.Normalize();
				m_pCurrPos->vLook = vLook;
			}

			ImGui::InputFloat("Speed", &m_pCurrPos->fSpeed);
			ImGui::InputFloat3("Pos", &m_pCurrPos->vPos.x);
			ImGui::InputFloat3("Look", &m_pCurrPos->vLook.x);
		}

		Update_CutScene(fTimeDelta);

		ImGui::End();
	}
}

void CCameraTool::Update_Tool(_float fTimeDelta)
{

	Update_CameraTool(fTimeDelta);

	Update_CameraPosTool(fTimeDelta);

	Update_SaveLoad();
}

void CCameraTool::Update_CutScene(_float fTimeDelta)
{
	if (m_bTestStart)
	{
		m_fTimeRatio += m_fCameraSpeed * fTimeDelta;

		if (m_fTimeRatio >= 1.0f)
		{	
			++m_iCurrTestPosIndex;
			if (m_iCurrTestPosIndex >= m_CutList[m_szCurrTestCut].PosList.size() - 2)
			{
				m_bTestStart = false;
				m_fTimeRatio = 1.0f;
				--m_iCurrTestPosIndex;
			}
			else
				m_fTimeRatio -= 1.0f;
		}

		Vec3 vPos1 = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex - 1].vPos;
		Vec3 vPos2 = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex].vPos;
		Vec3 vPos3 = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex + 1].vPos;
		Vec3 vPos4 = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex + 2].vPos;

		Vec3 vLook1 = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex - 1].vLook;
		Vec3 vLook2 = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex].vLook;
		Vec3 vLook3 = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex + 1].vLook;
		Vec3 vLook4 = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex + 2].vLook;

		Vec3 vPos = XMVectorCatmullRom(vPos1, vPos2, vPos3, vPos4, m_fTimeRatio);
		Vec3 vLook = XMVectorCatmullRom(vLook1, vLook2, vLook3, vLook4, m_fTimeRatio);
		m_fCameraSpeed = CAsUtils::Lerpf(m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex].fSpeed, m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex + 1].fSpeed, m_fTimeRatio);

		Set_Camera(vPos, vLook);
	}


	if (ImGui::Button("Start"))
	{
		for (auto& Cut : m_CutList)
		{
			if (&Cut.second == m_pCurrCameraPos)
			{
				if (m_pCurrCameraPos->PosList.size() >= 4 && m_iCurrPosIndex != 0 && m_iCurrPosIndex < m_pCurrCameraPos->PosList.size() - 2)
				{
					m_iCurrTestPosIndex = m_iCurrPosIndex;
					m_szCurrTestCut = Cut.first;
					m_bTestStart = true;
					m_fTimeRatio = 0.0f;
					m_fCameraSpeed = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex].fSpeed;

					Vec3 vPos = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex].vPos;
					Vec3 vLook = m_CutList[m_szCurrTestCut].PosList[m_iCurrTestPosIndex].vLook;
					Set_Camera(vPos, vLook);
				}
			}
		}
	}

	if (ImGui::Button("Stop"))
	{
		m_bTestStart = false;
	}
}

void CCameraTool::Update_SaveLoad()
{
	ImGui::Begin("Camera Data Files");

	const _char* hint = "Enter the Name of New file";
	ImGui::InputTextWithHint("NewFileName", hint, m_szDataText, IM_ARRAYSIZE(m_szDataText));

	wstring strDirectory = TEXT("../Bin/Resources/CameraData/");

	if (ImGui::Button("Save"))
	{
		Save();
	}ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		Load();
	}

	vector<const _char*> vecItems;

	if (fs::exists(strDirectory) && fs::is_directory(strDirectory))
	{
		for (const auto& entry : fs::directory_iterator(strDirectory))
		{
			s2cPushBack(vecItems, CAsUtils::ToString(entry.path().filename()));
		}
	}
	
	ImGui::ListBox("Saved Data", &m_iSelectedDataIndex, vecItems.data(), vecItems.size(), 5);
	if (!vecItems.empty())
		m_szSelectedFileName = CAsUtils::ToWString(vecItems[m_iSelectedDataIndex]);

	ImGui::End();

}

void CCameraTool::Save()
{
	CAsFileUtils* pFileUtil = new CAsFileUtils();
	wstring strDirectory = TEXT("../Bin/Resources/CameraData/");
	strDirectory += CAsUtils::ToWString(string(m_szDataText)) + L".data";

	pFileUtil->Open(strDirectory, FileMode::Write);

	pFileUtil->Write<_uint>(m_CutList.size());

	for (auto& Scene : m_CutList)
	{
		pFileUtil->Write<string>(CAsUtils::ToString(Scene.first));
		pFileUtil->Write<_uint>(Scene.second.PosList.size());

		for (auto& Pos : Scene.second.PosList)
		{
			pFileUtil->Write<_float>(Pos.fSpeed);

			pFileUtil->Write<_float>(Pos.vPos.x);
			pFileUtil->Write<_float>(Pos.vPos.y);
			pFileUtil->Write<_float>(Pos.vPos.z);

			pFileUtil->Write<_float>(Pos.vLook.x);
			pFileUtil->Write<_float>(Pos.vLook.y);
			pFileUtil->Write<_float>(Pos.vLook.z);
		}
	}

	Safe_Delete(pFileUtil);
}

void CCameraTool::Load()
{
	CAsFileUtils* pFileUtil = new CAsFileUtils();
	wstring strDirectory = TEXT("../Bin/Resources/CameraData/");
	strDirectory += m_szSelectedFileName;

	filesystem::path Path(strDirectory);

	if (filesystem::exists(Path))
	{
		m_CutList.clear();

		pFileUtil->Open(strDirectory, FileMode::Read);

		_uint iSize = pFileUtil->Read<_uint>();

		for (_uint i = 0; i < iSize; ++i)
		{
			wstring szName = CAsUtils::ToWString(pFileUtil->Read<string>());
			_uint iPosSize = pFileUtil->Read<_uint>();

			m_CutList.emplace(szName, CameraScene());

			for (_uint i = 0; i < iPosSize; ++i)
			{
				_float fSpeed = pFileUtil->Read<_float>();

				Vec3 vPos;
				vPos.x = pFileUtil->Read<_float>();
				vPos.y = pFileUtil->Read<_float>();
				vPos.z = pFileUtil->Read<_float>();

				Vec3 vLook;
				vLook.x = pFileUtil->Read<_float>();
				vLook.y = pFileUtil->Read<_float>();
				vLook.z = pFileUtil->Read<_float>();

				CameraPos tDesc;
				tDesc.fSpeed = fSpeed;
				tDesc.vPos = vPos;
				tDesc.vLook = vLook;

				m_CutList[szName].PosList.push_back(tDesc);
			}
		}
	}
	

	Safe_Delete(pFileUtil);
}

void CCameraTool::Set_Camera(Vec3 vPos, Vec3 vLook)
{
	m_pCamera->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
	m_pCamera->Get_TransformCom()->LookAt(vPos + vLook);
}

void CCameraTool::Render_DummyCollider()
{
	m_pDummyCollider->DebugRender();
}



CCameraTool* CCameraTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCameraTool* pInstance = new CCameraTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CNpcTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCameraTool::Free()
{
	Super::Free();
}
