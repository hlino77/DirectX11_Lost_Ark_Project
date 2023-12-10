#include "Event.h"

void CEvent::Free()
{
}

CEvent::CEvent(_uint iID, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_iEventID(iID)
	, m_pDevice(pDevice)
	, m_pContext(pContext)
{
}
