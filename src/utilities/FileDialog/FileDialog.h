
#if defined(__EMSCRIPTEN__)
#include "EmscriptenFileDialog.h"
typedef util::EmscriptenFileDialog FileDialog;
#else
#include "NativeFileDialog.h"
typedef util::NativeFileDialog FileDialog;
#endif
