#pragma once
#include "ToolBase.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CUtils;

END

BEGIN(Client)

class CEffectTool : public CToolBase
{
    using Super = CToolBase;
private:
	CEffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEffectTool() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr)	override;
	virtual HRESULT Tick()					override;
	virtual HRESULT LateTick()				override;
	virtual HRESULT	DebugRender()			override;

private:
	void	Input();
	//void	PlaceObject(const LAYER_TYPE& eLayerTag, const wstring& strPrototypeTag, const Vec3& vPickPosition);
	void	PlaceObject(const LAYER_TYPE& eLayerTag, const wstring& strPrototypeTag, const Vec3& vPickPosition);

private:
	void	InfoView();
	void	TreeGroups();

	void	Tex_Mask();
	void	Tex_Noise();
	void	Tex_Gradient();
	void	Tex_Ex();

private:
	HRESULT	LoadMeshes();
	HRESULT	LoadTextures();

private:
	_int					m_Item_Current = 0;

	vector<const _char*>	m_vecMeshes;

	vector<ImTextureID>		m_vecMaskTextureIDs;
	vector<ImTextureID>		m_vecNoiseTextureIDs;
	vector<ImTextureID>		m_vecGradientTextureIDs;
	vector<ImTextureID>		m_vecExtraTextureIDs;

	ImTextureID				m_pTextureMask		= nullptr;
	ImTextureID				m_pTextureNoise		= nullptr;
	ImTextureID				m_pTextureGradient	= nullptr;
	ImTextureID				m_pTextureEx0		= nullptr;
	ImTextureID				m_pTextureEx1		= nullptr;

	CUtils*					m_pUtils			= nullptr;

public:
	static class CEffectTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END