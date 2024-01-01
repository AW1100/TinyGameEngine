#pragma once

namespace TGE
{
	enum EventCategory
	{
		None = 0,
		Keyboard = 1,
		Mouse = 2
	};

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMove, MouseScroll,
		AppTick
	};

	class Event
	{
	public:
		Event(EventCategory ec, EventType et)
			:category(ec), type(et)
		{}
		virtual EventCategory GetEventCategory() const
		{
			return category;
		}
		virtual EventType GetEventType() const
		{
			return type;
		}

	protected:
		bool hasHandled = false;
		EventCategory category;
		EventType type;
	};
}