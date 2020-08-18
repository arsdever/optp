#pragma once

#ifndef CUSTOM_CONSTRUCTOR_TYPE
#define CUSTOM_CONSTRUCTOR_TYPE void*
#endif

namespace ctor
{
	// ===| ! |=| Derived class must have public default constructor        ===| //
	// ===| i |=| In case the interface is disabled, the user has to have a ===| //
	//            <code>static void* ctor()</code> method in the class      ===| //

	template <typename TypeList>
	struct list_size;

	template <typename Type, typename TypeList>
	struct id_of;

	template <typename Class>
	struct constructable
	{
		inline static constexpr CUSTOM_CONSTRUCTOR_TYPE ctor() { return CUSTOM_CONSTRUCTOR_TYPE(new Class()); }
	};

	template <typename ... Types> struct type_list;

	typedef CUSTOM_CONSTRUCTOR_TYPE(*constructor_t)();

	template <typename TypesList>
	struct constructor_mapping
	{
		static constexpr int size = list_size<TypesList>::value;
		static constructor_t map[size];
	};

	namespace _private
	{
		template <typename _S, typename _L> struct id_of_helper;

		template <typename _S, typename _T, typename ... _L>
		struct id_of_helper<_S, type_list<_T, _L ...>>
		{
			static constexpr int v = id_of_helper<_S, type_list<_L ...>>::v + 1;
		};

		template <typename _S, typename ... _L>
		struct id_of_helper<_S, type_list<_S, _L ...>>
		{
			static constexpr int v = 0;
		};

		template <typename _S>
		struct id_of_helper<_S, type_list<>>
		{
			static constexpr int v = -1;
		};

		template <typename _L, typename _CL = _L> struct constructor_mapping_generator_helper;

		template <typename _T, typename ... _L, typename _CL>
		struct constructor_mapping_generator_helper<type_list<_T, _L ...>, _CL>
		{
			inline static constexpr void g()
			{
				constructor_mapping<_CL>::map[id_of<_T, _CL>::value] = &std::conditional<
					std::is_base_of<constructable<_T>, _T>::value,
					constructable<_T>,
					_T>::type::ctor;
				constructor_mapping_generator_helper<type_list<_L ...>, _CL>::g();
			}
		};

		template <typename _L>
		struct constructor_mapping_generator_helper<type_list<>, _L>
		{
			inline static constexpr void g() { }
		};

		template <typename _L> struct list_size_helper;

		template <typename _T, typename ... _L>
		struct list_size_helper<type_list<_T, _L ...>>
		{
			static constexpr int v = list_size_helper<type_list<_L ...>>::v + 1;
		};

		template <>
		struct list_size_helper<type_list<>>
		{
			static constexpr int v = 0;
		};
	}

	template <typename TypeList>
	struct list_size
	{
		static constexpr int value = _private::list_size_helper<TypeList>::v;
	};

	template <typename TypesList>
	constructor_t constructor_mapping<TypesList>::map[constructor_mapping<TypesList>::size];

	template <typename Type, typename TypeList>
	struct id_of
	{
		static constexpr int value = _private::id_of_helper<Type, TypeList>::v;
	};

	template <typename T, typename TypeList = T>
	struct constructor_mapping_generator
	{
		inline static constexpr void generate()
		{
			_private::constructor_mapping_generator_helper<T, TypeList>::g();
		}
	};
}