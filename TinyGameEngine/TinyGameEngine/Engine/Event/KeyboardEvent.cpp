#include "KeyboardEvent.h"
#include "EventHandler.h"

#include <memory>

TGE::KeyboardEvent::KeyboardEvent(EventType et, unsigned char code)
	:Event(EventCategory::Keyboard,et),code(code)
{
	TGE::EventHandler::GetInstance().Add(this);
}


