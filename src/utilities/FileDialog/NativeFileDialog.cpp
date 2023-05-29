#include "NativeFileDialog.h"

#ifndef __EMSCRIPTEN__

#include <nfd.h>
#include <cassert>
#include <stdio.h>
#include <fstream>
#include <vector>

namespace util
{

    NativeFileDialog::NativeFileDialog()
    {
        nfdresult_t res = NFD_Init();
        assert(res == NFD_OKAY);
    }

    NativeFileDialog::~NativeFileDialog()
    {
        NFD_Quit();
    }

    void NativeFileDialog::Load(const char* extension, const OnLoaded &on_loaded)
    {
        nfdchar_t *outPath;

        nfdfilteritem_t filterItem;
        filterItem.name = "";
        filterItem.spec = extension;

        nfdresult_t result = NFD_OpenDialog(&outPath, &filterItem, 1, NULL);
        if (result == NFD_OKAY)
        {
            std::string file_name(outPath);
            size_t extpos = file_name.rfind("glb", file_name.length());
            std::string ext(file_name, extpos, file_name.length());
            if (ext == "glb")
            {
                // mState = SUCCESS_FIND_FILE;
                printf("outPath: %s\n", outPath);
                std::ifstream in(outPath, std::ios::binary);
                std::vector<unsigned char> buff((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
                on_loaded(outPath, buff.data(), buff.size());
            }
            
            NFD_FreePath(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            // mState = CANCEL_FIND_FILE;
            printf("User pressed cancel.\n");
        }
        else
        {
            // mState = ERROR_FIND_FILE;
            printf("Error: %s\n", NFD_GetError());
        }
    }
    void NativeFileDialog::Save(const char* extension, const OnLoaded &on_loaded)
    {
        nfdchar_t *outPath;

        nfdfilteritem_t filterItem;
        filterItem.name = "";
        filterItem.spec = extension;

        nfdresult_t result = NFD_SaveDialog(&outPath, &filterItem, 1, NULL, NULL);
        if (result == NFD_OKAY)
        {
            // mState = SUCCESS_FIND_FILE;
            printf("outPath: %s\n", outPath);

            NFD_FreePath(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            // mState = CANCEL_FIND_FILE;
            printf("User pressed cancel.\n");
        }
        else
        {
            // mState = ERROR_FIND_FILE;
            printf("Error: %s\n", NFD_GetError());
        }
    }
}

#endif //__EMSCRIPTEN__