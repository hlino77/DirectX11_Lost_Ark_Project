#pragma once

#include "PipeLine.h"
#include "Renderer.h"
#include "Transform.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Point.h"
#include "VIBuffer_Cube.h"
#include "VIBuffer_Particle.h"
#include "VIBuffer_RectSSAO.h"
#include "Hasher.h"
#include "Lock.h"

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

    unordered_map<const wstring, class CComponent*, djb2Hasher>* Get_Prototype(_uint iLevelIndex) { return &m_Prototypes[iLevelIndex]; }

private:
    _uint   m_iNumLevels = 0;
    unordered_map<const wstring, class CComponent*, djb2Hasher>* m_Prototypes = nullptr;
    typedef unordered_map<const wstring, class CComponent*, djb2Hasher> PROTOTYPES;

private:
    class CComponent* Find_Component(_uint iLevelIndex, const wstring& strProtoTypeTag);

    USE_LOCK
public:
    virtual void Free() override;

};

END

