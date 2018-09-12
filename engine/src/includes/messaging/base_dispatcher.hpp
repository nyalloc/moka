#pragma once

namespace moka
{
	class close_queue;
	class create_program_cmd;
	class create_shader_cmd;
	class create_vertex_buffer_cmd;
	class frame_cmd;

	class base_dispatcher
	{
	public:
		virtual ~base_dispatcher() = default;
		virtual bool dispatch(const close_queue&) { return false; }
		virtual bool dispatch(const create_program_cmd&) { return false; }
		virtual bool dispatch(const frame_cmd&) { return false; }
		virtual bool dispatch(const create_shader_cmd&) { return false; }
		virtual bool dispatch(const create_vertex_buffer_cmd&) { return false; }
	};
}