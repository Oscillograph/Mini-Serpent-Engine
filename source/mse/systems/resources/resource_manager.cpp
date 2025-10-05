#include <mse/systems/resources/resource_manager.h>
#include <mse/systems/platform/renderer/texture.h>
#include <mse/systems/platform/renderer/font.h>
#include <mse/systems/platform/renderer/cursor.h>
//#include <mse/systems/scenes/scene.h>
#include <mse/systems/windows/layers/layer.h>
#include <mse/systems/windows/window.h>
#include <mse/systems/platform/renderer/renderer.h>

namespace mse
{
	int Resource::dynamicResourceId = 0;
	
	std::unordered_map<ResourceType, ResourceCache> ResourceManager::m_Cache;
	std::unordered_map<ResourceType, std::string> ResourceManager::m_ResourceTypeNames;
	
	Resource::Resource(
		const std::string& _path,
		ResourceType _type,
		void* _data,
		bool dynamic
		)
	: type(_type), data(_data)
	{
		if (dynamic)
		{
//			path = dynamicResourceId;
            std::stringstream stream;
            stream << dynamicResourceId;
            path = stream.str();
            stream.str("");
            stream.clear();
			dynamicResourceId++;
		} else {
			path = _path;
		}
	}
    
    Resource::~Resource()
    {
        switch (type)
        {
        case ResourceType::Texture:
            {
                delete (Texture*)data;
                break;
            }
        case ResourceType::Text_Plain:
            {
                delete (Data::Text*)data;
                break;
            }
        case ResourceType::FontBitmap:
            {
                delete (FontBitmap*)data;
                break;
            }
        case ResourceType::FontTrueType:
            {
                delete (FontTrueType*)data;
                break;
            }
        case ResourceType::Cursor:
            {
                delete (Cursor*)data;
                break;
            }
        }
    }
	
	int ResourceManager::LoadResource(ResourceType type, const std::string& path, const ResourceUser& user)
	{
		if (m_Cache.find(type) == m_Cache.end())
		{
			return -1; // no such resource type cache registered
		} else {
			auto it = m_Cache[type].find(path);
			if (it != m_Cache[type].end())
				return -2; // already exists
			
			Resource* resource = new Resource(
				path,
				type,
				nullptr
				);
			
			m_Cache[type][path] = resource;
			
			switch (type) 
			{
			case ResourceType::Text_Plain:
				{
					// open file
					std::ifstream m_FileInput(path);
					if (!m_FileInput.is_open())
						m_FileInput.open(path);
					MSE_CORE_LOG("- file \"", path.c_str(), "\" opened for reading.");
					
					// construct a text object and read file contents into it
					// it should be connected somehow to text system and font system
					std::string contents = "";
					char* buffer = new char[1024];
					if (m_FileInput.good())
					{
						if (m_FileInput.rdstate() && std::ifstream::eofbit)
							MSE_CORE_LOG("- end of file reached somehow.");
						while (!m_FileInput.eof())
						{
							m_FileInput.getline(buffer, 1024, '\n');
							contents.append(buffer);
							contents.append("\n");
						}
						m_FileInput.seekg(0);
						m_FileInput.clear();
						MSE_CORE_LOG("- job Done!");
					} else {
						MSE_CORE_LOG("- we're not good!");
					}
					
					delete[] buffer;
					resource->data = new Data::Text(contents);
					
					// close file
					if (!m_FileInput.is_open())
						m_FileInput.close();
					MSE_CORE_LOG("- file \"", path.c_str(), "\" closed.");
				}
				break;
			case ResourceType::Texture:
				{
					resource->data = new Texture(path, user->GetRenderer());
				}
				break;
			case ResourceType::FontBitmap:
				{
					resource->data = new FontBitmap(path, user->GetRenderer());
                    break;
				}
			default:
				{}
				break;
			}
		}
		
		return 0;
	}
	
	Resource* ResourceManager::UseResource(ResourceType type, const std::string& path, const ResourceUser& user)
	{
		MSE_CORE_LOG("Requesting resource of type ", m_ResourceTypeNames[type], " with path \"", path.c_str(), "\".");
		auto it = m_Cache[type].find(path);
		if (it == m_Cache[type].end())
		{
			MSE_CORE_LOG("Resource Manager: There is no such resource with path \"", path.c_str(), "\".");
			MSE_CORE_LOG("Resource Manager: loading a resource...");
			LoadResource(type, path, user);
			m_Cache[type][path]->users.push_back(user);
			MSE_CORE_LOG("Resource Manager: resource is loaded, its user is registered.");
			return m_Cache[type][path];
		}
		
		// register a new user if necessary
		bool newUser = true;
		for (auto usersIterator = (*it).second->users.begin(); usersIterator != (*it).second->users.end(); usersIterator++)
		{
			if ((*usersIterator) == user)
				newUser = false; // false alarm: the user was registered before
		}
		if (newUser)
			(*it).second->users.push_back(user);
		
		return (*it).second;
	}
	
	int ResourceManager::DropResource(Resource* resource, const ResourceUser& user)
	{
		if (resource != nullptr)
		{
			return DropResource(resource->type, resource->path, user);
		} else {
			MSE_CORE_LOG("Asking to drop a resource which is nullptr!");
			return -1; // resource is nullptr
		}
	}
	
	int ResourceManager::DropResource(ResourceType type, const std::string& path, const ResourceUser& user)
	{
		MSE_CORE_LOG("Resource dropping procedure...");
		if (m_Cache.find(type) == m_Cache.end())
		{
			MSE_CORE_LOG("resource type not found");
			return -1; // no such resource type cache registered
		} else {
			MSE_CORE_LOG("resource type found");
			auto it = m_Cache[type].find(path);
			if (it == m_Cache[type].end())
			{
				MSE_CORE_LOG("resource not found");
				return -2; // couldn't find a resource
			}
			
			// unregister a user
			if (m_Cache[type][path] != nullptr)
			{
				MSE_CORE_LOG("m_Cache[\"", m_ResourceTypeNames[type], "\"][\"", path, "\"] != nullptr");
				for (auto it = m_Cache[type][path]->users.begin(); it != m_Cache[type][path]->users.end(); it++)
				{
					MSE_CORE_LOG(*it, " ", user);
					if ((*it) == user)
					{
						MSE_CORE_LOG("resource user found");
						(*it) = nullptr;
						m_Cache[type][path]->users.erase(it);
						MSE_CORE_LOG("user unregistered");
						break;
					}
				}
				
				// if there are no users left, mark the resource for deletion
				if (m_Cache[type][path]->users.size() == 0)
				{
					m_Cache[type][path]->deletionFlag = true;
					MSE_CORE_LOG("Flagged resource \"", path.c_str(), "\" for deletion.");
				}
			} else {
				MSE_CORE_LOG("m_Cache[type][path] == nullptr");
			}
		}
		
		return 0;
	}
	
	int ResourceManager::UnloadResource(ResourceType type, const std::string& path)
	{
		if (m_Cache.find(type) == m_Cache.end())
		{
			return -1; // no such resource type cache registered
		} else {
			auto it = m_Cache[type].find(path);
			if (it == m_Cache[type].end())
				return -1; // couldn't find a resource
			
			for (auto it = m_Cache[type][path]->users.begin(); it != m_Cache[type][path]->users.end(); it++)
			{
				(*it) = nullptr;
				m_Cache[type][path]->users.erase(it);
			}
			
			delete m_Cache[type][path];
		}
		
		return 0;
	}
	
	Resource* ResourceManager::CreateTexture(const ResourceUser& user, void* renderer, int w, int h, uint32_t flags, int depth, const glm::ivec4& colorMask)
	{
		ResourceType type = ResourceType::Texture;
		MSE_CORE_LOG("Resource Manager: creating resource of type ", m_ResourceTypeNames[type], "\".");
	
//		MSE_CORE_LOG("Resource Manager: creating a blank resource container...");
		Resource* resource = new Resource(
			"",
			type,
			nullptr, 
			true);
		
		if (m_Cache.find(type) == m_Cache.end())
		{
			MSE_CORE_ERROR("Resource Manager: no cache registered for textures");
			return nullptr; // no such resource type cache registered
		} else {
//			MSE_CORE_LOG("Resource Manager: creating a new texture...");
			resource->data = new Texture(renderer, w, h, flags, depth, colorMask.x, colorMask.y, colorMask.z, colorMask.w);
//			MSE_CORE_LOG("Resource Manager: registering a user...");
			resource->users.push_back(user);
//			MSE_CORE_LOG("Resource Manager: registering resource in cache...");
			m_Cache[type][resource->path] = resource;
//			MSE_CORE_LOG("Resource Manager: Done. Returning resource to the client");
			return resource;
		}
		
		return nullptr;
	}
	
	int ResourceManager::LoadTexture(const std::string& path, const ResourceUser& user, const glm::vec3& colorKey)
	{
		ResourceType type = ResourceType::Texture;
		
		if (m_Cache.find(type) == m_Cache.end())
		{
			return -1; // no such resource type cache registered
		} else {
			auto it = m_Cache[type].find(path);
			if (it != m_Cache[type].end())
				return -2; // already exists
			
			Resource* resource = new Resource(
				path,
				type,
				nullptr
				);
			
			// TODO: monitor layers among users
			resource->data = new Texture(path, user->GetRenderer(), {colorKey.x, colorKey.y, colorKey.z});
			
			m_Cache[type][path] = resource;
		}
		
		return 0;
	}
	
	Resource* ResourceManager::UseTexture(const std::string& path, const ResourceUser& user, const glm::vec3& colorKey)
	{
		ResourceType type = ResourceType::Texture;
		MSE_CORE_LOG("Requesting a texture with path \"", path.c_str(), "\".");
		auto it = m_Cache[type].find(path);
		if (it == m_Cache[type].end())
		{
			MSE_CORE_LOG("Resource Manager: There is no such resource with path \"", path.c_str(), "\".");
			MSE_CORE_LOG("Resource Manager: loading a resource...");
			LoadTexture(path, user, colorKey);
			m_Cache[type][path]->users.push_back(user);
			MSE_CORE_LOG("Resource Manager: resource is loaded, its user is registered.");
			return m_Cache[type][path];
		}
		
		// register a new user if necessary
		bool newUser = true;
		for (auto usersIterator = (*it).second->users.begin(); usersIterator != (*it).second->users.end(); usersIterator++)
		{
			if ((*usersIterator) == user)
				newUser = false; // false alarm: the user was registered before
		}
		if (newUser)
			(*it).second->users.push_back(user);
		
		return (*it).second;
	}
	
    Resource* ResourceManager::CreateCursor(const ResourceUser& user, int hotX, int hotY, Resource* sourceTexture, const glm::uvec4& clip, const glm::vec3& colorKey)
    {
        ResourceType type = ResourceType::Cursor;
        
        MSE_CORE_LOG("Resource Manager: creating resource of type ", m_ResourceTypeNames[type], "\".");
        Resource* resource = new Resource(
                                          "",
                                          type,
                                          nullptr, 
                                          true);
        
        if (m_Cache.find(type) == m_Cache.end())
        {
            MSE_CORE_ERROR("Resource Manager: no cache registered for cursors");
            return nullptr; // no such resource type cache registered
        } else {
//			MSE_CORE_LOG("Resource Manager: creating a new cursor...");
            mse::Resource* cursorTexture = mse::ResourceManager::CreateTexture(
                                                                        user,
                                                                        user->GetRenderer(),
                                                                        32, 32,
                                                                        0,
                                                                        32,
                                                                        {0, 0, 0, 0});
            SDL_FRect cursorDestRect = {0, 0, 32, 32};
            SDL_Rect cursorSrcRect = {clip.x, clip.y, clip.z, clip.w};
            mse::Renderer::SurfaceDrawTexture((mse::Texture*)(cursorTexture->data), (mse::Texture*)(sourceTexture->data), &cursorDestRect, &cursorSrcRect);
            resource->data = new Cursor(cursorTexture, user, hotX, hotY);
//			MSE_CORE_LOG("Resource Manager: registering a user...");
            resource->users.push_back(user);
//			MSE_CORE_LOG("Resource Manager: registering resource in cache...");
            m_Cache[type][resource->path] = resource;
//			MSE_CORE_LOG("Resource Manager: Done. Returning resource to the client");
            return resource;
        }
        
        return nullptr;
    }
    
	void ResourceManager::Init()
	{
		MSE_CORE_LOG("Resource Manager: Initialization.");
		m_ResourceTypeNames[ResourceType::Unknown] = "Unknown";
		m_ResourceTypeNames[ResourceType::Text_Plain] = "Plain text";
		m_ResourceTypeNames[ResourceType::Text_YAML] = "YAML Text";
		m_ResourceTypeNames[ResourceType::Text_JSON] = "JSON Text";
		m_ResourceTypeNames[ResourceType::Texture] = "Texture";
		m_ResourceTypeNames[ResourceType::Audio] = "Audio";
		m_ResourceTypeNames[ResourceType::FontBitmap] = "Bitmap Font";
		m_ResourceTypeNames[ResourceType::FontTrueType] = "TrueType Font";
        m_ResourceTypeNames[ResourceType::Cursor] = "Cursor";
		
		if (m_Cache.empty())
		{
			InitCache(ResourceType::Text_Plain);
			InitCache(ResourceType::Texture);
			InitCache(ResourceType::Audio);
			InitCache(ResourceType::FontBitmap);
            InitCache(ResourceType::Cursor);
		}
	}
	
	void ResourceManager::InitCache(ResourceType type)
	{
		m_Cache[type].reserve(256);
		MSE_CORE_LOG("Resource Manager: Cache \"", m_ResourceTypeNames[type], "\" initialized.");
	}
	
	void ResourceManager::GarbageCollector()
	{
		// select all types of resources
//		MSE_CORE_LOG("Resource Manager: Garbage collector starts...");
		ResourceType type;
		for (auto typeIterator = m_Cache.begin(); typeIterator != m_Cache.end(); typeIterator++)
		{
			type = (*typeIterator).first;
//			 MSE_CORE_LOG("- choosing resource category \"", m_ResourceTypeNames[type], "\"...");
			if (m_Cache.find(type) != m_Cache.end())
			{
				// MSE_CORE_LOG("- walking through resource category ", (int)type, "...");
				// for every resource of the selected type do
				for (auto cacheIterator = m_Cache[type].begin(); cacheIterator != m_Cache[type].end(); )
				{
					// MSE_CORE_LOG("- - selected a resource \"", (*cacheIterator).second->path.c_str(), "\"...");
					if ((*cacheIterator).second == nullptr)
					{
						// MSE_CORE_LOG("- - resource was deleted before...");
						cacheIterator = m_Cache[type].erase(cacheIterator);
					} else {
						// when do we delete a resource from the cache?
						if (((*cacheIterator).second->deletionFlag) || // when it was marked to
							((*cacheIterator).second->data == nullptr) || // when there is no data
							((*cacheIterator).second->users.size() == 0) // when there are no users
							)
						{
							// MSE_CORE_LOG("- - resource is flagged for deletion...");
							// nullify all users
							for (auto usersIterator = (*cacheIterator).second->users.begin(); usersIterator != (*cacheIterator).second->users.end(); usersIterator++)
							{
								(*usersIterator) = nullptr;
							}
							// MSE_CORE_LOG("- - users list cleared...");
							
							// delete the resource itself
							delete (*cacheIterator).second;
							(*cacheIterator).second = nullptr;
							cacheIterator = m_Cache[type].erase(cacheIterator);
							 MSE_CORE_LOG("- - resource deleted...");
						} else {
							cacheIterator++;
						}
					}
				}
			}
		}
//		MSE_CORE_LOG("Resource Manager: Garbage collector stops.");
	}
	
	void ResourceManager::ShutDown()
	{
		MSE_CORE_LOG("Resource Manager: Shutdown...");
		MSE_CORE_LOG("- flagging all resources for deletion...");
		for (uint32_t i = 0; i < 100; i++)
		{
			if (m_Cache.find((ResourceType)i) != m_Cache.end())
			{
				// for every resource of the selected type do
				for (auto cacheIterator = m_Cache[(ResourceType)i].begin(); cacheIterator != m_Cache[(ResourceType)i].end(); cacheIterator++)
				{
					if ((*cacheIterator).second)
					{
						(*cacheIterator).second->deletionFlag = true;
					}
				}
			}
		}
		MSE_CORE_LOG("...done.");
		MSE_CORE_LOG("Resource Manager: call Garbage collector to do the rest.");
		GarbageCollector();
		MSE_CORE_LOG("Resource Manager: Shutdown complete.");
	}
}
