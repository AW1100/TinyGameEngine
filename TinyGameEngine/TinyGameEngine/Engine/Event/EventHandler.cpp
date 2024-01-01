#include "EventHandler.h"

void TGE::EventHandler::Add(Event* e)
{
	events.push(e);
}

const TGE::Event* TGE::EventHandler::consume()
{
	Event* ret = events.front();
	events.pop();
	return ret;
}
