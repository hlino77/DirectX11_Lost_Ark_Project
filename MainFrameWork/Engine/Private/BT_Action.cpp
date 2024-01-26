#include "BT_Action.h"
#include "GameObject.h"
#include "Model.h"
#include "BehaviorTree.h"
#include "AsUtils.h"
CBT_Action::CBT_Action()
{
}

CBT_Action::CBT_Action(const CBT_Action& rhs)
{
}

HRESULT CBT_Action::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	ACTION_DESC* pActionDesc = static_cast<ACTION_DESC*> (pArg);
	if (pActionDesc == nullptr)
		return E_FAIL;
	for (auto& AnimDesc :  pActionDesc->vecAnimations)
	{
		AnimDesc.iAnimIndex = m_pGameObject->Get_ModelCom()->Initailize_FindAnimation(AnimDesc.strAnimName,1.f);

		if (AnimDesc.iAnimIndex < 0)
			return E_FAIL;

		m_vecAnimDesc.push_back(AnimDesc);
	}
	m_eNodeType = BT_NODETYPE::ACTION;

	m_iMaxAnimation = m_vecAnimDesc.size();

	if (FAILED(pActionDesc->pBehaviorTree->Add_Action(pActionDesc->strActionName, this)))
		return E_FAIL;
	m_strActionName = pActionDesc->strActionName;
	return S_OK;
}

void CBT_Action::OnStart(_int iAnimIndex)
{
	__super::OnStart();
	m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[iAnimIndex].iAnimIndex, 
		m_vecAnimDesc[iAnimIndex].fChangeTime, m_vecAnimDesc[iAnimIndex].iStartFrame, 
		m_vecAnimDesc[iAnimIndex].iChangeFrame, m_vecAnimDesc[iAnimIndex].fRootDist, m_vecAnimDesc[iAnimIndex].IsRootRot);
	m_fFrame = 0.f;
	m_fLoopTime = 0;
	m_iCurrAnimation = iAnimIndex;
	m_bSoundOn = false;
	m_bStart = true;
	m_bEnd = true;
}

CBT_Node::BT_RETURN CBT_Action::OnUpdate(const _float& fTimeDelta)
{
	if (m_vecAnimDesc[0].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim()&& m_bStart)
	{
		On_FirstAnimStart();
		m_bStart = false;
	}
	if (m_iCurrAnimation == m_iMaxAnimation - 1&&m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[m_iMaxAnimation - 1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iMaxAnimation - 1].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[m_iMaxAnimation - 1].iAnimIndex) - 3&& m_bEnd)
	{
		On_LastAnimEnd();
		m_bEnd = false;
	}
	if (m_vecAnimDesc[m_iCurrAnimation].bIsLoop&& m_vecAnimDesc[m_iCurrAnimation].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		if (m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime&& m_vecAnimDesc[m_iCurrAnimation].IsEndInstant)
		{
			if (m_iCurrAnimation == m_iMaxAnimation - 1)
				return BT_SUCCESS;
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_fLoopTime = 0;
		}
		else if(m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime && !m_vecAnimDesc[m_iCurrAnimation].IsEndInstant&& m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
		{
			if (m_iCurrAnimation == m_iMaxAnimation - 1)
				return BT_SUCCESS;
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_fLoopTime = 0;
		}
		m_fLoopTime += fTimeDelta;
	}
	else if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) - 3)
	{
		if (m_iCurrAnimation == m_iMaxAnimation - 1)
			return BT_SUCCESS;
		else
		{
			m_iCurrAnimation++;

			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_pGameObject->Get_ModelCom()->Set_Anim_Speed(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fAnimSpeed);
		}
	}

	return BT_RUNNING;
}

void CBT_Action::On_FirstAnimStart()
{

}

void CBT_Action::On_LastAnimEnd()
{

}

void CBT_Action::OnEnd()
{
	__super::OnEnd();
}

void CBT_Action::Free()
{
	__super::Free();
}
