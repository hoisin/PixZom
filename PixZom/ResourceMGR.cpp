#include "ResourceMGR.h"

CResourceMGR* CResourceMGR::__instance = nullptr;

CResourceMGR::CResourceMGR() 
{
	m_xmlDataDir = "..\\Assets\\Data\\PixZomAssetData.xml";
	m_textureDataDir = "..\\Assets\\Textures\\";
}


CResourceMGR::~CResourceMGR()
{
}

bool CResourceMGR::Initialise(SDL_Renderer * sdlRenderer)
{
	m_textureMGR.Initialise(sdlRenderer);
	return true;
}

//---------------------------------------------------------------------------
//
//	Returns texture of given id.
//  If bLoad is true, attempts to load the texture if not already loaded.
//
//---------------------------------------------------------------------------
STexture * CResourceMGR::GetTextureResource(const std::string & id, bool bLoad)
{
	// Check if texture resource is loaded
	auto findIt = m_textureMap.find(id);
	// If loaded, then simply return
	if (findIt != m_textureMap.end())
		return m_textureMGR.GetTexture(findIt->second.handle);

	// If bLoad flag enabled attempt to load the texture
	if (bLoad)
	{
		if (LoadTexture(id))
		{
			findIt = m_textureMap.find(id);
			if (findIt != m_textureMap.end())
				return m_textureMGR.GetTexture(findIt->second.handle);

			// Something went horribly wrong if texture not found at this point
		}
	}

	return nullptr;
}

void CResourceMGR::ClearAllResourceType(EResourceType type)
{
	switch (type)
	{
	case RESOURCE_TEXTURE:
		m_textureMGR.CleanUp();
		break;
	default:
		break;
	}
}

CResourceMGR * CResourceMGR::GetInstance()
{
	if (__instance)
		return __instance;

	__instance = new CResourceMGR;

	return __instance;
}

void CResourceMGR::Release()
{
	if (__instance)
	{
		delete __instance;
		__instance = nullptr;
	}
}

//---------------------------------------------------------------------------
//
//	Attempts to load texture of given id.
//  Id is checked against resource XML.
//  If Id not defined in XML, texture does not load.
//
//---------------------------------------------------------------------------
bool CResourceMGR::LoadTexture(const std::string& id)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError error = doc.LoadFile(m_xmlDataDir.c_str());

	if (error == tinyxml2::XML_SUCCESS)
	{
		tinyxml2::XMLElement* node = doc.RootElement();
		if (node)
		{
			tinyxml2::XMLElement* texturesNode = node->FirstChildElement("Textures");
			if (texturesNode)
			{
				tinyxml2::XMLElement* textureNode = texturesNode->FirstChildElement("Texture");
				while (textureNode)
				{
					std::string textureId = textureNode->Attribute("id");
					if (textureId == id)
					{
						Resource newTextureRes;
						newTextureRes.type = RESOURCE_TEXTURE;

						tinyxml2::XMLElement* dataNode = textureNode->FirstChildElement("TexturePath");
						std::string fileName = dataNode->GetText();
						newTextureRes.handle = m_textureMGR.LoadTexture(m_textureDataDir + fileName, textureId);
						STexture* pTexture = m_textureMGR.GetTexture(newTextureRes.handle);
						
						// Texture width and height is already found when loading texture.
						// Could add additional check for validatio against the XML with what we have loaded.
						dataNode = textureNode->FirstChildElement("FrameWidth");	
						pTexture->frameWidth = dataNode->IntText();

						dataNode = textureNode->FirstChildElement("FrameHeight");
						pTexture->frameHeight = dataNode->IntText();

						dataNode = textureNode->FirstChildElement("Animations");
						int animationCount = dataNode->IntAttribute("rows");
						tinyxml2::XMLElement* framesInRowNode = dataNode->FirstChildElement("FramesInRow");
						for (int i = 0; i < animationCount; i++) 
						{
							if (framesInRowNode)
							{
								std::string animId = framesInRowNode->Attribute("id");
								pTexture->m_rowFrames[animId] = framesInRowNode->IntText();
							}
							framesInRowNode = framesInRowNode->NextSiblingElement();
						}

						m_textureMap[textureId] = newTextureRes;
						// Found texture and loaded successfully
						return true;
					}
					// Look for next xml node till found match or no more xml
					textureNode = textureNode->NextSiblingElement();
				}
			}
		}
	}
	// Failed to load specified texture of id
	return false;
}

