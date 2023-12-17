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
		AnimDesc.iAnimIndex = m_pGameObject->Get_ModelCom()->Find_AnimIndex(AnimDesc.strAnimName);
		_uint iAnimFrame = m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(AnimDesc.iAnimIndex);
		if (AnimDesc.iAnimIndex < 0)
			return E_FAIL;

		m_vecAnimIndexFrame.push_back(make_pair(AnimDesc, iAnimFrame));
	}
	m_eNodeType = BT_NODETYPE::ACTION;
	m_iLoopAnimationIndex = pActionDesc->iLoopAnimationIndex;
	m_fMaxLoopTime = pActionDesc->fMaxLoopTime;
	m_iMaxAnimation = m_vecAnimIndexFrame.size();
	if (FAILED(pActionDesc->pBehaviorTree->Add_Action(pActionDesc->strActionName, this)))
		return E_FAIL;
	m_strActionName = pActionDesc->strActionName;
	return S_OK;
}

void CBT_Action::OnStart(_int iAnimIndex)
{
	__super::OnStart();

	m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimIndexFrame[iAnimIndex].first.iAnimIndex, 
		m_vecAnimIndexFrame[iAnimIndex].first.fChangeTime, m_vecAnimIndexFrame[iAnimIndex].first.iStartFrame, 
		m_vecAnimIndexFrame[iAnimIndex].first.iChangeFrame, m_vecAnimIndexFrame[iAnimIndex].first.fRootDist);
	m_fFrame = 0.f;
	m_fLoopTime = 0;
	m_iCurrAnimation = 0;
	m_bSoundOn = false;
}

CBT_Node::BT_RETURN CBT_Action::OnUpdate(const _float& fTimeDelta)
{
	if (m_iLoopAnimationIndex != -1)
		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimIndexFrame[m_iLoopAnimationIndex].first.iAnimIndex && m_fLoopTime > m_fMaxLoopTime)
		{
			m_fLoopTime += fTimeDelta;
			if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[m_iLoopAnimationIndex].first.iAnimIndex))
				m_pGameObject->Get_ModelCom()->Set_CurrAnim(m_vecAnimIndexFrame[m_iLoopAnimationIndex].first.iAnimIndex);
		}

	if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[m_iCurrAnimation].first.iAnimIndex))
	{
		if (m_iCurrAnimation == m_vecAnimIndexFrame.size() - 1)
			return BT_SUCCESS;
		else
		{
			m_iCurrAnimation++;
		
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimIndexFrame[m_iCurrAnimation].first.iAnimIndex, m_vecAnimIndexFrame[m_iCurrAnimation].first.fChangeTime, 
				m_vecAnimIndexFrame[m_iCurrAnimation].first.iStartFrame, m_vecAnimIndexFrame[m_iCurrAnimation].first.iChangeFrame);
		}
	}

	return BT_RUNNING;
}

void CBT_Action::Free()
{
	__super::Free();
}
