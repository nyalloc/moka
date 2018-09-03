#pragma once

namespace moka
{
	class mouse_motion;
	class key_down;
	class close_queue;
	class create_program;

	class base_dispatcher
	{
	public:
		virtual ~base_dispatcher() = default;
		virtual bool dispatch(const mouse_motion&) { return false; }
		virtual bool dispatch(const key_down&) { return false; }
		virtual bool dispatch(const close_queue&) { return false; }
		virtual bool dispatch(const create_program&) { return false; }
	};
}