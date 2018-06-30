//---------------------------------------------------------------------------
//
//	Resource manager that handles all resources
//
//---------------------------------------------------------------------------
#pragma once

#include <map>
#include <string>
#include "CTextureMGR.h"
#include "tinyxml2.h"

enum EResourceType
{
	RESOURCE_UNKNOWN,
	RESOURCE_TEXTURE,
	RESOURCE_TOTAL
};

struct Resource
{
	EResourceType type;
	int handle;

	Resource() : type(RESOURCE_UNKNOWN), handle(-1)
	{
	}

	bool operator <(const Resource& other)
	{
		bool typeCheck =  (type < other.type) ? true : false;
		bool handleCheck = (handle < other.handle) ? true : false;

		return typeCheck && handleCheck;
	}

	bool operator ==(const Resource& other)
	{
		bool typeCheck = (type == other.type) ? true : false;
		bool handleCheck = (handle == other.handle) ? true : false;

		return typeCheck && handleCheck;
	}
};

class CResourceMGR
{
public:
	CResourceMGR();
	~CResourceMGR();

	bool Initialise(SDL_Renderer* sdlRenderer);

	STexture* GetTextureResource(const std::string& id, bool bLoad = false);

	void ClearAllResourceType(EResourceType type);

	static CResourceMGR* GetInstance();
	void Release();

private:
	bool LoadTexture(const std::string& id);

private:
	std::map<std::string, Resource> m_textureMap;
	CTextureMGR m_textureMGR;

	std::string m_xmlDataDir;
	std::string m_textureDataDir;

	static CResourceMGR* __instance;
};

#define RESOURCEMGR CResourceMGR::GetInstance()
