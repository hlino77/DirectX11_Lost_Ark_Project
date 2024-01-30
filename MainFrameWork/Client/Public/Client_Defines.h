#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>


namespace Client
{
	enum LEVELID 
	{ 
		LEVEL_STATIC, 
		LEVEL_TOOL, 
		LEVEL_LOGO, 
		LEVEL_SERVERSELECT, 
		LEVEL_LOBBY,
		LEVEL_BERN,
		LEVEL_CHAOS_1,
		LEVEL_CHAOS_2,
		LEVEL_CHAOS_3,
		LEVEL_VALTANENTERANCE,
		LEVEL_VALTANMAIN,
		LEVEL_LOADING,
		LEVEL_TOOL_NPC,
		LEVEL_END 
	};

	enum OBJ_TYPE { PLAYER, MONSTER, BOSS, NPC, SKILL, ESTHER, ITEM, TERRAIN, PROP, UI, COLMESH, EFFECT, SPAWNER, PART, PROJECTILE, DUNGEAN, OBJ_END };

	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;
	const float				g_fSizeRatio = 1.25f;
	extern float			g_fLoadingSizeX;

	enum class LAYER_TYPE
	{
		LAYER_TERRAIN,
		LAYER_SKYBOX,
		LAYER_CAMERA,
		LAYER_BACKGROUND,
		LAYER_PLAYER,
		LAYER_MONSTER,
		LAYER_BOSS,
		LAYER_NPC,
		LAYER_SKILL,
		LAYER_ESTHER,
		LAYER_ITEM,
		LAYER_EFFECT,
		LAYER_UI,
		LAYER_END
	};

	enum class LAYER_COLLIDER
	{
		LAYER_BODY_PLAYER,
		LAYER_BODY_MONSTER,
		LAYER_BODY_BOSS,
		LAYER_BODY_STATICMODEL,
		LAYER_BODY_NPC,
		LAYER_ATTACK_MONSTER,
		LAYER_ATTACK_PLAYER,
		LAYER_ATTACK_BOSS,
		LAYER_SKILL_PLAYER,
		LAYER_SKILL_BOSS,
		LAYER_GRAB_BOSS,
		LAYER_BUFF_PLAYER,
		LAYER_SKILL_ESTHER,
		LAYER_BUFF_ESTHER,
		LAYER_SPAWNER,
		LAYER_SAFEZONE,
		LAYER_CHILD,
		LAYER_SKILL_PLAYER_BUFF,
		LAYER_END
	};


	enum class COLLIDER_ATTACK
	{
		MIDDLE,
		LEFT,
		RIGHT,
		SPINBLOWUP,
		SPINBLOWDOWN,
		ATTACK_END
	};

	enum LEVELSTATE { INITREADY, PLAYERREADY, INITEND, STATEEND };

	enum class EVENTSTATE { EVENTSTART, READY, EVENT, EVENTEND, SETEND, TYPEEND };

	enum class EVENT
	{
		EVENTEND
	};

	enum CHANNELID {
		CHANNEL_BGM,
		CHANNEL_UI,
		CHANNEL_BACKGROUND,
		CHANNEL_LOOPSTART,
		CHANNEL_END = 29
	};

	enum class CHR_CLASS { GUNSLINGER, SLAYER, DESTROYER, BARD, CLASSEND };

	enum class STATUSEFFECT { COUNTER, GROGGY, EARTHQUAKE, BUG, FEAR, SHOCK, STUN, SILENCE ,  EFFECTEND };

	enum class SKILL_TYPE
	{
		SKILL_RISINGSUN,
		SKILL_TELEPORTDOOR,
		SKILLEND

	struct ITEM_SLOTDESC
	{
		vector<class CItem*> vecItems;
	};

	struct ITEMTAG_DESC
	{
		_bool	bOwn = { false };
		_int	iIndex = { -1 };
	};
}

#define EFFECT_START(name, pivotDesc)						CEffect_Manager::GetInstance()->Effect_Start(name, pivotDesc);
#define EFFECT_START_OUTLIST(name, pivotDesc, effectlist)	CEffect_Manager::GetInstance()->Effect_Start(name, pivotDesc, effectlist);
#define TRAIL_START(name, pivotGetter)						CEffect_Manager::GetInstance()->Trail_Start(name, pivotGetter);
#define TRAIL_START_OUTLIST(name, pivotGetter, effectlist)	CEffect_Manager::GetInstance()->Trail_Start(name, pivotGetter, effectlist);
#define EFFECT_LATE_START(name, pivotDescLate)				CEffect_Manager::GetInstance()->Effect_LateStart(name, pivotDescLate);

extern _float g_fVolume;
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
using namespace Client;


#define SERVER_IP		L"127.0.0.1"
#define SERVER_PORT		7777