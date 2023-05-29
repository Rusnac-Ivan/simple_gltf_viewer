#ifndef _UTIL_IRESOURCES_MANAGER_H_
#define _UTIL_IRESOURCES_MANAGER_H_

namespace rsrc
{
	class Resource;
}



namespace util
{
	class IResourceManager
	{
		public:
			virtual void Load(const char* path) = 0;
			virtual void OnLoaded(rsrc::Resource* res) = 0;
			virtual bool IsReady() = 0; //all preloadable is ready for use
			
	};
}

#endif