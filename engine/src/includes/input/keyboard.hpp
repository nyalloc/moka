#pragma once

#include <memory>
#include <array>

namespace moka
{
	enum class key
	{
		none = 0,
		esc,
		enter,
		tab,
		space,
		backspace,
		up,
		down,
		left,
		right,
		insert,
		del,
		home,
		end,
		page_up,
		page_down,
		print,
		plus,
		minus,
		left_bracket,
		right_bracket,
		semicolon,
		quote,
		comma,
		period,
		slash,
		backslash,
		tilde,
		f1,
		f2,
		f3,
		f4,
		f5,
		f6,
		f7,
		f8,
		f9,
		f10,
		f11,
		f12,
		num_pad_0,
		num_pad_1,
		num_pad_2,
		num_pad_3,
		num_pad_4,
		num_pad_5,
		num_pad_6,
		num_pad_7,
		num_pad_8,
		num_pad_9,
		key_0,
		key_1,
		key_2,
		key_3,
		key_4,
		key_5,
		key_6,
		key_7,
		key_8,
		key_9,
		key_a,
		key_b,
		key_c,
		key_d,
		key_e,
		key_f,
		key_g,
		key_h,
		key_i,
		key_j,
		key_k,
		key_l,
		key_m,
		key_n,
		key_o,
		key_p,
		key_q,
		key_r,
		key_s,
		key_t,
		key_u,
		key_v,
		key_w,
		key_x,
		key_y,
		key_z,
	};

	class keyboard_state
	{
		friend class application;

		std::array<bool, 86> keys_;

		void set_key_down(const key key)
		{
			keys_[static_cast<int>(key)] = true;
		}

		void set_key_up(const key key)
		{
			keys_[static_cast<int>(key)] = false;
		}
	public:

		bool is_key_down(const key key) const
		{
			return keys_[static_cast<int>(key)];
		}

		bool is_key_up(const key key) const
		{
			return !is_key_down(key);
		}
	};

	class keyboard
	{
		friend class application;

		inline static keyboard_state state{};
	public:
		static const keyboard_state& get_state()
		{
			return state;
		}
	};
}