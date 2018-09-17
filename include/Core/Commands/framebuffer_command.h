#ifndef _H_FRAMEBUFFER_COMMAND_
#define _H_FRAMEBUFFER_COMMAND_
#include "command.h"
#include "Core/Objects/rendertotexture.h"
namespace Awesome {
	class BeginFrameBufferCommand : public Command
	{
	public:
		BeginFrameBufferCommand(FrameBuffer*);
		~BeginFrameBufferCommand();
		void execute();
	private:
		FrameBuffer * tmpRTT;
	};

	BeginFrameBufferCommand::BeginFrameBufferCommand(FrameBuffer* rrtt)
	{
		tmpRTT = rrtt;
	}

	BeginFrameBufferCommand::~BeginFrameBufferCommand()
	{
	}

	void BeginFrameBufferCommand::execute() {
		tmpRTT->begin();
	}


	class EndFrameBufferCommand : public Command
	{
	public:
		EndFrameBufferCommand(FrameBuffer*);
		~EndFrameBufferCommand();
		void execute();
	private:
		FrameBuffer * tmpRTT;
	};

	EndFrameBufferCommand::EndFrameBufferCommand(FrameBuffer* rrtt)
	{
		tmpRTT = rrtt;
	}

	EndFrameBufferCommand::~EndFrameBufferCommand()
	{
	}

	void EndFrameBufferCommand::execute() {
		tmpRTT->end();
		
	}
}

#endif