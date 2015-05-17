/*
 *  Copyright (c) 2015 Sam Harwell, Tunnel Vision Laboratories LLC
 *  All rights reserved.
 *
 *  The source code of this document is proprietary work, and is not licensed for
 *  distribution. For information about licensing, contact Sam Harwell at:
 *      sam@tunnelvisionlabs.com
 */
#include "stdafx.h"

#include <unordered_map>

#include <antlr/v4/runtime/atn/atn_configuration_set.hpp>
#include <antlr/v4/runtime/atn/conflict_information.hpp>

namespace antlr4 {
namespace runtime {
namespace atn {

	class atn_configuration_set::private_data
	{
	private:
		private_data(private_data const&) = delete;
		private_data& operator= (private_data const&) = delete;

	public:
		explicit private_data()
			: cached_hash_code()
		{
		}

	public:
		std::unordered_map<int64_t, std::shared_ptr<atn_configuration>> merged_configurations;
		std::vector<std::shared_ptr<atn_configuration>> unmerged_configurations;
		std::vector<std::shared_ptr<atn_configuration>> configurations;

		int32_t unique_alternative;
		std::unique_ptr<atn::conflict_information> conflict_information;
		bool has_semantic_context;
		bool dips_into_outer_context;
		bool outermost;

		size_t cached_hash_code;
	};

	bool operator==(antlr4::runtime::atn::atn_configuration_set const& x, antlr4::runtime::atn::atn_configuration_set const& y)
	{
		if (&x == &y)
		{
			return true;
		}

		if (x.outermost() != y.outermost())
		{
			return false;
		}

		if (!(x.conflict_information() == y.conflict_information()))
		{
			return false;
		}

		throw std::runtime_error("equality comparison of std::vector<std::shared_ptr<...>> is not yet implemented");
	}

}
}
}

namespace std {

	using namespace antlr4::runtime::atn;

	size_t hash<atn_configuration_set>::operator()(atn_configuration_set const& configuration_set) const
	{
		if (configuration_set.read_only() && configuration_set.data->cached_hash_code)
		{
			return configuration_set.data->cached_hash_code;
		}

		size_t hash = 1;
		hash = 5 * hash ^ (configuration_set.outermost() ? 1 : 0);
		throw std::runtime_error("hash = 5 * hash ^ hasher(configuration_set.configurations());");

		if (configuration_set.read_only())
		{
			configuration_set.data->cached_hash_code = hash;
		}

		return hash;
	}

}
