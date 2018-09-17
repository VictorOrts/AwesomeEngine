#ifndef _H_DISPLAYLIST_
#define _H_DISPLAYLIST_
#include "Commands/command.h"
#include "Commands/clear_command.h"
#include <vector>
#include <memory>
#include "types.h"
namespace Awesome {
	class Displaylist
	{
	public:
		Displaylist();
		~Displaylist();
		typedef std::shared_ptr<Command> CCommand;
		void addCommand(CCommand co);
		void runDisplaylist();
		void clear_list();
		void setList(std::vector<CCommand> new_list);
	private:
		//List or queue podria ser mejor para hacer pop del primer elemento !
		std::vector<CCommand> list_commands;
		bool finished_;
	};
}
#endif // !_H_DISPLAYLIST_
