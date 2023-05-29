#ifndef _UTIL_EMSCRIPTEN_FILE_DIALOG_H_
#define _UTIL_EMSCRIPTEN_FILE_DIALOG_H_

#ifdef __EMSCRIPTEN__

#include <core/Platform.h>
#include <utilities/FileDialog/IFileDialog.h>

namespace util
{
    class EmscriptenFileDialog : public IFileDialog
    {
    private:
        emscripten::val mInput;
        emscripten::val mFileReader;
        OnLoaded mOnLoaded;
        std::string mFileName;

    public:
        EmscriptenFileDialog(/* args */);
        ~EmscriptenFileDialog();

        static void OnChange(emscripten::val thiz, emscripten::val event);
        static void OnLoad(emscripten::val thiz, emscripten::val event);
        static void OnAbort(emscripten::val thiz, emscripten::val event);
        static void OnError(emscripten::val thiz, emscripten::val event);

        virtual void Load(const char *extension, const OnLoaded &on_loaded);
        virtual void Save(const char *extension, const OnLoaded &on_loaded);
    };

}

#endif //__EMSCRIPTEN__

#endif