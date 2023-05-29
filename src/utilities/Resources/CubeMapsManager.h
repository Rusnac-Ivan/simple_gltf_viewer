#ifndef _UTIL_CUBEMAPS_MANAGER_H_
#define _UTIL_CUBEMAPS_MANAGER_H_

#include "IResourceManager.h"
#include <resources/CubeMap.h>

namespace util
{
    class CubeMapsManager : public IResourceManager
    {
    public:
        enum TheCubeMap
        {
            BASE,
            CUBEMAP_COUNT
        };

    private:
        rsrc::CubeMap mCubeMaps[CUBEMAP_COUNT];
        bool mIsReady;

    public:
        CubeMapsManager(/* args */);
        ~CubeMapsManager();

        rsrc::CubeMap *GetCubeMap(TheCubeMap cube_map) { return &mCubeMaps[cube_map]; }

        virtual void Load(const char *path);
        virtual bool IsReady() { return mIsReady; }
        virtual void OnLoaded(rsrc::Resource *res);
    };
}

#endif