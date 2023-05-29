#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include <core/Platform.h>
#include "ProgramsManager.h"
#include <resources/CubeMap.h>
#include <resources/Model.h>
#include <resources/Image.h>
#include "ModelsManager.h"
#include "CubeMapsManager.h"
#include "ImagesManager.h"

namespace util
{
    class ResourcesManager
    {
        struct ManagerStates
        {
            bool programsMan : 1;
            bool cubeMapsMan : 1;
            bool modelsMan : 1;
            bool imagesMan : 1;
        };

        static ManagerStates mManagerStates;

        static std::unique_ptr<util::ProgramsManager> mProgramsManager;
        static std::unique_ptr<util::CubeMapsManager> mCubeMapsManager;
        static std::unique_ptr<util::ModelsManager> mModelsManager;
        static std::unique_ptr<util::ImagesManager> mImagesManager;


        ResourcesManager();
        ~ResourcesManager();

        static bool mIsLoaded;

    public:
        static void OnInitialize();
        static void OnFinalize();

        static util::CubeMapsManager *GetCubeMapsManager();
        static util::ProgramsManager *GetProgramsManager();
        static util::ModelsManager* GetModelsManager();
        static util::ImagesManager* GetImagesManager();


        static bool IsLoaded();

        static void OnCubeMapsManagerReady();
        static void OnProgramsManagerReady();
        static void OnModelsManagerReady();
        static void OnImagesManagerReady();
    };
}

#endif