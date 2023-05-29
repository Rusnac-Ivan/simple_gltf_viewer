#ifndef _UTIL_MODELS_MANAGER_H_
#define _UTIL_MODELS_MANAGER_H_

#include "IResourceManager.h"
#include <resources/Model.h>
#include <memory>

namespace util
{
    class ModelsManager : public IResourceManager
    {
    public:
        enum TheTempModel
        {
            PBR,
            TEMP_MODEL_COUNT
        };


    private:
        std::unique_ptr<rsrc::Model> mTempModel;
        bool mIsReady;
        std::string mTempModelsPath;

    public:
        ModelsManager(/* args */);
        ~ModelsManager();

        rsrc::Model* GetTmpModel() { return mTempModel.get(); }
        void LoadModel(TheTempModel model);
        void LoadModel(const char* file, unsigned char* data, size_t size);
        void UnLoadModel();

        virtual void Load(const char *path);
        virtual bool IsReady() { return mIsReady; }
        virtual void OnLoaded(rsrc::Resource *res);
    };
}

#endif
