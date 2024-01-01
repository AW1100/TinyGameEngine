#pragma once
#include "Event.h"

#include <queue>
#include <memory>

namespace TGE
{
	class EventHandler
	{
	public:
		void Add(Event* e);
		const Event* consume();
		static EventHandler& GetInstance()
		{
			static EventHandler handler;
			return handler;
		}

	private:
		std::queue<Event*> events;
	};
}