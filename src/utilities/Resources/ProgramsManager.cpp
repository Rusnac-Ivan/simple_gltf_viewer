#include "ProgramsManager.h"
#include "ResourcesManager.h"

namespace util
{
    void ProgramsManager::Load(const char* path)
    {
        for (uint32_t i = 0; i < PROGRAM_COUNT; i++)
        {
            std::string vert_sh_path = std::string(path) + mShaderFileNames[mProgramShadersMap[i][0]];
            std::string frag_sh_path = std::string(path) + mShaderFileNames[mProgramShadersMap[i][1]];
            mPrograms[i].Load(this, vert_sh_path.c_str(), frag_sh_path.c_str());
        }
    }

    void ProgramsManager::OnLoaded(rsrc::Resource* res)
    {
        mIsReady = true;
        for (uint32_t i = 0; i < PROGRAM_COUNT; i++)
        {
            mIsReady = mIsReady && mPrograms[i].IsReady();
        }
        if (mIsReady)
            util::ResourcesManager::OnProgramsManagerReady();
    }
}