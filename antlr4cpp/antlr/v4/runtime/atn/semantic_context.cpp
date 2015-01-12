#include "stdafx.h"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>

#include <antlr/v4/runtime/atn/semantic_context.hpp>
#include <antlr/v4/runtime/misc/unordered_ptr_set.hpp>

namespace antlr4 {
namespace runtime {
namespace atn {

	using namespace antlr4::runtime::misc;

	namespace {

		//typedef std::shared_ptr<semantic_context> semantic_context_ptr;
		using semantic_context_ptr = std::shared_ptr<semantic_context>;

		template<typename Container>
		std::vector<std::shared_ptr<semantic_context::precedence_predicate>> filter_precedence_predicates(Container& container)
		{
			std::vector<std::shared_ptr<semantic_context::precedence_predicate>> result;
			for (auto it = container.begin(); it != container.end(); /*empty*/)
			{
				if ((*it)->type() == semantic_context::context_type::precedence_predicate)
				{
					result.push_back(std::static_pointer_cast<semantic_context::precedence_predicate>(*it));
					container.erase(it++);
				}
				else
				{
					++it;
				}
			}

			return std::move(result);
		}

	}

	const std::shared_ptr<semantic_context> semantic_context::none = std::make_shared<semantic_context::predicate>(-1, -1, false);

	std::shared_ptr<semantic_context> semantic_context::combine_and(std::shared_ptr<semantic_context> const& x, std::shared_ptr<semantic_context> const& y)
	{
		if (!x || x == none)
		{
			return y;
		}

		if (!y || y == none)
		{
			return x;
		}

		auto result = std::make_shared<and_operator>(x, y);
		if (result->operands().size() == 1)
		{
			return result->operands()[0];
		}

		return std::move(result);
	}

	std::shared_ptr<semantic_context> semantic_context::combine_or(std::shared_ptr<semantic_context> const& x, std::shared_ptr<semantic_context> const& y)
	{
		if (!x)
		{
			return y;
		}

		if (!y)
		{
			return x;
		}

		if (x == none || y == none)
		{
			return none;
		}

		auto result = std::make_shared<or_operator>(x, y);
		if (result->operands().size() == 1)
		{
			return result->operands()[0];
		}

		return std::move(result);
	}

	semantic_context::and_operator::and_operator(std::shared_ptr<semantic_context> const& x, std::shared_ptr<semantic_context> const& y)
		: semantic_context(context_type::and_operator)
	{
		unordered_ptr_set<semantic_context_ptr> operands;
		if (x->type() == context_type::and_operator)
		{
			and_operator* x_and = static_cast<and_operator*>(x.get());
			operands.insert(x_and->operands().begin(), x_and->operands().end());
		}
		else
		{
			operands.insert(x);
		}

		if (y->type() == context_type::and_operator)
		{
			and_operator* y_and = static_cast<and_operator*>(y.get());
			operands.insert(y_and->operands().begin(), y_and->operands().end());
		}
		else
		{
			operands.insert(y);
		}

		std::vector<std::shared_ptr<precedence_predicate>> precedence_predicates(filter_precedence_predicates(operands));
		if (!precedence_predicates.empty())
		{
			// interested in the transition with the lowest precedence
			auto reduced = std::min_element(
				precedence_predicates.begin(),
				precedence_predicates.end(),
				[](std::shared_ptr<precedence_predicate> const& x, std::shared_ptr<precedence_predicate> const& y)
			{
				return x->precedence() < y->precedence();
			});
			operands.insert(*reduced);
		}

		std::copy(operands.begin(), operands.end(), std::back_inserter(_operands));
	}

	semantic_context::or_operator::or_operator(std::shared_ptr<semantic_context> const& x, std::shared_ptr<semantic_context> const& y)
		: semantic_context(context_type::or_operator)
	{
		unordered_ptr_set<semantic_context_ptr> operands;
		if (x->type() == context_type::or_operator)
		{
			or_operator* x_or = static_cast<or_operator*>(x.get());
			operands.insert(x_or->operands().begin(), x_or->operands().end());
		}
		else
		{
			operands.insert(x);
		}

		if (y->type() == context_type::or_operator)
		{
			or_operator* y_or = static_cast<or_operator*>(y.get());
			operands.insert(y_or->operands().begin(), y_or->operands().end());
		}
		else
		{
			operands.insert(y);
		}

		std::vector<std::shared_ptr<precedence_predicate>> precedence_predicates(filter_precedence_predicates(operands));
		if (!precedence_predicates.empty())
		{
			// interested in the transition with the highest precedence
			auto reduced = std::max_element(
				precedence_predicates.begin(),
				precedence_predicates.end(),
				[](std::shared_ptr<precedence_predicate> const& x, std::shared_ptr<precedence_predicate> const& y)
				{
					return x->precedence() < y->precedence();
				});
			operands.insert(*reduced);
		}

		std::copy(operands.begin(), operands.end(), std::back_inserter(_operands));
	}

}
}
}
