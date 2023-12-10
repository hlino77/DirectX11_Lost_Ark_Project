#pragma once

enum class KEY_STATE
{
	NONE,
	TAP,
	HOLD,
	AWAY
};

enum class KEY
{
	LEFT_ARROW,
	RIGHT_ARROW,
	UP_ARROW,
	DOWN_ARROW,
	TAB,
	OPEN_SQUARE_BRACKET,
	CLOSE_SQUARE_BRACKET,
	F1, F2, F3, F4, F5, F6, F7, F8, F9,
	INSERT, DEL, HOME, END_KEY, PAGE_UP, PAGE_DOWN,
	NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	ALT, CTRL, SHIFT,
	SPACE, ENTER, ESC, BACK,
	LBTN, RBTN,
	TYPE_END
};

enum DIMK { DIMK_LBUTTON, DIMK_RBUTTON, DIMK_WHEEL, DIMK_X, DIMK_END };
enum DIMM { DIMM_X, DIMM_Y, DIMM_WHEEL, DIMM_END };


enum aiTextureType
{
	aiTextureType_NONE = 0,
	aiTextureType_DIFFUSE = 1,
	aiTextureType_SPECULAR = 2,
	aiTextureType_AMBIENT = 3,
	aiTextureType_EMISSIVE = 4,
	aiTextureType_HEIGHT = 5,
	aiTextureType_NORMALS = 6,
	aiTextureType_SHININESS = 7,
	aiTextureType_OPACITY = 8,
	aiTextureType_DISPLACEMENT = 9,
	aiTextureType_LIGHTMAP = 10,
	aiTextureType_REFLECTION = 11,
	aiTextureType_BASE_COLOR = 12,
	aiTextureType_NORMAL_CAMERA = 13,
	aiTextureType_EMISSION_COLOR = 14,
	aiTextureType_METALNESS = 15,
	aiTextureType_DIFFUSE_ROUGHNESS = 16,
	aiTextureType_AMBIENT_OCCLUSION = 17,
	aiTextureType_UNKNOWN = 18,
	_aiTextureType_Force32Bit = INT_MAX
};
#define AI_TEXTURE_TYPE_MAX  aiTextureType_UNKNOWN