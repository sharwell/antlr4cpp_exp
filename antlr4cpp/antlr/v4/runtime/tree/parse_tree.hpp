#pragma once

#include <memory>
#include <string>
#include "../misc/visitor.hpp"

namespace antlr4 {
namespace runtime {
namespace tree {

	class terminal_node;
	class error_node;

	class parse_tree : public misc::visitable<parse_tree, terminal_node, error_node>
	{
	public:
		virtual std::shared_ptr<parse_tree> parent() const = 0;
		virtual std::shared_ptr<parse_tree> child(size_t index) const = 0;
		virtual size_t size() const = 0;

		virtual std::pair<size_t, size_t> source_interval() const = 0;
		virtual std::wstring text() const = 0;
		virtual std::wstring tree_text() const = 0;
	};

	class rule_node : public parse_tree
	{
	};

	class terminal_node : public parse_tree
	{
	};

	class error_node : public terminal_node
	{
	};

}
}
}
