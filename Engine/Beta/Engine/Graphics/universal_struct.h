#pragma once
#include "DllSettings.h"
#include <typeinfo>

namespace Graphics
{
	/*Data structure to hold abstract data for a "universal_map".
	Data that is passed to this class must be owned by a shared_ptr
	To encase an object, two methods exist:
	-Calling the constructor with object in the args
	-Calling the function Encase() from an existing "universal_struct"
	Encase() will not work if the "universal_struct" is already encasing something else.*/
	class universal_struct
	{
	public:
		universal_struct();
		std::string name()const;
		std::size_t hash_code()const;

		template<typename T>
		universal_struct(const std::shared_ptr<T>& obj);

		template<typename T>
		void Encase(const std::shared_ptr<T>& obj);

		//Returns nullptr on failed
		template<typename T>
		std::shared_ptr<T> As()const;

	private:

		bool m_empty = true;
		std::shared_ptr<void> m_obj = nullptr;
		const std::type_info* m_obj_info;
	};

	inline universal_struct::universal_struct():
		m_obj_info(nullptr)
	{
		m_empty = true;
		m_obj = nullptr;
	}

	inline std::string universal_struct::name() const
	{
		return m_obj_info->name();
	}

	inline std::size_t universal_struct::hash_code() const
	{
		return m_obj_info->hash_code();
	}

	template<typename T>
	inline universal_struct::universal_struct(const std::shared_ptr<T>& obj):
		m_obj_info(&typeid(T))
	{
		if (m_empty)
		{
			m_empty = false;
			m_obj = std::static_pointer_cast<void>(obj);
		}
	}

	template<typename T>
	inline void universal_struct::Encase(const std::shared_ptr<T>& obj)
	{
		if(m_empty)
			*this = universal_struct(obj);
	}

	template<typename T>
	inline std::shared_ptr<T> universal_struct::As()const
	{
		if(m_obj_info->hash_code() == typeid(T).hash_code())
			return std::static_pointer_cast<T>(m_obj);

		return nullptr;
	}
}
