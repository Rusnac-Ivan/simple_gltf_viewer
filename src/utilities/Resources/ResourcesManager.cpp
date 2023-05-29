#include "ResourcesManager.h"   

namespace util
{
        bool ResourcesManager::mIsLoaded = false;

        ResourcesManager::ManagerStates ResourcesManager::mManagerStates;
        std::unique_ptr<util::ProgramsManager> ResourcesManager::mProgramsManager;
        std::unique_ptr<util::CubeMapsManager> ResourcesManager::mCubeMapsManager;
        // std::unique_ptr<rsrc::CubeMap> ResourcesManager::mCubeMap;
        // std::unique_ptr<rsrc::Model> ResourcesManager::mModel;
        // std::unique_ptr<rsrc::Model> ResourcesManager::mModel1;
        std::unique_ptr<util::ModelsManager> ResourcesManager::mModelsManager;
        std::unique_ptr<util::ImagesManager> ResourcesManager::mImagesManager;


        util::CubeMapsManager *ResourcesManager::GetCubeMapsManager() { return mCubeMapsManager.get(); }
        util::ProgramsManager *ResourcesManager::GetProgramsManager() { return mProgramsManager.get(); }
        util::ModelsManager* ResourcesManager::GetModelsManager() { return mModelsManager.get(); }
        util::ImagesManager* ResourcesManager::GetImagesManager() { return mImagesManager.get(); }



        ResourcesManager::ResourcesManager()
        {
        }
        ResourcesManager::~ResourcesManager()
        {
        }

        void ResourcesManager::OnCubeMapsManagerReady()
        {
            mManagerStates.cubeMapsMan = true;
            mIsLoaded = mManagerStates.cubeMapsMan && mManagerStates.imagesMan && mManagerStates.modelsMan && mManagerStates.programsMan;
        }
        void ResourcesManager::OnProgramsManagerReady()
        {
            mManagerStates.programsMan = true;
            mIsLoaded = mManagerStates.cubeMapsMan && mManagerStates.imagesMan && mManagerStates.modelsMan && mManagerStates.programsMan;
        }
        void ResourcesManager::OnModelsManagerReady()
        {
            mManagerStates.modelsMan = true;
            mIsLoaded = mManagerStates.cubeMapsMan && mManagerStates.imagesMan && mManagerStates.modelsMan && mManagerStates.programsMan;
        }
        void ResourcesManager::OnImagesManagerReady()
        {
            mManagerStates.imagesMan = true;
            mIsLoaded = mManagerStates.cubeMapsMan && mManagerStates.imagesMan && mManagerStates.modelsMan && mManagerStates.programsMan;
        }

        bool ResourcesManager::IsLoaded() { return mIsLoaded; }

        void ResourcesManager::OnInitialize()
        {
            mManagerStates.cubeMapsMan = false;
            mManagerStates.imagesMan = false;
            mManagerStates.modelsMan = false;
            mManagerStates.programsMan = false;
#ifdef __EMSCRIPTEN__
                std::string path = "./resources";
#else
                std::string path = "D:/myexamples/gl_test/resources";
#endif

                // loads shaders
                mProgramsManager = std::make_unique<util::ProgramsManager>();
                mProgramsManager->Load(std::string(path + "/shaders/").c_str());
                // load models
                mModelsManager = std::make_unique<util::ModelsManager>();
                mModelsManager->Load(std::string(path + "/models/").c_str());
                // load cube map
                mCubeMapsManager = std::make_unique<util::CubeMapsManager>();
                mCubeMapsManager->Load(std::string(path + "/cube_maps/").c_str());
                // load images
                mImagesManager = std::make_unique<util::ImagesManager>();
                mImagesManager->Load(std::string(path + "/images/").c_str());

                /*mImage = std::make_unique<rsrc::Image>();
                std::vector<rsrc::Resource::ResData> image = {{0, std::string(std::string(path) + "/models/Porsche/porche.png")}};
                mImage->Load(nullptr, rsrc::Resource::Type::PRE_LOADABLE, image);*/
                
        }
        void ResourcesManager::OnFinalize()
        {
                mProgramsManager.release();
                // mCubeMap.release();
                mCubeMapsManager.release();
                // mModel.release();
                // mModel1.release();
                mModelsManager.release();
                mImagesManager.release();

                // mImage.release();
        }
}