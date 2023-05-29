#include "CubeMapsManager.h"
#include "ResourcesManager.h"

namespace util
{
    static constexpr char const *CubeMapPaths[CubeMapsManager::CUBEMAP_COUNT] = {"old_hall/"};

    CubeMapsManager::CubeMapsManager(/* args */) : mIsReady(false)
    {
    }

    CubeMapsManager::~CubeMapsManager()
    {
    }

    void CubeMapsManager::Load(const char *path)
    {
        for (uint32_t i = 0; i < CUBEMAP_COUNT; i++)
        {
            mCubeMaps[i].Load(this, rsrc::Resource::Type::PRE_LOADABLE, std::string(std::string(path) + CubeMapPaths[i]).c_str());
        }
    }
    void CubeMapsManager::OnLoaded(rsrc::Resource *res)
    {
        //if (res->GetType() == rsrc::Resource::Type::PRE_LOADABLE)
        {
            mIsReady = true;
            for (uint32_t i = 0; i < CUBEMAP_COUNT; i++)
            {
                mIsReady = mIsReady && mCubeMaps[i].GetCurrentState() == rsrc::Resource::State::READY;
            }
        }
        if (mIsReady)
            util::ResourcesManager::OnCubeMapsManagerReady();
    }
}