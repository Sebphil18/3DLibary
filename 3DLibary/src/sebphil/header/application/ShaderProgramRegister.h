#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "globjects/ShaderProgram.h"

namespace glib {

	enum class ShaderProgramPreset {
		High, Medium, Low
	};

	class ShaderProgramRegister {

	public:
		inline ShaderProgramRegister(ShaderProgramPreset preset = ShaderProgramPreset::High) {
			loadStandardPrograms();
		};

		inline std::shared_ptr<ShaderProgram> operator[](const std::string& key) noexcept {
			return programs[key];
		};

		template<class... Args>
		inline void loadProgram(const std::string& key, Args&&... args) {
			programs[key] = std::make_shared<ShaderProgram>(std::forward<Args>(args)...);
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> programs;

		inline void loadStandardPrograms() {

			programs["main"] = std::make_shared<glib::ShaderProgram>(
				"src/sebphil/shader/vertex/VertexStandard.glsl",
				"src/sebphil/shader/fragment/FragmentStandard.glsl"
				);

			programs["screen"] = std::make_shared<glib::ShaderProgram>(
				"src/sebphil/shader/vertex/VertexScreen.glsl",
				"src/sebphil/shader/fragment/FragmentScreen.glsl"
				);

			programs["skyBox"] = std::make_shared<glib::ShaderProgram>(
				"src/sebphil/shader/vertex/SkyBox.glsl",
				"src/sebphil/shader/fragment/SkyBox.glsl"
				);

			programs["convolution"] = std::make_shared<glib::ShaderProgram>(
				"src/sebphil/shader/vertex/IrradianceConvolution.glsl",
				"src/sebphil/shader/fragment/IrradianceConvolution.glsl"
				);

			programs["brdfIntegration"] = std::make_shared<glib::ShaderProgram>(
				"src/sebphil/shader/vertex/BRDFConvolution.glsl",
				"src/sebphil/shader/fragment/BRDFConvolution.glsl"
				);

			programs["prefilter"] = std::make_shared<glib::ShaderProgram>(
				"src/sebphil/shader/vertex/PrefilterConvolution.glsl",
				"src/sebphil/shader/fragment/PrefilterConvolution.glsl"
				);

			programs["equirectConversion"] = std::make_shared<glib::ShaderProgram>(
				"src/sebphil/shader/vertex/EquirectangularToCube.glsl",
				"src/sebphil/shader/fragment/EquirectangularToCube.glsl"
				);

			programs["main"]->setUniformBlockBinding("Matrices", 0);
		};

	};
}