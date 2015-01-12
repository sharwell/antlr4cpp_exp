#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>

namespace antlr4 {
namespace runtime {
namespace atn {

	class atn_state;
	class lexer_action_executor;
	class prediction_context;
	class prediction_context_cache;
	class semantic_context;

	class atn_configuration
	{
		atn_configuration() = delete;
		atn_configuration(atn_configuration const&) = delete;
		atn_configuration& operator= (atn_configuration const&) = delete;

	private:
		/**
		 * This field stores the bit mask for implementing the `precedence_filter_suppressed` property as a bit within
		 * the existing `_alt_and_outer_context_depth` field.
		 */
		static const int32_t suppress_precedence_filter = static_cast<int32_t>(0x80000000);

	private:
		std::shared_ptr<atn_state> _state;
		std::shared_ptr<prediction_context> _context;
		std::shared_ptr<lexer_action_executor> _lexer_action_executor;
		std::shared_ptr<semantic_context> _semantic_context;

		int32_t _alt_and_outer_context_depth;
		bool _passed_through_non_greedy_decision;

	public:
		std::shared_ptr<atn_state> const& state() const
		{
			return _state;
		}

		int32_t alternative() const
		{
			return _alt_and_outer_context_depth & 0x00FFFFFF;
		}

		std::shared_ptr<prediction_context> context() const
		{
			return _context;
		}

		bool reaches_into_outer_context() const
		{
			return !!outer_context_depth();
		}

		std::shared_ptr<lexer_action_executor> lexer_action_executor() const
		{
			return _lexer_action_executor;
		}

		std::shared_ptr<semantic_context> semantic_context() const
		{
			return _semantic_context;
		}

		bool passed_through_non_greedy_decision() const
		{
			return _passed_through_non_greedy_decision;
		}

		int32_t outer_context_depth() const
		{
			return static_cast<int32_t>(static_cast<uint32_t>(_alt_and_outer_context_depth) >> 24) & 0x7F;
		}

		void outer_context_depth(int32_t depth)
		{
			assert(depth >= 0);
			// saturate at 0x7F - everything but zero/positive is only used for debug information anyway
			int32_t outer_context_depth = std::min(this->outer_context_depth(), 0x7F);
			_alt_and_outer_context_depth = (outer_context_depth << 24) | (_alt_and_outer_context_depth & ~0x7F000000);
		}

		bool precedence_filter_suppressed() const
		{
			return !!(_alt_and_outer_context_depth & suppress_precedence_filter);
		}

		void precedence_filter_suppressed(bool value)
		{
			if (value)
			{
				_alt_and_outer_context_depth |= suppress_precedence_filter;
			}
			else
			{
				_alt_and_outer_context_depth &= ~suppress_precedence_filter;
			}
		}

		//
		// transformations
		//
	public:
		std::shared_ptr<atn_configuration> transform(std::shared_ptr<atn_state> const& state, bool check_non_greedy) const;
		std::shared_ptr<atn_configuration> transform(std::shared_ptr<atn_state> const& state, std::shared_ptr<class semantic_context> const& semantic_context, bool check_non_greedy) const;
		std::shared_ptr<atn_configuration> transform(std::shared_ptr<atn_state> const& state, std::shared_ptr<prediction_context> const& context, bool check_non_greedy) const;
		std::shared_ptr<atn_configuration> transform(std::shared_ptr<atn_state> const& state, std::shared_ptr<class lexer_action_executor> const& lexer_action_executor, bool check_non_greedy) const;

	private:
		std::shared_ptr<atn_configuration> transform(std::shared_ptr<atn_state> const& state, std::shared_ptr<prediction_context> const& context, std::shared_ptr<class semantic_context> const& semantic_context, std::shared_ptr<class lexer_action_executor> const& lexer_action_executor, bool check_non_greedy) const;

	public:
		std::shared_ptr<atn_configuration> append_context(int32_t context, prediction_context_cache& context_cache) const;
		std::shared_ptr<atn_configuration> append_context(std::shared_ptr<prediction_context> const& context, prediction_context_cache& context_cache) const;
		bool contains(std::shared_ptr<atn_configuration> const& subconfiguration) const;

	};

	bool operator== (antlr4::runtime::atn::atn_configuration const& x, antlr4::runtime::atn::atn_configuration const& y);

}
}
}

namespace std {

	template<>
	struct hash<antlr4::runtime::atn::atn_configuration>
	{
		size_t operator() (antlr4::runtime::atn::atn_configuration const& configuration) const;
	};

}
