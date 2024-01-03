#pragma once
#include "ToolBase.h"
#include "Hasher.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CUtils;

END

BEGIN(Client)

class CVoidEffect;
class CLevel_Tool;
class CEffectTool : public CToolBase
{
    using Super = CToolBase;
private:
	CEffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEffectTool() = default;

public:
	virtual HRESULT Initialize(void* pArg = nullptr)	override;
	virtual HRESULT Tick(const _float& fTimeDelta)		override;
	virtual HRESULT LateTick(const _float& fTimeDelta)	override;
	virtual HRESULT	DebugRender()			override;

private:
	void	Input();

private:
	void	InfoView();
	void	ShowCurrentMaterials();

	void	SelectBaseMesh();
	void	SelectBaseTexture();
	void	SelectNoiseTexture();
	void	SelectMaskTexture();
	void	SelectEmissiveTexture();
	void	SelectDissolveTexture();

	void	SelectMesh(string& strCurrentCategory);
	pair<wstring, ID3D11ShaderResourceView*> SelectTexture(string& strCurrentCategory);

private:
	HRESULT EffectTool();
	HRESULT EffectDetail();
	HRESULT EffectsList();

	HRESULT	DataFiles();

	HRESULT CreateEffect();
	HRESULT Reset();

	HRESULT Save(_char* szFileName);
	HRESULT Load();

private:
	HRESULT	LoadMeshes();
	HRESULT	LoadTextures();

	HRESULT Ready_Camera();

private:
	_int					m_iCurrentEffectType	= -1; // Mesh = 0, Texture = 1, Type_End = -1;
	string					m_strCurrentCategory	= "";
	string					m_strCurrentMeshCategory= "";
	wstring					m_strCurrentPath		= TEXT("");
	wstring					m_strCurrentMeshPath	= TEXT("");
	_int					m_Item_Current			= 0;

	using MESHES = vector<pair<string, vector<string>>>;
	using TEXTURES = vector<pair<string, vector<pair<wstring, ID3D11ShaderResourceView*>>>>;
	MESHES					m_vecMeshes;
	TEXTURES				m_vecTextures;

	CVoidEffect*			m_pCurrentEffect = nullptr;
	vector<CVoidEffect*>	m_vecEffects;
	_int					m_iSelectedEffectIndex = 0;

	pair<wstring, string>									m_BaseMesh;

	pair<wstring, pair<wstring, ID3D11ShaderResourceView*>>	m_BaseTexture;
	pair<wstring, pair<wstring, ID3D11ShaderResourceView*>>	m_CurrentNoise;
	pair<wstring, pair<wstring, ID3D11ShaderResourceView*>>	m_CurrentMask;
	pair<wstring, pair<wstring, ID3D11ShaderResourceView*>>	m_CurrentEmissive;
	pair<wstring, pair<wstring, ID3D11ShaderResourceView*>>	m_CurrentDissolve;

	CUtils*					m_pUtils = nullptr;
	class CCamera_Free*		m_pCamera = nullptr;

	_bool		m_IsResetReserved = false;

	_int		m_iSelectedDataIndex = 0;
	wstring		m_strSelectedGroup = TEXT("");
	_char		m_szBundleNameBuf[128] = "";

public:
	static class CEffectTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CLevel_Tool* pLevel_Tool);
	virtual void Free() override;
};

END