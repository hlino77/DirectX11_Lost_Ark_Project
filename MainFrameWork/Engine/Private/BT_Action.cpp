#include "BT_Action.h"
#include "GameObject.h"
#include "Model.h"
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
	return S_OK;
}

void CBT_Action::OnStart(_int iAnimIndex)
{
	__super::OnStart();
	m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimIndexFrame[iAnimIndex].first.iAnimIndex, 
		m_vecAnimIndexFrame[iAnimIndex].first.fChangeTime, m_vecAnimIndexFrame[iAnimIndex].first.iStartFrame, 
		m_vecAnimIndexFrame[iAnimIndex].first.iChangeFrame);
	m_fFrame = 0.f;
	m_bSoundOn = false;
}

void CBT_Action::Free()
{
	__super::Free();
}
