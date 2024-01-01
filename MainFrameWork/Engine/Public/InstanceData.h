#pragma once
#include "Engine_Defines.h"
#include <future>

BEGIN(Engine)

typedef struct ENGINE_DLL InstanceDataDesc
{
	_uint iMaxInstanceCount = 0;
	ID3D11Buffer* pInstanceBuffer = nullptr;
	tagTypeLess* pInstanceValue = nullptr;
	class CShader* pInstanceShader = nullptr;

	tagTypeLess* pAnimInstanceValue = nullptr;
	ID3D11Texture2D* pAnimInstanceTexture = nullptr;
	ID3D11ShaderResourceView* pAnimSRV = nullptr;

	ID3D11DeviceContext* pInstanceContext = nullptr;
	ID3D11DeviceContext* pInstanceAnimContext = nullptr;
	future<HRESULT> Future_Instance;
	future<HRESULT> Future_AnimInstance;


	InstanceDataDesc() {};
	~InstanceDataDesc();
}INSTANCEDATA;

END