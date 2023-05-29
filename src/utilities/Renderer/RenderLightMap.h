#ifndef _UTIL_RENDER_LIGHT_MAP_H_
#define _UTIL_RENDER_LIGHT_MAP_H_


#include "IRender.h"
#include <resources/Program.h>
#include <utilities/Resources/ResourcesManager.h>
#include <opengl/Pipeline.h>

namespace util
{
	class RenderLightMap : public IRender
	{
		rsrc::Program* mProgram;
	public:
		RenderLightMap();
		~RenderLightMap();

		virtual void Prepare(rsrc::Material* material, const glm::mat4& model);

	private:

	};

	
}

#endif