#ifndef _RSRC_CUBEMAP_H_
#define _RSRC_CUBEMAP_H_

#include <core/Platform.h>
#include <opengl/CubeMap.h>
#include <opengl/VertexArray.h>
#include <opengl/VertexBuffer.h>
#include "Resource.h"
#include <utilities/Resources/IResourceManager.h>

namespace rsrc
{
    class CubeMap : public Resource
    {
    public:
        enum Target
        {
            POS_X,
            NEG_X,
            POS_Y,
            NEG_Y,
            POS_Z,
            NEG_Z,
            TARGET_COUNT
        };

    private:
        gl::CubeMap mGLCubeMap;

        bool mIsLoadedText[TARGET_COUNT] = {};
        bool mIsReady;
        bool mIsLoaded;

        gl::VertexBuffer mVBO;
        gl::VertexArray mVAO;

    public:
        CubeMap();
        ~CubeMap();

        gl::CubeMap *GetGLCubeMap() { return &mGLCubeMap; }

        void Load(util::IResourceManager *manager, Type type, const char *path);
        void Draw();

    private:
        void Loaded(int i);

        virtual void OnLoaded(uint32_t key, void *data, unsigned size);
        virtual void OnError(uint32_t key, int error_code, const char *status);
        virtual void OnProgress(uint32_t key, int bytes_loaded, int total_size);
    };

}

#endif