#ifndef _UTIL_PROGRAMS_MANAGER_H_
#define _UTIL_PROGRAMS_MANAGER_H_

#include <core/Platform.h>
#include <resources/Program.h>
#include "IResourceManager.h"

namespace util
{
    class ProgramsManager : public IResourceManager
    {
        enum TheShader
        {
            PBR_VERT_SH,
            PBR_FRAG_SH,
            CUBE_MAP_VERT_SH,
            CUBE_MAP_FRAG_SH,
            LIGHT_MAP_VERT_SH,
            LIGHT_MAP_FRAG_SH,
            GET_BACK_NORMALS_VERT_SH,
            GET_BACK_NORMALS_FRAG_SH,
            WPARALLAX_VERT_SH,
            WPARALLAX_FRAG_SH,

            SHADER_COUNT
        };
        enum TheProgram
        {
            PBR_PROG,
            CUBE_MAP_PROG,
            LIGHT_MAP_PROG,
            GET_BACK_NORMALS_PROG,
            WPARALLAX_PROG,

            PROGRAM_COUNT
        };
    private:
        const int mProgramShadersMap[PROGRAM_COUNT][2] =
        {
            {PBR_VERT_SH, PBR_FRAG_SH},
            {CUBE_MAP_VERT_SH, CUBE_MAP_FRAG_SH},
            {LIGHT_MAP_VERT_SH, LIGHT_MAP_FRAG_SH},
            {GET_BACK_NORMALS_VERT_SH, GET_BACK_NORMALS_FRAG_SH},
            {WPARALLAX_VERT_SH, WPARALLAX_FRAG_SH}
        };

        const char* mShaderFileNames[SHADER_COUNT] = { "pbr.vs", "pbr.fs", "cube_map.vs", "cube_map.fs", "light_map.vs", "light_map.fs", "get_back_normals.vs", "get_back_normals.fs", "w_parallax.vs", "w_parallax.fs"};
        rsrc::Program mPrograms[PROGRAM_COUNT];
        bool mIsReady;
    public:

        virtual bool IsReady() { return mIsReady; }

        rsrc::Program* GetPBRProg() { return &mPrograms[PBR_PROG]; }
        rsrc::Program* GetBackNormalsProg() { return &mPrograms[GET_BACK_NORMALS_PROG]; }
        rsrc::Program* GetCubeMapProg() { return &mPrograms[CUBE_MAP_PROG]; }
        rsrc::Program* GetLightMapProg() { return &mPrograms[LIGHT_MAP_PROG]; }
        rsrc::Program* GetWParallaxProg() { return &mPrograms[WPARALLAX_PROG]; }

        virtual void Load( const char* path);
        virtual void OnLoaded(rsrc::Resource* res);
    };

    
}

#endif