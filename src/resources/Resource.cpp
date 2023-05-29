#include "Resource.h"

#include <core/Platform.h>
#include <cstdarg>
#ifndef __EMSCRIPTEN__
#include <fstream>
#include <vector>
#endif

namespace rsrc
{
    Resource::Resource() : mCurrentState(State::NONE),
                           mType(Type::PRE_LOADABLE),
                           mCurrentResManager(nullptr)
    {
    }
    Resource ::~Resource()
    {
    }

    void Resource::CheckState()
    {
        bool is_all_processed = true;
        bool exist_error = false;
        for (Info &info : mQueue)
        {
            is_all_processed = is_all_processed && (info.state == State::READY || info.state == State::ERROR);
            if (info.state == State::ERROR)
                exist_error = true;
        }

        if (is_all_processed)
        {
            if (exist_error)
                mCurrentState = State::ERROR;
            else
            {
                mCurrentState = State::READY;
                if (mCurrentResManager)
                    mCurrentResManager->OnLoaded(this);
            }
            mCurrentResManager = nullptr;
            mQueue.clear();
        }
    }

    void Resource::Load(util::IResourceManager *resource_manager, Type type, const std::vector<ResData> resources)
    {
        mCurrentResManager = resource_manager;
        mType = type;
        mCurrentState = State::LOADING;

        mQueue.resize(resources.size());
        for (uint32_t i = 0; i < mQueue.size(); i++)
        {
            mQueue[i].key = resources[i].key;
            mQueue[i].resPath = resources[i].resPath;
            mQueue[i].resource = this;
            mQueue[i].state = State::LOADING;
#ifdef __EMSCRIPTEN__
            emscripten_async_wget2_data(
                mQueue[i].resPath.c_str(), "GET", NULL, &mQueue[i], true,
                [](unsigned handle, void *arg, void *data, unsigned size)
                {
                    Info *res_info = (Info *)arg;
                    res_info->resource->OnLoaded(res_info->key, data, size);
                    res_info->state = State::READY;
                    res_info->resource->CheckState();
                },
                [](unsigned handle, void *arg, int error_code, const char *status)
                {
                    Info *res_info = (Info *)arg;
                    res_info->resource->OnError(res_info->key, error_code, status);
                    res_info->state = State::ERROR;
                    res_info->resource->CheckState();
                },
                [](unsigned handle, void *arg, int bytes_loaded, int total_size)
                {
                    Info *res_info = (Info *)arg;
                    res_info->resource->OnProgress(res_info->key, bytes_loaded, total_size);
                });
#else
            std::ifstream in_file;
            in_file.open(mQueue[i].resPath.c_str(), std::ios::in | std::ios::binary);

            if (in_file.is_open())
            {
                OnProgress(mQueue[i].key, 0, 0);
                mCurrentState = State::LOADING;

                // get length of file:
                in_file.seekg(0, in_file.end);
                int length = in_file.tellg();
                in_file.seekg(0, in_file.beg);

                // read data as a block:
                std::vector<char> buffer;
                buffer.resize(length, 0);
                in_file.read(buffer.data(), length);

                OnLoaded(mQueue[i].key, buffer.data(), buffer.size());

                mQueue[i].state = State::READY;

                CheckState();
            }
            else
            {
                OnError(mQueue[i].key, 1, "Cannot find resource !");
                mCurrentState = State::ERROR;
            }
#endif
        }
    }

}