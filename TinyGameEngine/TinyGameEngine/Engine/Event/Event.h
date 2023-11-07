#pragma once

namespace TGE
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMove, MouseScroll,
		AppTick
	};

	enum EventCategory
	{
		None = 0,
		Input = 1,
		Application = 2,
		Mouse = 4,
		Keyboard = 8
	};

	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;

	protected:
		bool hasHandled = false;
	};
}