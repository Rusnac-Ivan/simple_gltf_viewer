#ifndef _UTIL_NATIVE_FILE_DIALOG_H_
#define _UTIL_NATIVE_FILE_DIALOG_H_

#include <utilities/FileDialog/IFileDialog.h>

#ifndef __EMSCRIPTEN__

namespace util
{

	class NativeFileDialog : public IFileDialog
	{
	public:
		NativeFileDialog();
		~NativeFileDialog();

		virtual void Load(const char* extension, const OnLoaded &on_loaded);
		virtual void Save(const char* extension, const OnLoaded &on_loaded);

	private:
	};

}

#endif //__EMSCRIPTEN__

#endif