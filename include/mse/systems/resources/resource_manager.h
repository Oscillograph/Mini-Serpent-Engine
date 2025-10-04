#ifndef MSE_SYSTEMS_RESOURCES_RESOURCE_MANAGER_H
#define MSE_SYSTEMS_RESOURCES_RESOURCE_MANAGER_H

#include <mse/core.h>

namespace mse
{	
	// Resource management stuff
	enum class ResourceType
	{
		Unknown				= 0,
		
		// user defines
		UserDefined_1		= 1,
		UserDefined_2		= 2,
		UserDefined_3		= 3,
		UserDefined_4		= 4,
		UserDefined_5		= 5,
		UserDefined_6		= 6,
		UserDefined_7		= 7,
		UserDefined_8		= 8,
		UserDefined_9		= 9,
		
		// media
		Audio				= 10,
		Texture				= 11,
		FontBitmap			= 12,
		FontTrueType		= 13,
		
		// text data
		Text_Plain			= 20,
		Text_JSON			= 21,
		Text_YAML			= 22
	};
	
	typedef Window* ResourceUser;
	
	struct Resource
	{
		Resource(
			const std::string& _path,
			ResourceType _type,
			void* _data,
			bool dynamic = false // true - generated in runtime; false - stored on disk before
			);
        ~Resource();
		
		std::string path = "";
		ResourceType type = ResourceType::Unknown;
		void* data = nullptr;
		std::vector<ResourceUser> users = {};
		bool deletionFlag = false;
		
	private:
		friend ResourceManager;
		static int dynamicResourceId;
	};
	
	namespace Data
	{
		struct Text
		{
			Text(const std::string& data)
			: text(data) {}
			
			std::string text = "";
		};
	}
	
	typedef std::unordered_map<std::string, Resource*> ResourceCache;
	
	// Basic usage:
	// - to initialize the manager
	// 		ResourceManager::Init();
	// - to ask for a resource and use it
	// 		Resource* resource1 = RM::ResourceManager::UseResource(RM::ResourceType::Text_Plain, "./main.cpp", nullptr);
	// - to let go of a resource
	// 		ResourceManager::DropResource(RM::ResourceType::Text_Plain, "./main.cpp", scenePointer);
	//			or
	//		ResourceManager::DropResource(resource1, scenePointer);
	// - to call for a garbage collector somewhere in the game cycle
	//		ResourceManager::GarbageCollector();
	// - to shutdown the manager
	//		ResourceManager::ShutDown();
	
	class ResourceManager
	{
	public:
		// default routines with no customization at all
		static int LoadResource(ResourceType type, const std::string& path, const ResourceUser& user);
		static Resource* UseResource(ResourceType type, const std::string& path, const ResourceUser& user);
		static int DropResource(Resource* resource, const ResourceUser& user); // an alias for the method below
		static int DropResource(ResourceType type, const std::string& path, const ResourceUser& user);
		static int UnloadResource(ResourceType type, const std::string& path);
		
		// TODO: customizable routines to load textures with color keys, etc.
		static Resource* CreateTexture(const ResourceUser& user, void* renderer, int w, int h, uint32_t flags=0, int depth=32, const glm::ivec4& colorMask = {0, 0, 0, 0});
		static int LoadTexture(const std::string& path, const ResourceUser& user, const glm::vec3& colorKey);
		static Resource* UseTexture(const std::string& path, const ResourceUser& user, const glm::vec3& colorKey);
		
		static void Init();
		static void InitCache(ResourceType type);
		static void GarbageCollector();
		static void ShutDown();
		
	private:
		static std::unordered_map<ResourceType, ResourceCache> m_Cache;
		static std::unordered_map<ResourceType, std::string> m_ResourceTypeNames;
	};
}

#endif

