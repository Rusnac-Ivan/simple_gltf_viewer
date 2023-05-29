#ifndef _UTIL_FILE_DIALOG_H_
#define _UTIL_FILE_DIALOG_H_

#include <utilities/Events/CallBack.h>

namespace util
{
	using OnLoaded = CallBack<const char *, unsigned char *, int>;

	class IFileDialog
	{
	protected:
		bool mIsInStandby;
		enum
		{
			IDLE_FIND_FILE,
			SUCCESS_FIND_FILE,
			CANCEL_FIND_FILE,
			ERROR_FIND_FILE
		} mState;

	public:
		IFileDialog() : mState(IDLE_FIND_FILE), mIsInStandby(false) {}
		virtual ~IFileDialog() {}

		bool IsInStandby() { return mIsInStandby; }

		virtual void Load(const char *extension, const OnLoaded &on_loaded) = 0;
		virtual void Save(const char *extension, const OnLoaded &on_loaded) = 0;

	private:
	};
}

#endif