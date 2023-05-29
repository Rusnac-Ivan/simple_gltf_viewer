#include "EmscriptenFileDialog.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>

namespace util
{

    EMSCRIPTEN_BINDINGS(EmscriptenFileDialog)
    {
        emscripten::function("JSEMFileDialogOnChange", &EmscriptenFileDialog::OnChange);
        emscripten::function("JSEMFileReaderOnLoad", &EmscriptenFileDialog::OnLoad);
        emscripten::function("JSEMFileReaderOnAbort", &EmscriptenFileDialog::OnAbort);
        emscripten::function("JSEMFileReaderOnError", &EmscriptenFileDialog::OnError);
    };

    EmscriptenFileDialog::EmscriptenFileDialog(/* args */) : mInput(emscripten::val::undefined()), mFileReader(emscripten::val::undefined())
    {
        emscripten::val document = emscripten::val::global("document");
        mInput = document.call<emscripten::val>("createElement", emscripten::val("input"));

        mInput.set("type", std::string("file"));

        emscripten::val::global("console").call<void>("log", mInput);

        mFileReader = emscripten::val::global("FileReader").new_();

        emscripten::val::global("console").call<void>("log", mFileReader);

        emscripten::val params = emscripten::val::object();
        params.set("instance", (uintptr_t)this);

        emscripten::val EMFileDialogOnChange = emscripten::val::module_property("JSEMFileDialogOnChange").call<emscripten::val>("bind", emscripten::val::null(), params);
        mInput.call<void>("addEventListener", std::string("change"), EMFileDialogOnChange, true);

        emscripten::val EMFileReaderOnLoad = emscripten::val::module_property("JSEMFileReaderOnLoad").call<emscripten::val>("bind", emscripten::val::null(), params);
        mFileReader.call<void>("addEventListener", std::string("load"), EMFileReaderOnLoad, true);

        emscripten::val EMFileReaderOnAbort = emscripten::val::module_property("JSEMFileReaderOnAbort").call<emscripten::val>("bind", emscripten::val::null(), params);
        mFileReader.call<void>("addEventListener", std::string("abort"), EMFileReaderOnAbort, true);

        emscripten::val EMFileReaderOnError = emscripten::val::module_property("JSEMFileReaderOnError").call<emscripten::val>("bind", emscripten::val::null(), params);
        mFileReader.call<void>("addEventListener", std::string("error"), EMFileReaderOnError, true);
    }

    EmscriptenFileDialog::~EmscriptenFileDialog()
    {
    }

    void EmscriptenFileDialog::OnAbort(emscripten::val thiz, emscripten::val event)
    {
        EmscriptenFileDialog *em_file_dialog = (EmscriptenFileDialog *)(thiz["instance"].as<uintptr_t>());
        printf("EmscriptenFileDialog::OnAbort\n");
        em_file_dialog->mOnLoaded.UnSet();
        em_file_dialog->mIsInStandby = false;
    }
    void EmscriptenFileDialog::OnError(emscripten::val thiz, emscripten::val event)
    {
        EmscriptenFileDialog *em_file_dialog = (EmscriptenFileDialog *)(thiz["instance"].as<uintptr_t>());
        printf("EmscriptenFileDialog::OnError\n");
        em_file_dialog->mOnLoaded.UnSet();
        em_file_dialog->mIsInStandby = false;
    }

    void EmscriptenFileDialog::OnChange(emscripten::val thiz, emscripten::val event)
    {
        EmscriptenFileDialog *em_file_dialog = (EmscriptenFileDialog *)(thiz["instance"].as<uintptr_t>());

        emscripten::val fileList = event["target"]["files"];
        if (fileList["length"].as<int>() > 0)
        {
            em_file_dialog->mFileReader.call<emscripten::val>("readAsArrayBuffer", fileList[0]);

            printf("file name: %s\n", fileList[0]["name"].as<std::string>().c_str());
            em_file_dialog->mFileName = fileList[0]["name"].as<std::string>();
        }
        else
        {
            printf("is click CANCEL !\n");
            em_file_dialog->mOnLoaded.UnSet();
            em_file_dialog->mIsInStandby = false;
        }
    }
    void EmscriptenFileDialog::OnLoad(emscripten::val thiz, emscripten::val event)
    {
        EmscriptenFileDialog *em_file_dialog = (EmscriptenFileDialog *)(thiz["instance"].as<uintptr_t>());

        emscripten::val array_buffer = em_file_dialog->mFileReader["result"].as<emscripten::val>();

        emscripten::val uint8Array = emscripten::val::global("Uint8Array").new_(array_buffer);

        std::vector<unsigned char> buff = emscripten::vecFromJSArray<uint8_t>(uint8Array);

        printf("buff size: %d\n", buff.size());
        //  printf("mCallBack: %p\n", em_file_dialog->mOnLoaded);
        //  if (em_file_dialog->mOnLoaded.IsNotEmpty())
        size_t extpos = em_file_dialog->mFileName.rfind("glb", em_file_dialog->mFileName.length());
        std::string ext(em_file_dialog->mFileName, extpos, em_file_dialog->mFileName.length());
        if (ext == "glb")
        {
            // printf("begin mCallBack: %p\n", em_file_dialog->mCallBack);
            em_file_dialog->mOnLoaded(em_file_dialog->mFileName.c_str(), static_cast<unsigned char *>(buff.data()), static_cast<int>(buff.size()));
            // printf("end mCallBack: %p\n", em_file_dialog->mCallBack);
        }

        em_file_dialog->mOnLoaded.UnSet();
        em_file_dialog->mIsInStandby = false;
    }

    void EmscriptenFileDialog::Load(const char *extension, const OnLoaded &on_loaded)
    {
        // mInput.set("name", MODEL_FILTER.name);

        mInput.set("accept", std::string("." + std::string(extension)));

        mInput.call<void>("click");

        mOnLoaded = on_loaded;

        printf("EmscriptenFileDialog::Load\n");
        // printf("mCallBack: %p\n", mCallBack);
        mIsInStandby = true;
    }
    void EmscriptenFileDialog::Save(const char *extension, const OnLoaded &on_loaded)
    {
    }
}
#endif //__EMSCRIPTEN__