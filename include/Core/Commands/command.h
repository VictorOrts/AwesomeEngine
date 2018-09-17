#ifndef _H_COMMAND__
#define _H_COMMAND__

namespace Awesome {

	/**
	 * @brief      Base class for al commands.
	 */
	class Command
	{
	public:

	
		Command();
		~Command();
		/**
		 * @brief     execute , virtual function where the commands make calls in the graphic library
		 */
		virtual void execute() = 0;

	private:

	};
}
#endif // !_H_COMMAND_
