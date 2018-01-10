#pragma once
#include "DllSettings.h"
#include <map>
#include <cstdarg>

namespace Graphics
{
	template<class T>
	class Library
	{
	public:
		Library();
		~Library();

		template<typename... Args>
		int Emplace(Args&& ...args);
		void Remove(int index);
		void Clear();
		const std::shared_ptr<T> At(const int index)const;
		const std::shared_ptr<T> operator[] (const int index)const;

	private:
		int m_incrIndex;
		std::map<int, std::shared_ptr<T> > m_data;

	};

	template<class T>
	inline Library<T>::Library()
	{
		m_incrIndex = 0;
	}
	
	template<class T>
	inline Library<T>::~Library()
	{
		Clear();
	}

	template<class T>
	template<typename ...Args>
	inline int Library<T>::Emplace(Args && ...args)
	{
		int index = m_incrIndex;
		m_data[m_incrIndex++] = std::shared_ptr<T>(new T(std::forward<Args>(args)...));
		return index;
	}
	
	template<class T>
	inline void Library<T>::Remove(int index)
	{
		auto& found = m_data.find(index);
		if (found != m_data.end())
			m_data.erase(index);
	}
	
	template<class T>
	inline void Library<T>::Clear()
	{
		m_data.clear();
	}

	template<class T>
	inline const std::shared_ptr<T> Library<T>::At(const int index) const
	{
		auto result = m_data.find(index);
		if (result != m_data.end())
		{
			return result->second;
		}
		return nullptr;
	}

	template<class T>
	inline const std::shared_ptr<T> Library<T>::operator[](const int index) const
	{
		return At(index);
	}


}

