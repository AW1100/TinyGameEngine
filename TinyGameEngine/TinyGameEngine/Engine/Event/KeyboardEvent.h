#pragma once
#include "Event.h"

namespace TGE
{
	class KeyboardEvent : public Event
	{
	public:
		KeyboardEvent(EventType et, unsigned char code);
	private:
		unsigned char code;
	};
}