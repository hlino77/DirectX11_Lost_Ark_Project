#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>


namespace Client
{
	enum LEVELID { LEVEL_STATIC, LEVEL_TOOL, LEVEL_LOGO, LEVEL_SERVERSELECT, LEVEL_LOBBY ,LEVEL_BERN, LEVEL_LOADING, LEVEL_END };

	enum OBJ_TYPE { PLAYER, MONSTER, BOSS, SKILL, TERRAIN, PROP, UI, COLMESH, EFFECT, SPAWNER, PART, OBJ_END };

	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;
	const float				g_fSizeRatio = 1.25f;
	extern float			g_fLoadingSizeX;

	enum class LAYER_TYPE
	{
		LAYER_COLMESH,
		LAYER_TERRAIN,
		LAYER_SKYBOX,
		LAYER_CAMERA,
		LAYER_BACKGROUND,
		LAYER_PLAYER,
		LAYER_MONSTER,
		LAYER_BOSS,
		LAYER_SKILL,
		LAYER_EFFECT,
		LAYER_UI,
		LAYER_END
	};

	enum class LAYER_COLLIDER
	{
		LAYER_BODY,
		LAYER_ATTACK,
		LAYER_CHILD,
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

	enum LEVELSTATE { LOADING, INITREADY, INITSTART, INITEND, GAMEPLAY, STATEEND };

	enum class EVENTSTATE { EVENTSTART, READY, EVENT, EVENTEND, SETEND, TYPEEND };

	enum class EVENT
	{
		EVENTEND
	};

	enum CHANNELID {
		CHANNEL_BGM,
		CHANNEL_UI, CHANNEL_BACKGROUND,
		CHANNEL_LOOPSTART,
		CHANNEL_END = 29
	};


	enum class CHR_CLASS { GUNSLINGER, SLAYER, CLASSEND };
}

extern _float g_fVolume;
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
using namespace Client;

#define SERVER_IP		L"26.80.185.93"
#define SERVER_PORT		7777