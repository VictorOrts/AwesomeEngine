#ifndef _H_ENGINE_
#define _H_ENGINE_

#include <memory>
#include <map>
#include "types.h"
#include "input.h"
#include "Objects/defaultobjects.h"
#include "taskmanager.h"
#include "render.h"
#include "scene.h"
#include "Objects/window.h"
#include "gui.h"
#include "time.h"
namespace Awesome {
	class Engine {
	public:
		~Engine();

		/**
		 * @brief      Initialize the engine
		 *
		 * @param[in]  a     Name of the window
		 * @param[in]  res   The resolution for the window
		 */
		void initEngine(string a, Window::WindowResolution res);

		/**
		 * @brief      Sets the scene.
		 *
		 * @param[in]  scene  The scene
		 */
		void setScene(std::shared_ptr<Scene> scene);
		void run();
		static Engine * engine_instance();

		////////////////////////////////////////////////////

		/**
		 * @brief      Get the window
		 *
		 * @return     return the window
		 */
		std::shared_ptr<Window> window();

		void reset_instance();
		///////// LOG ////////////
		
		/**
		 * @brief      Adds a log.
		 *
		 * @param[in]  log      The log
		 * @param[in]  typelog  The typelog
		 */
		void add_log(std::string log, kLogColors typelog);
		logGui log_array();
		void clear_log();
		///////// END LOG ////////////
		std::shared_ptr<Input>  get_input();
		void update();
		void render();
		std::shared_ptr<Render> get_render();
		std::shared_ptr<TaskManager> get_task();
		std::shared_ptr<DefaultObjects> defaults();
		void closeEngine();
		std::shared_ptr<Scene> currentScene() const;

		/**
		 * @brief      Gets the time.
		 *
		 * @return     The time.
		 */
		std::shared_ptr<Time> get_time() const;

		/**
		 * @brief      Loads a scene.
		 *
		 * @param[in]  nameFile  The name file
		 */
		void LoadScene(const string& nameFile) const;

		/**
		 * @brief      Saves a scene.
		 *
		 * @param[in]  nameFile  The name file
		 */
		void SaveScene(const string& nameFile) const ;
	private:
		Engine();
		std::shared_ptr<Time> time_;
		std::shared_ptr<DefaultObjects> defaults_;
		//En vez de machacar esta scene hacer un vector de escenas y mantener current para la que esta en uso ! selector
		std::shared_ptr<Scene> currentScene_;
		static Engine *m_pIEngine;
		std::shared_ptr<Render> render_;
		std::shared_ptr<TaskManager> taskM_;
		std::shared_ptr<Window> window_;
		std::shared_ptr<Input> input_;
		logGui logArray_;
		std::shared_ptr<Gui> gui_;
	};
}
#endif // !_H_ENGINE_
