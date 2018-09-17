/*
*Class Material
*	Material class manage setting for the object
*	Textures and color
*/

#ifndef _H_MATERIAL_
#define _H_MATERIAL_

#include "Objects/texture.h"
#include "types.h"
#include <vector>
namespace Awesome {
	class Material {
	public:
		//Move to TYPES?¿
		
		Material();
		~Material();

		void addTexture(std::shared_ptr<Texture> tex_);
		void setTexture(std::shared_ptr<Texture> tex_) {
			texture_[0] = tex_;
		}
		std::vector<std::shared_ptr<Texture>> textures();
		uniform_faster_map get_uniforms_information() {
			return uniforms_information_;
		}
		template <class Archive>
		void serialize(Archive & ar) {
			ar(Ambient, Diffuse, Specular);
		}
		glm::vec3 Ambient ;
		glm::vec3 Diffuse ;
		glm::vec3 Specular ;
		float Shininess = 32;
	private:
		void passUniformsToFasterMethod();
		std::vector<std::shared_ptr<Texture>> texture_;
		uniform_map uniforms_;
		uniform_faster_map uniforms_information_;
	};
}

#endif // !_H_MATERIAL_
