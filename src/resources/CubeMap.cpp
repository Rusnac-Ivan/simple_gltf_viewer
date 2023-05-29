#include "CubeMap.h"
#include <stb_image.h>
#include <opengl/Pipeline.h>
#include <opengl/Render.h>
#include <opengl/VertexAttribute.h>
#include <fstream>
#include <utilities/Resources/ResourcesManager.h>

namespace rsrc
{
    static float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    CubeMap::CubeMap(/* args */) : mIsReady(false), mIsLoaded(false)
    {
    }

    CubeMap::~CubeMap()
    {
    }

    void CubeMap::Loaded(int i)
    {
        mIsLoadedText[i] = true;
        mIsLoaded = true;
        for (uint32_t i = 0; i < TARGET_COUNT; i++)
        {
            mIsLoaded = mIsLoadedText[i] && mIsLoaded;
        }

        if (mIsLoaded)
        {
            mVBO.SetData(sizeof(skyboxVertices), skyboxVertices);
            mVAO.AddVertexLayout(mVBO, {gl::VertexAttribute::Entry<glm::vec3>()});

            mGLCubeMap.SetWrapModeS(gl::Texture::WrapMode::CLAMP_TO_EDGE);
            mGLCubeMap.SetWrapModeT(gl::Texture::WrapMode::CLAMP_TO_EDGE);

            mGLCubeMap.GenerateMipmaps();

            mIsReady = true;
        }
    }

    void CubeMap::Draw()
    {
        rsrc::Program* cubemap_prog = util::ResourcesManager::GetProgramsManager()->GetCubeMapProg();
        cubemap_prog->Use();
        if (mIsReady)
        {
            gl::Pipeline::SetDepthFunc(gl::CompareFunc::LEQUAL);    
            mGLCubeMap.Activate(0);
            mVAO.Bind();
            gl::Render::DrawVertices(gl::Primitive::TRIANGLES, 36, 0);
            gl::Pipeline::SetDepthFunc(gl::CompareFunc::LESS);
            gl::CubeMap::UnBind();
        }
    }

    void CubeMap::OnLoaded(uint32_t key, void *data, unsigned size)
    {

        int width, height, nrChannels;
        unsigned char *img_data = stbi_load_from_memory((unsigned char *)data, size, &width, &height, &nrChannels, 0);
        printf("load image w: %d, h: %d, c: %d\n", width, height, nrChannels);
        if (img_data)
        {
            switch (key)
            {
            case POS_X:
                mGLCubeMap.SetPositiveX(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                break;
            case NEG_X:
                mGLCubeMap.SetNegativeX(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                break;
            case POS_Y:
                mGLCubeMap.SetPositiveY(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                break;
            case NEG_Y:
                mGLCubeMap.SetNegativeY(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                break;
            case POS_Z:
                mGLCubeMap.SetPositiveZ(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                break;
            case NEG_Z:
                mGLCubeMap.SetNegativeZ(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                break;
            default:
                break;
            }
            Loaded(key);
            stbi_image_free(img_data);
        }
        assert(img_data && "stb not load texture");
    }
    void CubeMap::OnError(uint32_t key, int error_code, const char *status)
    {
        fprintf(stderr, "Failed to load cube_map texture error_code: %d, status: %s\n", error_code, status);
    }
    void CubeMap::OnProgress(uint32_t key, int bytes_loaded, int total_size)
    {
    }

    void CubeMap::Load(util::IResourceManager *manager, Type type, const char *path)
    {
        std::string imageNames[TARGET_COUNT] = {
            path + std::string("posx.png"),
            path + std::string("negx.png"),

            path + std::string("posy.png"),
            path + std::string("negy.png"),

            path + std::string("posz.png"),
            path + std::string("negz.png")};

        std::vector<Resource::ResData> cubeMapImages = {
            {POS_X, imageNames[POS_X]},
            {NEG_X, imageNames[NEG_X]},

            {POS_Y, imageNames[POS_Y]},
            {NEG_Y, imageNames[NEG_Y]},

            {POS_Z, imageNames[POS_Z]},
            {NEG_Z, imageNames[NEG_Z]}};

        Resource::Load(manager, type, cubeMapImages);
        /*#ifdef __EMSCRIPTEN__
                emscripten_async_wget2_data(
                    yokohama[0].c_str(), "GET", NULL, this, true,
                    [](unsigned handle, void *arg, void *data, unsigned size)
                    {
                        int width, height, nrChannels;
                        unsigned char *img_data = stbi_load_from_memory((unsigned char *)data, size, &width, &height, &nrChannels, 0);
                        printf("load image w: %d, h: %d, c: %d\n", width, height, nrChannels);
                        if (img_data)
                        {
                            CubeMap *thiz = (CubeMap *)arg;
                            thiz->mGLCubeMap.SetPositiveX(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                            stbi_image_free(img_data);
                            thiz->Loaded(0);
                        }
                        assert(img_data && "stb not load texture");
                    },
                    [](unsigned handle, void *arg, int error_code, const char *status)
                    {
                        fprintf(stderr, "Failed to load cube_map texture error_code: %d, status: %s\n", error_code, status);
                    },
                    [](unsigned handle, void *arg, int bytes_loaded, int total_size)
                    {
                        CubeMap *cubeMap = (CubeMap *)arg;
                        if (total_size)
                            cubeMap->mProgress[0] = ((float)bytes_loaded / total_size) * 100.f;
                    });

                emscripten_async_wget2_data(
                    yokohama[1].c_str(), "GET", NULL, this, true,
                    [](unsigned handle, void *arg, void *data, unsigned size)
                    {
                        int width, height, nrChannels;
                        unsigned char *img_data = stbi_load_from_memory((unsigned char *)data, size, &width, &height, &nrChannels, 0);
                        printf("load image w: %d, h: %d, c: %d\n", width, height, nrChannels);
                        if (img_data)
                        {
                            CubeMap *thiz = (CubeMap *)arg;
                            thiz->mGLCubeMap.SetNegativeX(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                            stbi_image_free(img_data);
                            thiz->Loaded(1);
                        }
                        assert(img_data && "stb not load texture");
                    },
                    [](unsigned handle, void *arg, int error_code, const char *status)
                    {
                        fprintf(stderr, "Failed to load cube_map texture error_code: %d, status: %s\n", error_code, status);
                    },
                    [](unsigned handle, void *arg, int bytes_loaded, int total_size)
                    {
                        CubeMap *cubeMap = (CubeMap *)arg;
                        if (total_size)
                            cubeMap->mProgress[1] = ((float)bytes_loaded / total_size) * 100.f;
                    });
                emscripten_async_wget2_data(
                    yokohama[2].c_str(), "GET", NULL, this, true,
                    [](unsigned handle, void *arg, void *data, unsigned size)
                    {
                        int width, height, nrChannels;
                        unsigned char *img_data = stbi_load_from_memory((unsigned char *)data, size, &width, &height, &nrChannels, 0);
                        printf("load image w: %d, h: %d, c: %d\n", width, height, nrChannels);
                        if (img_data)
                        {
                            CubeMap *thiz = (CubeMap *)arg;
                            thiz->mGLCubeMap.SetPositiveY(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                            stbi_image_free(img_data);
                            thiz->Loaded(2);
                        }
                        assert(img_data && "stb not load texture");
                    },
                    [](unsigned handle, void *arg, int error_code, const char *status)
                    {
                        fprintf(stderr, "Failed to load cube_map texture error_code: %d, status: %s\n", error_code, status);
                    },
                    [](unsigned handle, void *arg, int bytes_loaded, int total_size)
                    {
                        CubeMap *cubeMap = (CubeMap *)arg;
                        if (total_size)
                            cubeMap->mProgress[2] = ((float)bytes_loaded / total_size) * 100.f;
                    });
                emscripten_async_wget2_data(
                    yokohama[3].c_str(), "GET", NULL, this, true,
                    [](unsigned handle, void *arg, void *data, unsigned size)
                    {
                        int width, height, nrChannels;
                        unsigned char *img_data = stbi_load_from_memory((unsigned char *)data, size, &width, &height, &nrChannels, 0);
                        printf("load image w: %d, h: %d, c: %d\n", width, height, nrChannels);
                        if (img_data)
                        {
                            CubeMap *thiz = (CubeMap *)arg;
                            thiz->mGLCubeMap.SetNegativeY(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                            stbi_image_free(img_data);
                            thiz->Loaded(3);
                        }
                        assert(img_data && "stb not load texture");
                    },
                    [](unsigned handle, void *arg, int error_code, const char *status)
                    {
                        fprintf(stderr, "Failed to load cube_map texture error_code: %d, status: %s\n", error_code, status);
                    },
                    [](unsigned handle, void *arg, int bytes_loaded, int total_size)
                    {
                        CubeMap *cubeMap = (CubeMap *)arg;
                        if (total_size)
                            cubeMap->mProgress[3] = ((float)bytes_loaded / total_size) * 100.f;
                    });
                emscripten_async_wget2_data(
                    yokohama[4].c_str(), "GET", NULL, this, true,
                    [](unsigned handle, void *arg, void *data, unsigned size)
                    {
                        int width, height, nrChannels;
                        unsigned char *img_data = stbi_load_from_memory((unsigned char *)data, size, &width, &height, &nrChannels, 0);
                        printf("load image w: %d, h: %d, c: %d\n", width, height, nrChannels);
                        if (img_data)
                        {
                            CubeMap *thiz = (CubeMap *)arg;
                            thiz->mGLCubeMap.SetPositiveZ(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                            stbi_image_free(img_data);
                            thiz->Loaded(4);
                        }
                        assert(img_data && "stb not load texture");
                    },
                    [](unsigned handle, void *arg, int error_code, const char *status)
                    {
                        fprintf(stderr, "Failed to load cube_map texture error_code: %d, status: %s\n", error_code, status);
                    },
                    [](unsigned handle, void *arg, int bytes_loaded, int total_size)
                    {
                        CubeMap *cubeMap = (CubeMap *)arg;
                        if (total_size)
                            cubeMap->mProgress[4] = ((float)bytes_loaded / total_size) * 100.f;
                    });
                emscripten_async_wget2_data(
                    yokohama[5].c_str(), "GET", NULL, this, true,
                    [](unsigned handle, void *arg, void *data, unsigned size)
                    {
                        int width, height, nrChannels;
                        unsigned char *img_data = stbi_load_from_memory((unsigned char *)data, size, &width, &height, &nrChannels, 0);
                        printf("load image w: %d, h: %d, c: %d\n", width, height, nrChannels);
                        if (img_data)
                        {
                            CubeMap *thiz = (CubeMap *)arg;
                            thiz->mGLCubeMap.SetNegativeZ(0, gl::Texture::Format::RGB, width, height, 0, gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE, img_data);
                            stbi_image_free(img_data);
                            thiz->Loaded(5);
                        }
                        assert(img_data && "stb not load texture");
                    },
                    [](unsigned handle, void *arg, int error_code, const char *status)
                    {
                        fprintf(stderr, "Failed to load cube_map texture error_code: %d, status: %s\n", error_code, status);
                    },
                    [](unsigned handle, void *arg, int bytes_loaded, int total_size)
                    {
                        CubeMap *cubeMap = (CubeMap *)arg;
                        if (total_size)
                            cubeMap->mProgress[5] = ((float)bytes_loaded / total_size) * 100.f;
                    });
        #else
                for (uint32_t i = 0; i < TEXT_COUNT; i++)
                {
                    int width, height, nrChannels;
                    unsigned char *img_data = stbi_load(yokohama[i].c_str(), &width, &height, &nrChannels, 0);
                    printf("load image w: %d, h: %d, c: %d\n", width, height, nrChannels);
                    if (img_data)
                    {
                        mGLCubeMap.SetTarget(
                            static_cast<gl::CubeMap::Target>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0,
                            gl::Texture::Format::RGB, width, height, 0,
                            gl::Texture::Format::RGB, gl::DataType::UNSIGNED_BYTE,
                            img_data);
                        stbi_image_free(img_data);
                        Loaded(i);
                        mProgress[i] = 100.f;
                    }
                    else
                        assert(img_data && "stb not load texture");
                }

        #endif*/
    }
}