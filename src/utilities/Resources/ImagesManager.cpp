#include "ImagesManager.h"
#include "ResourcesManager.h"

namespace util
{
	static constexpr char const *ModelImageNames[ImagesManager::IMAGE_COUNT] =
		{
			"test.jpg"
		};


	ImagesManager::ImagesManager() : mIsReady(false)
	{
	}

	ImagesManager::~ImagesManager()
	{
	}

	void ImagesManager::Load(const char *path)
	{
		for (uint32_t i = 0; i < IMAGE_COUNT; i++)
		{
			std::vector<rsrc::Resource::ResData> image = {{0, std::string(path + std::string(ModelImageNames[i])).c_str()}};
			mImages[i].Load(this, rsrc::Resource::Type::PRE_LOADABLE, image);
		}
	}

	void ImagesManager::OnLoaded(rsrc::Resource *res)
	{
		//if (res->GetType() == rsrc::Resource::Type::PRE_LOADABLE)
		{
			mIsReady = true;
			for (uint32_t i = 0; i < IMAGE_COUNT; i++)
			{
				mIsReady = mIsReady && mImages[i].GetCurrentState() == rsrc::Resource::State::READY;
			}
		}
		if (mIsReady)
			util::ResourcesManager::OnImagesManagerReady();
	}
}