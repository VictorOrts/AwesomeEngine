#ifndef _H_SCRIPT_COMPONENT_
#define _H_SCRIPT_COMPONENT_

#include "Core/component.h"
#include "Core/types.h"
#include "Core/Objects/entity.h"
#include "deps/ChaiScript/include/chaiscript/chaiscript.hpp"
#include "deps/ChaiScript/include/chaiscript/chaiscript_stdlib.hpp"
//#include "Core/Script/script.h"
namespace Awesome {
	class ScriptComponent :public Component {
	public:
		ScriptComponent();
		~ScriptComponent();
		void set_file(string);
		void update();
		void render(mat4 transform, uint32 program);

		std::shared_ptr<Entity> getGameObject(const std::string& name);

	private:
		void scriptInit();
		struct ScriptData;
		ScriptData * data_ptr_;
		string fileName_;
		std::function<void()> onStart_;
		std::function<void()> onUpdate_;
		//std::shared_ptr<Script> script_;
	};
}
#endif // !_H_SCRIPT_COMPONENT_
