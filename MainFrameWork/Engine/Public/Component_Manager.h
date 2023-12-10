#pragma once

#include "PipeLine.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"


BEGIN(Engine)
class CComponent_Manager : public CBase
{
    DECLARE_SINGLETON(CComponent_Manager)

private:
    CComponent_Manager();
    virtual ~CComponent_Manager() = default;

public:
    HRESULT Reserve_Manager(_uint iNumLevels);
    HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strProtoTypeTag, class CComponent* pPrototype);
    class CComponent* Clone_Component(CGameObject* pObject, _uint iLevelIndex, const wstring& strProtoTypeTag, void* pArg);
    HRESULT Check_Prototype(_uint iLevelIndex, const wstring& strProtoTypeTag);

private:
    _uint   m_iNumLevels = 0;
    map<const wstring, class CComponent*>* m_Prototypes = nullptr;
    typedef map<const wstring, class CComponent*> PROTOTYPES;

private:
    class CComponent* Find_Component(_uint iLevelIndex, const wstring& strProtoTypeTag);

public:
    virtual void Free() override;

};

END

