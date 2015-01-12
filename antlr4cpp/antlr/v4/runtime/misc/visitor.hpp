#pragma once

namespace antlr4 {
namespace runtime {
namespace misc {

	template<typename... _Types>
	class visitor;

	template<typename _T>
	class visitor<_T>
	{
	public:
		virtual void visit(_T const& node) = 0;
	};

	template<typename _T, typename... _Types>
	class visitor<_T, _Types...> : public visitor<_T>, public visitor<_Types...>
	{
	public:
		using visitor<_T>::visit;
		using visitor<_Types...>::visit;
	};

	template<typename... _Types>
	class visitable
	{
	public:
		virtual void accept(visitor<_Types...>& visitor) const = 0;
	};

}
}
}
