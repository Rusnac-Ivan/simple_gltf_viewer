#include "Program.h"
#include <cstring>

namespace rsrc
{

    constexpr char const *ATTRIB_NAMES[Program::ATTRIB_COUNT] = {
        "aPos",
        "aNorm",
        "aUV0",
        "aUV1",
    };

    constexpr char const *UNIFORM_NAMES[Program::UNIFORM_COUNT] = {
        "uTime",

        "uIsVideo",

        "uModel",
        "uView",
        "uProj",
        "uProjView",

        "uDebugViewInputs",

        "uBaseColorFactor",
        "uBaseColorMap",
        "uBaseColorMapSet",

        "uRoughnessFactor",
        "uMetalnessFactor",
        "uMetallicRoughnessMap",
        "uMetallicRoughnessMapSet",

        "uNormalMap",
        "uNormalMapSet",

        "uEmissiveFactor",
        "uEmissiveMap",
        "uEmissiveMapSet",

        "uEyePos",
        "uLightPos",

        "uExposure",
        "uGamma",

        "uCubeMap",

        "uBackNormalsMap",

        "uResolution"
    };

    Program::Program(/* args */) : mIsReady(false), mVertShLoaded(false), mFragShLoaded(false)
    {
        for (uint32_t i = 0; i < UNIFORM_COUNT; i++)
        {
            mUniforms[i] = -1;
        }
    }

    Program::~Program()
    {
    }

    void Program::LoadVertSh(const char *shaderCodes, const int length)
    {
        char *shaders_code[2] = {(char *)GLSL_VERSION, (char *)shaderCodes};
        int sizes[2] = {};
        sizes[0] = strlen(GLSL_VERSION);
        sizes[1] = length;

        if (mVertSh.LoadSources(2, (const char **)shaders_code, sizes))
            mVertShLoaded = true;
        CreateProg();
    }
    void Program::LoadFragSh(const char *shaderCodes, const int length)
    {
        char *shaders_code[2] = {(char *)GLSL_VERSION, (char *)shaderCodes};
        int sizes[2] = {};
        sizes[0] = strlen(GLSL_VERSION);
        sizes[1] = length;

        if (mFragSh.LoadSources(2, (const char **)shaders_code, sizes))
            mFragShLoaded = true;
        CreateProg();
    }
    void Program::CreateProg()
    {
        if (mVertShLoaded && mFragShLoaded)
        {
            mGLProgram.Attach(&mVertSh, &mFragSh, NULL);
            if (mGLProgram.Link())
            {
                for (uint32_t i = 0; i < UNIFORM_COUNT; i++)
                {
                    mUniforms[i] = mGLProgram.Uniform(UNIFORM_NAMES[i]);
                }
                mIsReady = true;
            }
        }
    }

    void Program::Load(util::IResourceManager *manager, const char *vert_sh, const char *frag_sh)
    {
        for (uint32_t i = 0; i < UNIFORM_COUNT; i++)
        {
            mUniforms[i] = -1;
        }
        std::vector<Resource::ResData> shaders = {
            {VERT_SH, std::string(vert_sh)},
            {FRAG_SH, std::string(frag_sh)}};
        Resource::Load(manager, rsrc::Resource::Type::PRE_LOADABLE, shaders);
    }

    void Program::OnLoaded(uint32_t key, void *data, unsigned size)
    {
        if (key == VERT_SH)
            LoadVertSh((const char *)data, size);
        else if (key)
            LoadFragSh((const char *)data, size);
    }
    void Program::OnError(uint32_t key, int error_code, const char *status)
    {
        fprintf(stderr, "Failed to load vert shader error_code: %d, status: %s\n", error_code, status);
    }
    void Program::OnProgress(uint32_t key, int bytes_loaded, int total_size)
    {
    }
}