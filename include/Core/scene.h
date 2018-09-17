/*
*	Class Scene
*	Scene class ,contain the gameobjects that will be renderings aswell the cameras of the scenes
*	WORKING
*	by Victor Orts
*/

#ifndef _H_SCENE_
#define _H_SCENE_

#include "types.h"
#include "Objects/camera.h"
//#include "Objects\framebuffer.h"
#include "Core/Objects/rendertotexture.h"
#include "Core/Objects/shadowtexture.h"
#include "Components/lightcomponent.h"
#include "Core/Objects/entitiymanager.h"
#include <vector>

namespace Awesome {
	class Scene
	{
	public:
		Scene();
		~Scene();
		void update();
		void render();

		/**
		 * @brief      Sets the name.
		 *
		 * @param[in]  string  String of the name of the scene
		 */
		void setName(string string);
		string name_scene();
		template <class tmpObject>
		std::shared_ptr<tmpObject> CreateObject();
		std::shared_ptr<Texture> CreateTexture(string name);
		std::shared_ptr<Camera> CreateCamera(vec3 position,vec2 near_far,vec3 lookAt, Camera::TypeCamera typecamera);
		std::vector<std::shared_ptr<GameObject>> get_gameobjects();
		/**
		 * @brief      Get the vector of cameras
		 *
		 * @return     Return the vectors of camera
		 */
		std::vector<std::shared_ptr<Camera>> camera();

		/**
		 * @brief      Sets the camera.
		 *
		 * @param[in]  cam_select  Var of the current camera in use
		 */
		void setCamera(uint32 cam_select);

		/**
		 * @brief      Get the current camera
		 *
		 * @return     Return the current camera using in the scene
		 */
		std::shared_ptr<Camera> currentCamera();

		/**
		 * @brief      Adds a light c.
		 *
		 * @param[in]  a     Light to add in the vector of lights
		 */
		void addLightC(std::shared_ptr<Light>a);
		std::vector<std::shared_ptr<Light>> get_lights();


		std::shared_ptr<RenderToTexture> get_rtt();
		std::shared_ptr<Entity> CreateEntity();

		std::shared_ptr<EntityManager> getEntityManager();
		int8 get_number_light();
		int freichen;
		int gradient;
		void initShadowExam() {
			shadowExam = std::make_shared<ShadowTexture>(800,600);
		}
		std::shared_ptr<ShadowTexture> shadowExam;
		private:
		int8 count_Light;
		void addCamera(std::shared_ptr<Camera> cam);
	    std::shared_ptr<RenderToTexture> rtt_;
		std::vector<std::shared_ptr<Light>> light_array_;
		std::shared_ptr<Camera> current_camera_;
		std::vector<std::shared_ptr<Camera>> camera_;
		std::shared_ptr<EntityManager> entityCenter_;
		std::vector<std::shared_ptr<GameObject>> gameobjects_array_;
		string name_;
	};
}
#endif // !_H_SCENE_
