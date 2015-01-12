#pragma once

#include <memory>
#include <vector>

namespace antlr4 {
namespace runtime {
namespace atn {

	class atn_configuration;
	class atn_simulator;
	class atn_state;
	class conflict_information;
	class prediction_context_cache;

	class atn_configuration_set
	{
		class private_data;

		atn_configuration_set(atn_configuration_set const&) = delete;
		atn_configuration_set& operator= (atn_configuration_set const&) = delete;

		friend bool operator== (atn_configuration_set const& x, atn_configuration_set const& y);
		friend std::hash<atn_configuration_set>;

	private:
		std::unique_ptr<private_data> data;

	public:
		explicit atn_configuration_set();
		~atn_configuration_set();

		bool read_only() const;
		bool outermost() const;
		size_t size() const;
		bool empty() const;

		int32_t unique_alternative() const;
		bool has_semantic_context() const;
		std::unique_ptr<conflict_information> const& conflict_information() const;
		bool dips_into_outer_context() const;

		std::vector<bool> represented_alternatives() const;
		std::vector<std::shared_ptr<atn_state>> states() const;
		std::vector<std::shared_ptr<atn_configuration>> const& configurations() const;

	public:
		bool contains(std::shared_ptr<atn_configuration> const& configuration) const;

	public:
		bool add(std::shared_ptr<atn_configuration> const& configuration, prediction_context_cache& context_cache);
		void clear();
		void optimize(std::shared_ptr<atn_simulator> const& interpreter);
	};

	bool operator== (atn_configuration_set const& x, atn_configuration_set const& y);

}
}
}

namespace std {

	template<>
	struct hash<antlr4::runtime::atn::atn_configuration_set>
	{
		size_t operator()(antlr4::runtime::atn::atn_configuration_set const& x) const;
	};

}

//#include "atn_configuration_set.inl"
