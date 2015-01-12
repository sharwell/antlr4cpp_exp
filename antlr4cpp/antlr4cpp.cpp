// antlr4cpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <antlr/test/test_graph_nodes.hpp>
#include <antlr/v4/runtime/tree/parse_tree.hpp>
#include <antlr/v4/runtime/tree/parse_tree_visitor.hpp>

using namespace antlr4::runtime;
using namespace antlr4::runtime::misc;
using namespace antlr4::runtime::tree;

class sample_rule_node;

class sample_rule_node : public rule_node, public visitable<sample_rule_node>
{
public:
	virtual std::shared_ptr<parse_tree> parent() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual std::shared_ptr<parse_tree> child(size_t index) const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual size_t size() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual std::pair<size_t, size_t> source_interval() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual std::wstring text() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual std::wstring tree_text() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual void accept(visitor<parse_tree, terminal_node, error_node>& visitor) const override
	{
		misc::visitor<sample_rule_node>* typed_visitor = dynamic_cast<misc::visitor<sample_rule_node>*>(&visitor);
		if (typed_visitor)
		{
			typed_visitor->visit(*this);
		}
		else
		{
			visitor.visit(*this);
		}
	}

	virtual void accept(visitor<sample_rule_node>& visitor) const override
	{
		visitor.visit(*this);
	}
};

class sample_rule_node2 : public rule_node, public visitable<sample_rule_node2>
{
public:
	virtual std::shared_ptr<parse_tree> parent() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual std::shared_ptr<parse_tree> child(size_t index) const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual size_t size() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual std::pair<size_t, size_t> source_interval() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual std::wstring text() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual std::wstring tree_text() const override
	{
		throw std::runtime_error("not implemented");
	}

	virtual void accept(visitor<parse_tree, terminal_node, error_node>& visitor) const override
	{
		misc::visitor<sample_rule_node2>* typed_visitor = dynamic_cast<misc::visitor<sample_rule_node2>*>(&visitor);
		if (typed_visitor)
		{
			typed_visitor->visit(*this);
		}
		else
		{
			visitor.visit(*this);
		}
	}

	virtual void accept(visitor<sample_rule_node2>& visitor) const override
	{
		visitor.visit(*this);
	}
};

class sample_parser_visitor : public visitor<sample_rule_node, sample_rule_node2>
{
};

template<typename _TResult>
class sample_parser_visitor_impl : public parse_tree_visitor_impl<_TResult>, public sample_parser_visitor
{
public:
	using parse_tree_visitor_impl<_TResult>::visit;

	virtual void visit(sample_rule_node const& node) override
	{
		visit_children(node);
	}

	virtual void visit(sample_rule_node2 const& node) override
	{
		visit_children(node);
	}
};

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
	sample_rule_node ptr;
	sample_rule_node2 ptr2;

	sample_parser_visitor_impl<int> visitor1;
	visitor<sample_rule_node>* typed_visitor = &visitor1;
	visitor<sample_rule_node2>* typed_visitor1 = &visitor1;
	visitor1.visit(ptr);
	visitor1.visit(ptr2);
	typed_visitor->visit(ptr);
	typed_visitor1->visit(ptr2);

	parse_tree_visitor_impl<int> visitor2;
	visitor2.visit(ptr);

	antlr::test::test_graph_nodes();
	return 0;
}
