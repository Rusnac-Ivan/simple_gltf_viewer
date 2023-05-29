#ifndef _UTIL_RENDERING_H_
#define _UTIL_RENDERING_H_

#include <glm/glm.hpp>
#include <opengl/FrameBuffer.h>

namespace rsrc
{
    class Material;
    class Program;
}

namespace gl
{
    class Texture2D;
}

namespace core
{
    class Application;
}

namespace util
{
    class Rendering
    {
    public:
        enum Type
        {
            NONE,
            LIGHT_MAP,
            PBR,
            BACK_NORMALS,
            COUNT
        };

        static rsrc::Program *mPBRProg;
        static rsrc::Program *mLightMapProg;
        static rsrc::Program* mWParallaxProg;

        static void PrepareLightMap(rsrc::Material *material, const glm::mat4 &model);
        static void PreparePBR(rsrc::Material *material, const glm::mat4 &model);
        static void PrepareWParallax(rsrc::Material* material, const glm::mat4& model);
    };
}

#endif
