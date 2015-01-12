#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace antlr4 {
namespace runtime {
namespace atn {

	class transition;

	enum class atn_state_type
	{
		invalid,
		basic,
		rule_start,
		block_start,
		plus_block_start,
		star_block_start,
		token_start,
		rule_stop,
		block_end,
		star_loop_back,
		star_loop_entry,
		plus_loop_back,
		loop_end,
	};

	class atn_state
	{
	public:
		size_t state_number() const;
		bool only_has_epsilon_transitions() const;
		bool optimized() const;

		std::vector<std::shared_ptr<atn::transition>> const& transitions() const;
		std::shared_ptr<transition> const& transition(size_t index) const;
		std::vector<std::shared_ptr<atn::transition>> const& optimized_transitions() const;
		std::shared_ptr<atn::transition> const& optimized_transition(size_t index) const;

		virtual int32_t non_stop_state_number() const;
		virtual atn_state_type state_type() const;

	public:
		void add_transition(std::shared_ptr<atn::transition> const& transition);
		void add_transition(size_t index, std::shared_ptr<atn::transition> const& transition);
		void set_transition(size_t index, std::shared_ptr<atn::transition> const& transition);
		std::shared_ptr<atn::transition> remove_transition(size_t index);

		void add_optimized_transition(size_t index, std::shared_ptr<atn::transition> const& transition);
		void set_optimized_transition(size_t index, std::shared_ptr<atn::transition> const& transition);
		std::shared_ptr<atn::transition> remove_optimized_transition(size_t index);
	};

	bool operator== (atn_state const& x, atn_state const& y)
	{
		return x.state_number() == y.state_number();
	}

}
}
}

namespace std {

	template<>
	class hash<antlr4::runtime::atn::atn_state>
	{
		size_t operator() (antlr4::runtime::atn::atn_state const& state) const
		{
			return state.state_number();
		}
	};

}
