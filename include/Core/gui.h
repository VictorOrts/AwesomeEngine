/*
*	Class Gui
*	GUI for the engine ,
*	Will show information of the scene , cameras , gameobjects.
*	More things to come
*	by Victor Orts
*/

#ifndef _H_GUI_
#define _H_GUI_

#include "types.h"
#include <functional>
#include "scene.h"
namespace Awesome {
	class Gui
	{
	public:
		enum kTema {
			kTema_BLACK, kTema_WHITE,
			kTema_RED, kTema_BLUE,
			kTema_DARK, kTema_DEFAULT
		};
		Gui();
		~Gui();
		void new_frame();
		void draw();
		uint32 image;
		void setTheme(kTema);

		/**
		 * @brief      Scene management,
		 */

		void sceneManagement();

		/**
		 * @brief      Logs a window.Windos bar in gui
		 */
		void logWindow();
		void camera_info(int *show);

		/**
		 * @brief      Shows the entity information
		 */
		void gameobject_info();
	
		/**
		 * @brief      Select the entity to modify
		 */
		void gameobject_management();

		/**
		 * @brief      Shows light info
		 */
		void light_info();

		/**
		 * @brief      Select the light to modify
		 */
		void light_management();
		void editor_vec3(string, vec3*);
	private:
		struct GuiData;
		GuiData *data_ptr_;
		int16 height_w;
		int16 width_w;
		int16 h_div_4;
		int16 w_div_4;
		//Info go
		vec3 pos_;
		int32 select_theme;
		std::shared_ptr<Scene> curr_scene_;
		int game_object_selection;
		int game_light_selection;

	};
}

#endif // !_H_RENDER_
