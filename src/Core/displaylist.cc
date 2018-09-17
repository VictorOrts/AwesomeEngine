#include "Core/displaylist.h"
#include "Core/Objects/utils.h"
using namespace Awesome;

Displaylist::Displaylist()
{
	finished_ = true;
}

Displaylist::~Displaylist()
{
}

void Displaylist::addCommand(CCommand co) {
	list_commands.push_back(co);
}

void Displaylist::runDisplaylist() {
	//Revisar esta parte !
	for (uint32 i = 0; i < list_commands.size(); i++)
	{
		finished_ = false;
		list_commands[i]->execute();
	}
	finished_ = true;
	clear_list();
}

void Displaylist::clear_list() {
	list_commands.clear();
	list_commands.resize(0,nullptr);
}
void Displaylist::setList(std::vector<CCommand> new_list) {
	if (list_commands.empty() == true && finished_ == true) {
		list_commands = new_list;

	}
	else {
	
	}

}