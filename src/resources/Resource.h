#ifndef _RSRC_RESOURCES_H_
#define _RSRC_RESOURCES_H_

#include <string>
#include <utilities/Resources/IResourceManager.h>
#include <list>
#include <vector>

namespace rsrc
{
	class Resource
	{

	public:
		struct ResData
		{
			uint32_t key;
			std::string resPath;
		};
		enum struct Type
		{
			PRE_LOADABLE, // resources that must be preloaded before running the application
			POST_LOADABLE // resources that can be loaded dynamically in runtime of application
		};
		enum struct State
		{
			NONE,
			LOADING,
			READY,
			ERROR
		};

	private:
		struct Info
		{
			uint32_t key;
			Resource *resource;
			std::string resPath;
			State state;
		};

		std::vector<Info> mQueue;
		Type mType;
		State mCurrentState;
		util::IResourceManager *mCurrentResManager;

	public:
		Resource();
		virtual ~Resource();

		Type GetType() { return mType; }
		State GetCurrentState() { return mCurrentState; }

		void Load(util::IResourceManager *resource_manager, Type type, const std::vector<ResData> resources);

	protected:
		virtual void OnLoaded(uint32_t key, void *data, unsigned size) = 0;
		virtual void OnError(uint32_t key, int error_code, const char *status) = 0;
		virtual void OnProgress(uint32_t key, int bytes_loaded, int total_size) = 0;

	private:
		void CheckState();
	};

}

#endif
