#ifndef _UTIL_IMAGES_MANAGER_H_
#define _UTIL_IMAGES_MANAGER_H_

#include <resources/Image.h>
#include <utilities/Resources/IResourceManager.h>

namespace util
{
	class ImagesManager : public IResourceManager
	{
	public:
		enum TheImage
		{
			BASE,
			IMAGE_COUNT
		};
	private:
		rsrc::Image mImages[IMAGE_COUNT];
		bool mIsReady;
	public:
		ImagesManager();
		~ImagesManager();

		rsrc::Image* GetImage(TheImage image) { return &mImages[image]; }

		
		virtual void Load(const char* path);
		virtual bool IsReady() { return mIsReady; }
		virtual void OnLoaded(rsrc::Resource* res);
		
	private:

	};

	



}

#endif