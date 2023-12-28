#include "BT_Action.h"
#include "GameObject.h"
#include "Model.h"
#include "BehaviorTree.h"
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
		AnimDesc.iAnimIndex = m_pGameObject->Get_ModelCom()->Initailize_FindAnimation(AnimDesc.strAnimName, AnimDesc.fAnimSpeed);

		if (AnimDesc.iAnimIndex < 0)
			return E_FAIL;

		m_vecAnimDesc.push_back(AnimDesc);
	}
	m_eNodeType = BT_NODETYPE::ACTION;
	m_iLoopAnimationIndex = pActionDesc->iLoopAnimationIndex;
	m_fMaxLoopTime = pActionDesc->fMaxLoopTime;
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
		m_vecAnimDesc[iAnimIndex].iChangeFrame, m_vecAnimDesc[iAnimIndex].fRootDist);
	m_fFrame = 0.f;
	m_fLoopTime = 0;
	m_iCurrAnimation = 0;
	m_bSoundOn = false;
}

CBT_Node::BT_RETURN CBT_Action::OnUpdate(const _float& fTimeDelta)
{
	if (m_iLoopAnimationIndex != -1)
		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[m_iLoopAnimationIndex].iAnimIndex && m_fLoopTime < m_fMaxLoopTime)
		{
			m_fLoopTime += fTimeDelta;
			if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[m_iLoopAnimationIndex].iAnimIndex))
				m_pGameObject->Get_ModelCom()->Set_CurrAnim(m_vecAnimDesc[m_iLoopAnimationIndex].iAnimIndex);
		}

	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex)-2)
	{
		if (m_iCurrAnimation == m_vecAnimDesc.size() - 1)
			return BT_SUCCESS;
		else
		{
			m_iCurrAnimation++;
		
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime, 
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
		}
	}

	return BT_RUNNING;
}

void CBT_Action::Free()
{
	__super::Free();
}
