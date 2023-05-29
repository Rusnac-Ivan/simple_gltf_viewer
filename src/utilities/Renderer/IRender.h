#ifndef _UTIL_IRENDER_H_
#define _UTIL_IRENDER_H_

#include <glm/glm.hpp>
#include <resources/Material.h>

namespace util
{
	class IRender
	{
	public:
		IRender() {}
		virtual ~IRender() {}

		virtual void Prepare(rsrc::Material* material, const glm::mat4& model) = 0;

	private:

	};
}

#endif