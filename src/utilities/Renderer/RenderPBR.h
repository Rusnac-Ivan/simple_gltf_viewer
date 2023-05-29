#ifndef _UTIL_RENDER_PBR_H_
#define _UTIL_RENDER_PBR_H_

#include "IRender.h"
#include <resources/Program.h>

#include <utilities/Resources/ResourcesManager.h>
#include <opengl/Pipeline.h>

namespace util
{
	class RenderPBR : public IRender
	{
		rsrc::Program* mProgram;
	public:
		RenderPBR();
		~RenderPBR();

		virtual void Prepare(rsrc::Material* material, const glm::mat4& model);

	private:

	};
}

#endif