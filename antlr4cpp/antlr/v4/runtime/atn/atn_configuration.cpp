/*
 *  Copyright (c) 2015 Sam Harwell, Tunnel Vision Laboratories LLC
 *  All rights reserved.
 *
 *  The source code of this document is proprietary work, and is not licensed for
 *  distribution. For information about licensing, contact Sam Harwell at:
 *      sam@tunnelvisionlabs.com
 */
#include "stdafx.h"

#include <antlr/v4/runtime/atn/atn_configuration.hpp>
#include <antlr/v4/runtime/atn/atn_state.hpp>
#include <antlr/v4/runtime/atn/lexer_action_executor.hpp>
#include <antlr/v4/runtime/atn/prediction_context.hpp>
#include <antlr/v4/runtime/atn/semantic_context.hpp>
#include <antlr/v4/runtime/misc/murmur_hash.hpp>

namespace antlr4 {
namespace runtime {
namespace atn {

	bool operator==(antlr4::runtime::atn::atn_configuration const& x, antlr4::runtime::atn::atn_configuration const& y)
	{
		if (&x == &y)
		{
			return true;
		}

		bool equal = x.state()->state_number() == y.state()->state_number()
			&& x.alternative() == y.alternative()
			&& x.reaches_into_outer_context() == y.reaches_into_outer_context()
			&& *x.context() == *y.context()
			&& *x.semantic_context() == *y.semantic_context()
			&& x.precedence_filter_suppressed() == y.precedence_filter_suppressed()
			&& x.passed_through_non_greedy_decision() == y.passed_through_non_greedy_decision();

		if (!equal)
		{
			return false;
		}

		if (!x.lexer_action_executor())
		{
			return !y.lexer_action_executor();
		}
		else if (!y.lexer_action_executor())
		{
			return false;
		}

		return *x.lexer_action_executor() == *y.lexer_action_executor();
	}

}
}
}

namespace std {

	using namespace antlr4::runtime::misc;

	size_t hash<antlr4::runtime::atn::atn_configuration>::operator()(antlr4::runtime::atn::atn_configuration const& configuration) const
	{
		int32_t hash_code = murmur_hash::initialize(7);
		hash_code = murmur_hash::update(hash_code, configuration.state()->state_number());
		hash_code = murmur_hash::update(hash_code, configuration.alternative());
		hash_code = murmur_hash::update(hash_code, configuration.reaches_into_outer_context() ? 1 : 0);
		hash_code = murmur_hash::update(hash_code, configuration.context());
		hash_code = murmur_hash::update(hash_code, configuration.semantic_context());
		hash_code = murmur_hash::update(hash_code, configuration.passed_through_non_greedy_decision() ? 1 : 0);
		hash_code = murmur_hash::update(hash_code, configuration.lexer_action_executor());
		hash_code = murmur_hash::finish(hash_code, 7);

		return static_cast<size_t>(hash_code);
	}

}
