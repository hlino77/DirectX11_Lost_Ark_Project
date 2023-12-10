#pragma once
#include "Engine_Defines.h"

using VertexType = tagVertexAnimModel;


struct asBone
{
	string name;
	int32 index = -1;
	int32 parent = -1;
	Matrix transform;
};

struct asMesh
{
	string name;
	vector<VertexType> vertices;
	vector<uint32> indices;

	int32 boneIndex;
	string materialName;
	int32 materialIndex;
};

struct asMaterial
{
	string name;
	Vec4 ambient;
	Vec4 diffuse;
	Vec4 specular;
	Vec4 emissive;
	string diffuseFile;
	string specularFile;
	string normalFile;
};

// Animation
struct asBlendWeight
{
	void Set(uint32 index, uint32 boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
		case 0: indices.x = i; weights.x = w; break;
		case 1: indices.y = i; weights.y = w; break;
		case 2: indices.z = i; weights.z = w; break;
		case 3: indices.w = i; weights.w = w; break;
		}
	}

	XMUINT4 indices = XMUINT4(0, 0, 0, 0);
	Vec4 weights = Vec4(0, 0, 0, 0);
};

// 정점마다 -> (관절번호, 가중치)
struct asBoneWeights
{
	void AddWeights(uint32 boneIndex, float weight)
	{
		if (weight <= 0.0f)
			return;

		auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
			[weight](const Pair& p) { return weight > p.second; });

		boneWeights.insert(findIt, Pair(boneIndex, weight));
	}

	asBlendWeight GetBlendWeights()
	{
		asBlendWeight blendWeights;

		for (uint32 i = 0; i < boneWeights.size(); i++)
		{
			if (i >= 4)
				break;

			blendWeights.Set(i, boneWeights[i].first, boneWeights[i].second);
		}

		return blendWeights;
	}

	void Normalize()
	{
		if (boneWeights.size() >= 4)
			boneWeights.resize(4);

		float totalWeight = 0.f;
		for (const auto& item : boneWeights)
			totalWeight += item.second;

		float scale = 1.f / totalWeight;
		for (auto& item : boneWeights)
			item.second *= scale;
	}

	using Pair = pair<int32, float>;
	vector<Pair> boneWeights;
};

struct asKeyframeData
{
	float time;
	Vec3 scale;
	Quaternion rotation;
	Vec3 translation;
};

struct asKeyframe
{
	string boneName;
	vector<asKeyframeData> transforms;
};

struct asAnimation
{
	string name;
	uint32 frameCount;
	float frameRate;
	float duration;
	vector<shared_ptr<asKeyframe>> keyframes;
};

// Cache
struct asAnimationNode
{
	vector<asKeyframeData> keyframe;
};


//Data Load

struct ModelBone
{
	wstring strName;
	int32 iIndex;
	int32 iParentID;
	shared_ptr<ModelBone> tParent; // Cache

	Matrix matTransform;
	vector<shared_ptr<ModelBone>> tChildrens; // Cache
};


struct ModelKeyframeData
{
	float fTime;
	Vec3 vScale;
	Quaternion vRotation;
	Vec3 vTranslation;
};

struct ModelKeyframe
{
	wstring szBoneName;
	vector<ModelKeyframeData> KeyData;
};



typedef struct KeyframeDesc
{
	uint32 iCurrFrame = 0;
	uint32 iNextFrame = 0;
	float fRatio = 0.f;
	float fSumTime = 0.f;
}KEY_DESC;

