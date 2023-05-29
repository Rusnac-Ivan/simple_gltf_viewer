#include "ModelsManager.h"
#include "ResourcesManager.h"

namespace util
{

    static constexpr char const* TempModelNames[ModelsManager::TEMP_MODEL_COUNT] =
    {
        "pbr.glb"
    };

    ModelsManager::ModelsManager(/* args */) : mIsReady(false)
    {
    }

    ModelsManager::~ModelsManager()
    {
    }

    void ModelsManager::Load(const char *path)
    {
        mTempModelsPath = std::string(path);
        std::string file_path = std::string(path);
        mTempModel = std::make_unique<rsrc::Model>();
        mTempModel->Load(this, rsrc::Resource::Type::POST_LOADABLE, std::string(file_path + TempModelNames[0]).c_str());
    }
    void ModelsManager::OnLoaded(rsrc::Resource *res)
    {
       
        mIsReady = true;

        mIsReady = mIsReady && mTempModel->GetCurrentState() == rsrc::Resource::State::READY;
        
        if (mIsReady)
            util::ResourcesManager::OnModelsManagerReady();
    }

    void ModelsManager::LoadModel(TheTempModel model)
    {
        mTempModel = std::make_unique<rsrc::Model>();
        mTempModel->Load(this, rsrc::Resource::Type::POST_LOADABLE, std::string(mTempModelsPath + TempModelNames[model]).c_str());
    }

    void ModelsManager::LoadModel(const char* file, unsigned char* data, size_t size)
    {
        mTempModel = std::make_unique<rsrc::Model>();
        mTempModel->Load(file, data, size);
    }

    void ModelsManager::UnLoadModel()
    {
        mTempModel.release();
    }
}