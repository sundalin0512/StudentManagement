#pragma once
#include <new>
namespace Sdalin
{

	// TODO: 

	//template <class T> class Vector;

	//template <class T>
	//bool operator==(const Vector<T>& x, const Vector<T>& y);
	//template <class T>
	//bool operator< (const Vector<T>& x, const Vector<T>& y);
	//template <class T>
	//bool operator!=(const Vector<T>& x, const Vector<T>& y);
	//template <class T>
	//bool operator> (const Vector<T>& x, const Vector<T>& y);
	//template <class T>
	//bool operator>=(const Vector<T>& x, const Vector<T>& y);
	//template <class T>
	//bool operator<=(const Vector<T>& x, const Vector<T>& y);

	//template <class T>
	//void swap(Vector<T>& x, Vector<T>& y);


	template <class T>
	class Vector
	{
	public:

		explicit Vector();
		explicit Vector(size_t n);
		Vector(size_t n, const T& value);
		Vector(const Vector<T>& x);

		~Vector();
		Vector<T>& operator=(const Vector<T>& x);

		/// <summary>
		/// 将数组中元素替换为n个t
		/// </summary>
		void      assign(size_t n, const T& t);



		// capacity:
		size_t    size() const noexcept;
		static size_t    max_size() noexcept;
		void      resize(size_t sz);
		void      resize(size_t sz, const T& c);
		size_t    capacity() const noexcept;
		bool      empty() const noexcept;
		void      reserve(size_t n);
		void      shrink_to_fit();

		// element access:
		T&       operator[](size_t n);
		const T& operator[](size_t n) const;
		T&       at(size_t n);
		const T& at(size_t n) const;
		T&       front();
		const T& front() const;
		T&       back();
		const T& back() const;

		//data access
		T*       data() noexcept;
		const T* data() const noexcept;

		// modifiers:
		void     push_back(const T& x);
		void     pop_back();

		T&       insert(size_t position, const T& x);
		T&       insert(size_t position, size_t n, const T& x);

		void       erase(size_t position);
		void       erase(size_t first, size_t last);
		void     swap(Vector<T>&) noexcept;
		void     clear() noexcept;

	private:
		T* m_data;
		size_t m_size;
		size_t m_capacity;
	};

	/* -------------------------------------------------- */
	template <class T>
	Vector<T>::Vector()
	{
		char* ptr = new char[sizeof(T)];
		m_data = reinterpret_cast<T*>(ptr);
		m_size = 0;
		m_capacity = 1;
	}

	template <class T>
	Vector<T>::Vector(const size_t n)
	{
		char* ptr = new char[n * sizeof(T)];
		for (size_t i = 0; i < n; i++)
		{
			new (ptr + i * sizeof(T)) T;
		}
		m_data = reinterpret_cast<T*>(ptr);
		m_size = n;
		m_capacity = n;
	}

	template <class T>
	Vector<T>::Vector(const size_t n, const T& value)
	{
		char* ptr = new char[n * sizeof(T)];
		for (size_t i = 0; i < n; i++)
		{
			new (ptr + i * sizeof(T)) T(value);
		}
		m_data = static_cast<T*>(ptr);
		m_size = n;
		m_capacity = n;
	}

	template <class T>
	Vector<T>::Vector(const Vector<T>& x)
	{
		const int n = x.size();
		char* ptr = new char[n * sizeof(T)];
		for (size_t i = 0; i < n; i++)
		{
			new (ptr + i * sizeof(T)) T(x[i]);
		}
		m_data = static_cast<T*>(ptr);
		m_size = n;
		m_capacity = n;
	}

	template <class T>
	Vector<T>::~Vector()
	{
		for (size_t i = 0; i < m_size; i++)
		{
			m_data[i].~T();
		}
		delete[]  reinterpret_cast<char*>(m_data);
	}

	template <class T>
	Vector<T>& Vector<T>::operator=(const Vector<T>& x)
	{
		for (size_t i = 0; i < x.size(); i++)
		{
			(*this)[i] = x[i];
		}
		return *this;
	}

	template <class T>
	void      Vector<T>::assign(const size_t n, const T& t)
	{
		while (m_capacity < n)
		{
			reserve(m_capacity * 2);
		}
		for (size_t i = 0; i < n; i++)
		{
			(*this)[i] = t;
		}
		m_size = n;
	}



	// capacity:
	template <class T>
	size_t    Vector<T>::size() const noexcept
	{
		return m_size;
	}

	template <class T>
	size_t    Vector<T>::max_size() noexcept
	{
		return (size_t() - 1) / sizeof(T);
	}

	template <class T>
	void      Vector<T>::resize(const size_t sz)
	{
		if (sz < m_size)
		{
			for (size_t i = sz + 1; i < m_size; i++)
			{
				m_data[i].~T();
			}
		}
		else
		{
			while (m_capacity < sz)
			{
				reserve(m_capacity * 2);
			}
			for (size_t i = m_size; i < sz; i++)
			{
				new (&m_data[i]) T;
			}
		}
		m_size = sz;
	}

	template <class T>
	void      Vector<T>::resize(const size_t sz, const T& c)
	{
		if (sz < m_size)
		{
			for (size_t i = sz + 1; i < m_size; i++)
			{
				m_data[i].~T();
			}
		}
		else
		{
			while (m_capacity < sz)
			{
				reserve(m_capacity * 2);
			}
			for (size_t i = m_size; i < sz; i++)
			{
				new (&m_data[i]) T(c);
			}
		}
		m_size = sz;
	}

	template <class T>
	size_t    Vector<T>::capacity() const noexcept
	{
		return m_capacity;
	}

	template <class T>
	bool      Vector<T>::empty() const noexcept
	{
		return m_size == 0;
	}

	template <class T>
	void      Vector<T>::reserve(const size_t n)
	{
		if (m_capacity < n)
		{
			char* ptr = new char[n * sizeof(T)];
			for (size_t i = 0; i < m_size; i++)
			{
				new (ptr + i * sizeof(T)) T(m_data[i]);
			}
			for (size_t i = 0; i < m_size; i++)
			{
				m_data[i].~T();
			}
			for (size_t i = m_size; i < n; i++)
			{
				new (ptr + i * sizeof(T)) T;
			}
			delete[]  reinterpret_cast<char*>(m_data);

			m_data = reinterpret_cast<T*>(ptr);
			m_size = m_size < n ? m_size : n;
			m_capacity = n;
		}
		else if (n < m_size)
		{
			for (size_t i = n + 1; i < m_size; i++)
			{
				m_data[i].~T();
			}
			m_size = n;
		}
		else
		{
			for (size_t i = m_size; i < n; i++)
			{
				new (&m_data[i]) T;
			}
			m_size = n;
		}
	}

	template <class T>
	void      Vector<T>::shrink_to_fit()
	{
		if (m_capacity > m_size)
		{
			const size_t n = m_size;
			char* ptr = new char[n * sizeof(T)];
			for (size_t i = 0; i < m_size; i++)
			{
				new (ptr + i * sizeof(T)) T(m_data[i]);
			}
			for (size_t i = 0; i < m_size; i++)
			{
				m_data[i].~T();
			}
			delete[] static_cast<char*>(m_data);

			m_data = static_cast<T*>(ptr);
			m_size = n;
			m_capacity = n;
		}
	}

	// element access:
	template <class T>
	T&       Vector<T>::operator[](const size_t n)
	{
		return m_data[n];
	}

	template <class T>
	const T& Vector<T>::operator[](const size_t n) const
	{
		return m_data[n];
	}

	template <class T>
	T&       Vector<T>::at(const size_t n)
	{
		return (*this)[n];
	}

	template <class T>
	const T& Vector<T>::at(const size_t n) const
	{
		return (*this)[n];
	}

	template <class T>
	T&       Vector<T>::front()
	{
		return (*this)[0];
	}

	template <class T>
	const T& Vector<T>::front() const
	{
		return (*this)[0];
	}

	template <class T>
	T&       Vector<T>::back()
	{
		return (*this)[m_size - 1];
	}

	template <class T>
	const T& Vector<T>::back() const
	{
		return (*this)[m_size - 1];
	}

	//data access
	template <class T>
	T*       Vector<T>::data() noexcept
	{
		return m_data;
	}

	template <class T>
	const T* Vector<T>::data() const noexcept
	{
		return m_data;
	}

	// modifiers:
	template <class T>
	void     Vector<T>::push_back(const T& x)
	{
		insert(m_size, x);
	}

	template <class T>
	void     Vector<T>::pop_back()
	{
		erase(m_size - 1);
	}

	template <class T>
	T&       Vector<T>::insert(const size_t position, const T& x)
	{
		if (m_size == m_capacity)
		{
			reserve(m_capacity * 2);
		}

		for (size_t i = m_size; i > position; i--)
		{
			(*this)[i] = (*this)[i - 1];
		}
		m_size++;
		new (&m_data[position]) T(x);
		return m_data[position];
	}

	template <class T>
	T&       Vector<T>::insert(const size_t position, const size_t n, const T& x)
	{
		while (m_size + n > m_capacity)
		{
			reserve(m_capacity * 2);
		}

		for (size_t i = m_size + n - 1; i > position + n - 1; i--)
		{
			(*this)[i] = (*this)[i - n];
		}
		m_size += n;
		for (size_t i = 0; i < n; i++)
		{
			new (&m_data[position + i]) T(x);
		}
		return m_data[position];
	}

	template <class T>
	void       Vector<T>::erase(const size_t position)
	{
		if (position > size())
		{
			throw "invalid index";
		}
		m_data[position].~T();
		for (size_t i = position; i < m_size - 1; i++)
		{
			(*this)[i] = (*this)[i + 1];
		}
		m_size--;
	}

	//不删除last
	template <class T>
	void       Vector<T>::erase(const size_t first, const size_t last)
	{
		const size_t reduceSize = last - first;
		for (size_t i = first; i < last; i++)
			m_data[i].~T();
		size_t count = 0;
		for (size_t i = last; i < m_size; i++)
		{
			(*this)[first + count] = (*this)[last + count];
			count++;
		}
		m_size -= reduceSize;
	}
	template <class T>
	void     Vector<T>::swap(Vector<T>& t) noexcept
	{
		auto temp1 = this->m_capacity;
		this->m_capacity = t.m_capacity;
		t.m_capacity = temp1;

		auto temp2 = this->m_data;
		this->m_data = t.m_data;
		t.m_data = temp2;

		auto temp3 = this->m_size;
		this->m_size = t.m_size;
		t.m_size = temp3;
	}

	template <class T>
	void     Vector<T>::clear() noexcept
	{
		for (size_t i = 0; i < m_size; i++)
		{
			m_data[i].~T();
		}
		m_size = 0;
	}

}