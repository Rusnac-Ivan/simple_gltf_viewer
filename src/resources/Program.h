#ifndef _UTIL_PROGRAM_H_
#define _UTIL_PROGRAM_H_

#include <core/Platform.h>
#include <opengl/Shader.h>
#include <opengl/Program.h>
#include <fstream>
#include <sstream>
#include <string>
#include "Resource.h"
#include <utilities/Resources/IResourceManager.h>

namespace rsrc
{
    class Program : public Resource
    {
        enum Shader
        {
            VERT_SH,
            FRAG_SH
        };

    public:
        enum Attrib
        {
            ATTRIB_POSITION,
            ATTRIB_NORMAL,
            ATTRIB_UV0,
            ATTRIB_UV1,
            ATTRIB_COUNT
        };

        enum Uniform
        {
            UNIFORM_TIME,
            UNIFORM_IS_VIDEO,

            UNIFORM_MODEL,
            UNIFORM_VIEW,
            UNIFORM_PROJ,
            UNIFORM_PROJ_VIEW,

            UNIFORM_DEBUG_VIEW_INPUTS,

            UNIFORM_BASE_COLOR_FACTOR,
            UNIFORM_BASE_COLOR_MAP,
            UNIFORM_BASE_COLOR_MAP_SET,

            UNIFORM_ROUGHNESS_FACTOR,
            UNIFORM_METALNESS_FACTOR,
            UNIFORM_METALLIC_ROUGHNESS_MAP,
            UNIFORM_METALLIC_ROUGHNESS_MAP_SET,

            UNIFORM_NORMAL_MAP,
            UNIFORM_NORMAL_MAP_SET,

            UNIFORM_EMISSIVE_FACTOR,
            UNIFORM_EMISSIVE_MAP,
            UNIFORM_EMISSIVE_MAP_SET,

            UNIFORM_EYE_POSE,
            UNIFORM_LIGHT_POSE,

            UNIFORM_EXPOSURE,
            UNIFORM_GAMMA,

            UNIFORM_CUBEMAP,

            UNIFORM_BACK_NORMALS_MAP,

            UNIFORM_RESOLUTION,

            UNIFORM_COUNT
        };

    private:
        gl::Program mGLProgram;

        int32_t mAttribs[ATTRIB_COUNT] = {};
        int32_t mUniforms[UNIFORM_COUNT] = {};

        gl::Shader<gl::IShader::Type::VERTEX> mVertSh;
        gl::Shader<gl::IShader::Type::FRAGMENT> mFragSh;

        bool mVertShLoaded;
        bool mFragShLoaded;

        bool mIsReady;

    public:
        Program(/* args */);
        ~Program();

        gl::Program *GetGLProgram() { return &mGLProgram; }

        void Use() { mGLProgram.Use(); }
        void StopUsing() { mGLProgram.StopUsing(); }

        bool IsReady() { return mIsReady; };

        template <typename T>
        void SetUniform(Uniform u, T val);

        void LoadVertSh(const char *shaderCodes, const int length);
        void LoadFragSh(const char *shaderCodes, const int length);
        void CreateProg();

        void Load(util::IResourceManager *manager, const char *vert_sh, const char *frag_sh);

    private:
        virtual void OnLoaded(uint32_t key, void *data, unsigned size);
        virtual void OnError(uint32_t key, int error_code, const char *status);
        virtual void OnProgress(uint32_t key, int bytes_loaded, int total_size);
    };

    template <>
    inline void Program::SetUniform<bool>(Uniform u, bool val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetBool(loc, val);
    }

    template <>
    inline void Program::SetUniform<int>(Uniform u, int val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetInt(loc, val);
    }

    template <>
    inline void Program::SetUniform<glm::ivec2>(Uniform u, glm::ivec2 val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetInt2(loc, val);
    }

    template <>
    inline void Program::SetUniform<glm::ivec3>(Uniform u, glm::ivec3 val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetInt3(loc, val);
    }

    template <>
    inline void Program::SetUniform<glm::ivec4>(Uniform u, glm::ivec4 val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetInt4(loc, val);
    }

    template <>
    inline void Program::SetUniform<float>(Uniform u, float val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetFloat(loc, val);
    }

    template <>
    inline void Program::SetUniform<glm::vec2>(Uniform u, glm::vec2 val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetFloat2(loc, val);
    }

    template <>
    inline void Program::SetUniform<glm::vec3>(Uniform u, glm::vec3 val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetFloat3(loc, val);
    }

    template <>
    inline void Program::SetUniform<glm::vec4>(Uniform u, glm::vec4 val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetFloat4(loc, val);
    }

    template <>
    inline void Program::SetUniform<glm::mat2>(Uniform u, glm::mat2 val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetMatrix2(loc, val);
    }

    template <>
    inline void Program::SetUniform<glm::mat3>(Uniform u, glm::mat3 val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetMatrix3(loc, val);
    }

    template <>
    inline void Program::SetUniform<glm::mat4>(Uniform u, glm::mat4 val)
    {
        assert(u >= 0 && u < UNIFORM_COUNT && "Uniform out of range !");
        int loc = mUniforms[u];
        assert(loc >= 0 && "Invalid uniform !");
        mGLProgram.SetMatrix4(loc, val);
    }

}

#endif