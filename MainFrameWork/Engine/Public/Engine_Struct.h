#ifndef Engine_Struct_h__
#define Engine_Struct_h__
#include "Engine_Enum.h"

namespace Engine
{
	typedef struct tagKeyFrame
	{
		float		fTime;

		SimpleMath::Vector3	vScale;
		SimpleMath::Vector4	vRotation;
		SimpleMath::Vector3	vPosition;

	}KEYFRAME;

	/* 빛의 정보를 담아놓기 위한 구조체. */
	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_POINT, TYPE_DIRECTIONAL, TYPE_END };

		TYPE			eType;

		SimpleMath::Vector4		vDirection;

		SimpleMath::Vector4		vPosition;
		float			fRange;

		SimpleMath::Vector4		vDiffuse;
		SimpleMath::Vector4		vAmbient;
		SimpleMath::Vector4		vSpecular;

	}LIGHTDESC;



	typedef struct tagMaterialDesc
	{
		char			strName[MAX_PATH];

		SimpleMath::Vector4		vAmbient;
		SimpleMath::Vector4		vDiffuse;
		SimpleMath::Vector4		vSpecular;
		SimpleMath::Vector4		vEmissive;
		class CTexture* pTexture[AI_TEXTURE_TYPE_MAX];
	}MATERIALDESC;

	typedef struct tagLineIndices16
	{
		unsigned short		_0, _1;
	}LINEINDICES16;

	typedef struct tagLineIndices32
	{
		unsigned long		_0, _1;
	}LINEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short		_0, _1, _2;
	}FACEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long		_0, _1, _2;
	}FACEINDICES32;

	///////////////
	/// VtxType ///
	///////////////
	// D3DDECLUSAGE
	typedef struct tagVertexTexture
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector2		vTexture;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertexTexture_Declaration
	{
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXTEX_DECLARATION;

	typedef struct tagVertexNormalTexture
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector3		vNormal;
		SimpleMath::Vector2		vTexture;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertexNormalTexture_Declaration
	{
		static const unsigned int iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXNORTEX_DECLARATION;

	typedef struct tagVertexModel
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector3		vNormal;
		SimpleMath::Vector2		vTexture;
		SimpleMath::Vector3		vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagVertexModel_Declaration
	{
		static const unsigned int iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXMODEL_DECLARATION;

	typedef struct tagVertexAnimModel
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector3		vNormal;
		SimpleMath::Vector2		vTexture;
		SimpleMath::Vector3		vTangent;
		XMUINT4			vBlendIndex; /* 이 정점에 영향을 주는 뼈의 인덱스 네개. */
		SimpleMath::Vector4		vBlendWeight; /* 영향르 주고 있는 각 뼈대의 영향 비율 */
	}VTXANIMMODEL;

	typedef struct ENGINE_DLL tagVertexAnimModel_Declaration
	{
		static const unsigned int iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXANIMMODEL_DECLARATION;




	typedef struct tagVertexCubeTexture
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector3		vTexture;
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertexCubeTexture_Declaration
	{
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXCUBETEX_DECLARATION;


	typedef struct tagVertexColor
	{
		SimpleMath::Vector3		vPosition;
		SimpleMath::Vector4		vColor;
	}VTXCOL;

	typedef struct ENGINE_DLL tagVertexColor_Declaration
	{
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	}VTXCOL_DECLARATION;


	typedef struct tagGraphicDesc
	{
		enum WINMODE { WINMODE_FULL, WINMODE_WIN, WINMODE_END };

		HWND		hWnd;
		WINMODE		eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;

	} GRAPHIC_DESC;

	typedef struct tagMyHierarchyNode
	{
		char cName[MAX_PATH];
		char cParent[MAX_PATH];
		int  iDepth;
		SimpleMath::Matrix matTransform;
	} DATA_NODE;

	typedef struct tagMyMaterialData
	{
		char cNames[AI_TEXTURE_TYPE_MAX][MAX_PATH];
	} DATA_MATERIAL;

	typedef struct tagMyBoneData
	{
		char		cNames[MAX_PATH];
		SimpleMath::Matrix		matOffset;
	} DATA_BONE;

	typedef struct tagMyMeshData
	{
		char				cName[MAX_PATH];
		int					iMaterialIndex;

		int					NumVertices;
		// VTXMODEL* pNonAnimVertices;
		// VTXANIMMODEL* pAnimVertices;

		int					iNumPrimitives;
		// FACEINDICES32* pIndices;

		int					iNumBones;
		DATA_BONE* pBones;

	} DATA_MESH;

	typedef struct tagMyChannelData
	{
		char szName[MAX_PATH];
		int iNumKeyFrames;
		KEYFRAME* pKeyFrames;
	} DATA_CHANNEL;

	typedef struct tagAnimData
	{
		char szName[MAX_PATH];

		bool bLoop;
		int	iNumChannels;

		float fDuration;
		float fTickPerSecond;

		DATA_CHANNEL* pChannel;
	} DATA_ANIM;

	typedef struct tagSceneData
	{

		int iNodeCount;
		DATA_NODE* pNodesData;

		int iMaterialCount;
		DATA_MATERIAL* pMaterialData;

		int iMeshCount;
		DATA_MESH* pMeshData;

		int iNumAnimations;
		DATA_ANIM* pAnimData;

	} DATA_SCENE;


	typedef struct tagTriangleDesc
	{
		Vec3 vPos0, vPos1, vPos2;
		Vec3 vNormal;
		_float fDist;
	}TRIAGLEDESC;



	typedef struct ENGINE_DLL tagVertex_Instance
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;

		/* 셰이더에 이러한 정점이 잘 들어가냐? 라는 검증을 위해 */
		/* 이 정점 뿐만아니라 그리기용 정점도 같이 받아야되지. */
		/*static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];*/
	}VTXINSTANCE;


	typedef struct ENGINE_DLL tagVertex_PointEffect_Instance
	{
		XMFLOAT4		vTranslation;
		XMFLOAT4		vDirection;
		XMFLOAT4		vScale;
		XMFLOAT4		vColor;
		XMFLOAT4		vBlurColor;

		/* 셰이더에 이러한 정점이 잘 들어가냐? 라는 검증을 위해 */
		/* 이 정점 뿐만아니라 그리기용 정점도 같이 받아야되지. */
		/*static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];*/
	}VTXINSTANCE_POINTEFFECT;


	typedef struct ENGINE_DLL tagVertex_ModelEffect_Instance
	{
		XMFLOAT4		vRight;
		XMFLOAT4		vUp;
		XMFLOAT4		vLook;
		XMFLOAT4		vTranslation;
		XMFLOAT4		vColor;
		XMFLOAT4		vBlurColor;

		/* 셰이더에 이러한 정점이 잘 들어가냐? 라는 검증을 위해 */
		/* 이 정점 뿐만아니라 그리기용 정점도 같이 받아야되지. */
		/*static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];*/
	}VTXINSTANCE_MODELEFFECT;


	typedef struct ENGINE_DLL tagVertex_Effect_Instance
	{
		XMFLOAT4		vTranslation;
		XMFLOAT4		vDirection;
		XMFLOAT4		vScale;
		XMFLOAT4		vColor;


		/* 셰이더에 이러한 정점이 잘 들어가냐? 라는 검증을 위해 */
		/* 이 정점 뿐만아니라 그리기용 정점도 같이 받아야되지. */
		/*static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];*/
	}VTXINSTANCE_EFFECT;


	typedef struct ENGINE_DLL tagVertex_Model_Instance
	{
		static const unsigned int				iNumElements = 8;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTX_MODEL_INSTANCE;



	typedef struct ENGINE_DLL tagVertex_LineCircle_Instance
	{
		static const unsigned int				iNumElements = 5;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTX_LINECIRCLE_INSTANCE;


	typedef struct ENGINE_DLL tagVertex_Trail_Instance
	{
		static const unsigned int				iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTX_TRAIL_INSTANCE;



	typedef struct ENGINE_DLL tagVertex_ModelEffect_Declaration
	{
		static const unsigned int				iNumElements = 10;
		static const D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements];
	}VTX_MODELEFFECT_DECLARATION;


}



#endif // Engine_Struct_h__
