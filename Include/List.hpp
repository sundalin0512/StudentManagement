#pragma once
namespace Sdalin
{
	template <class T>
	class List {
	private:

		struct Node
		{
			Node* m_pNext;
			Node* m_pPre;
			T m_data;
			Node()
				: m_pNext(nullptr), m_pPre(nullptr)
			{
				
			}

			explicit Node(const T& data)
				: m_pNext(nullptr), m_pPre(nullptr), m_data(data)
			{
				
			}
			Node(const Node& other);
		};
		Node m_node;
		size_t m_size;
	public:

		class iterator
		{
		public:
			iterator();
			iterator(iterator& other);
			iterator(Node* node);

			~iterator();
			iterator& operator ++ ()
			{
				node = node->m_pNext;
				return *this;
			}
			iterator operator ++ (int)
			{
				iterator tmp = *this;
				node = node->m_pNext;
				return tmp;
			}
			bool operator == (iterator & other) const
			{
				return this->node == other.node;
			}
			bool operator != (iterator & other) const
			{
				return this->node != other.node;
			}

			const T& operator * () const
			{
				return node->m_data;
			}
			Node* operator -> () const
			{
				return node;
			}

			T& operator * ()
			{
				return node->m_data;
			}
			Node* operator -> ()
			{
				return node;
			}

			operator Node*() const
			{
				return node;
			}
		protected:
			Node* node;
		};
	public:
		// construct/copy/destroy:
		List();
		explicit List(int  n);
		List(size_t n, const T& value);
		List(const List<T>& x);
		~List();

		List<T>& operator=(const List<T>& x)
		{
			erase(begin(), end());
			for (auto iter = x.begin(); iter != x.end(); ++iter)
			{
				insert(end(), *iter);
			}
			return *this;
		}
		void assign(size_t n, const T& t);

		// iterators:
		iterator                begin() noexcept;
		iterator          begin() const noexcept;

		iterator                end() noexcept;
		iterator          end() const noexcept;

		// capacity:
		size_t size() const noexcept;

		static size_t max_size() noexcept;

		void      resize(size_t sz);

		void      resize(size_t sz, const T& c);
		bool      empty() const noexcept;

		// element access:
		T&       front();
		const T& front() const;
		T&       back();
		const T& back() const;

		// modifiers:
		void pop_front();
		void push_front(const T& x);
		void push_back(const T& x);
		void pop_back();

		iterator insert(iterator position, const T& x);
		iterator insert(iterator position, size_t n, const T& x);

		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);
		void     swap(List<T>& right) noexcept;
		void     clear() noexcept;


	};

	template<class T>
	List<T>::Node::Node(const Node & other)
	{
		m_data = other.m_data;
	}

	template<class T>
	List<T>::iterator::iterator()
	{
		node = nullptr;
	}

	template<class T>
	List<T>::iterator::iterator(iterator & other)
	{
		node = other.node;
	}

	template<class T>
	List<T>::iterator::iterator(Node * node)
	{
		this->node = node;
	}

	template<class T>
	List<T>::iterator::~iterator()
	{

	}

	// construct/copy/destroy:
	template<class T>
	List<T>::List()
	{
		m_node.m_pNext = &m_node;
		m_node.m_pPre = &m_node;
		m_node.m_data = T();
		m_size = 0;
	}

	template<class T>
	List<T>::List(const int n) : List()
	{
		insert(end(), n, T());
	}

	template<class T>
	List<T>::List(const size_t n, const T & value) : List()
	{
		insert(end(), n, value);
	}

	template<class T>
	List<T>::List(const List<T>& x)
	{
		for (auto iter = x.begin(); iter != x.end(); ++iter)
		{
			insert(end(), *iter);
		}
	}

	template<class T>
	List<T>::~List()
	{
		for (auto iter = begin(); iter != end(); )
		{
			erase(iter++);
		}
	}

	template<class T>
	void List<T>::assign(const size_t n, const T & t)
	{
		erase(begin(), end());
		insert(end(), n, t);
	}

	// iterators:
	template<class T>
	typename List<T>::iterator List<T>::begin() noexcept
	{
		return m_node.m_pNext;
	}

	template<class T>
	typename List<T>::iterator List<T>::begin() const noexcept
	{
		return m_node.m_pNext;
	}

	template<class T>
	typename List<T>::iterator List<T>::end() noexcept
	{
		return &m_node;
	}

	template<class T>
	typename List<T>::iterator List<T>::end() const noexcept
	{
		return m_node.m_pNext->m_pPre;
	}

	// capacity:
	template<class T>
	size_t List<T>::size() const noexcept
	{
		return m_size;
	}

	template<class T>
	size_t List<T>::max_size() noexcept
	{
		return (size_t() - 1) / sizeof(T);
	}

	template<class T>
	void List<T>::resize(const size_t sz)
	{
		resize(sz, T());
		//if (size() > sz)
		//{
		//	auto iter = begin();
		//	for (size_t i = 0; i < sz; i++)
		//		++iter;
		//	while (iter != end())
		//	{
		//		iter = erase(iter);
		//	}
		//	erase(iter);
		//}
		//else
		//{
		//	insert(end(), sz - size(), T());
		//}
	}

	template<class T>
	void List<T>::resize(const size_t sz, const T & c)
	{
		if (size() > sz)
		{
			auto iter = begin();
			for (size_t i = 0; i < sz; i++)
				++iter;
			while (iter != end())
			{
				iter = erase(iter);
			}
			//erase(iter);
		}
		else
		{
			insert(end(), sz - size(), T(c));
		}
	}

	template<class T>
	bool List<T>::empty() const noexcept
	{
		return size() == 0;
	}

	// element access:
	template<class T>
	T & List<T>::front()
	{
		return m_node.m_pNext->m_data;
	}

	template<class T>
	const T & List<T>::front() const
	{
		return m_node.m_pNext->m_data;
	}

	template<class T>
	T & List<T>::back()
	{
		return m_node.m_pPre->m_data;
	}

	template<class T>
	const T & List<T>::back() const
	{
		return m_node.m_pPre->m_data;
	}

	// modifiers:
	template<class T>
	void List<T>::pop_front()
	{
		erase(begin());
	}

	template<class T>
	void List<T>::push_front(const T & x)
	{
		insert(begin(), x);
	}

	template<class T>
	void List<T>::push_back(const T & x)
	{
		insert(end(), x);
	}

	template<class T>
	void List<T>::pop_back()
	{
		erase(m_node.m_pPre);
	}

	template<class T>
	typename List<T>::iterator List<T>::insert(typename List<T>::iterator position, const T & x)
	{
		auto tmp = new Node(x);
		position->m_pPre->m_pNext = tmp;
		tmp->m_pPre = position->m_pPre;
		position->m_pPre = tmp;
		tmp->m_pNext = position;
		m_size++;
		return tmp;
	}

	template<class T>
	typename List<T>::iterator List<T>::insert(const typename List<T>::iterator position, const size_t n, const T & x)
	{
		typename List<T>::iterator ret;
		for (size_t i = 0; i < n; i++)
		{
			ret = insert(position, x);
		}
		return ret;
	}

	template<class T>
	typename List<T>::iterator List<T>::erase(typename List<T>::iterator position)
	{
		if (position == &m_node)
			throw "invalid iterator";
		auto ret = position->m_pNext;
		position->m_pNext->m_pPre = position->m_pPre;
		position->m_pPre->m_pNext = position->m_pNext;
		delete static_cast<Node*>(position);
		m_size--;
		return ret;
	}

	template<class T>
	typename List<T>::iterator List<T>::erase(typename List<T>::iterator first, typename List<T>::iterator last)
	{
		if(m_size==0)
		{
			return nullptr;
		}
		auto ret = last;
		last->m_pPre = first->m_pPre;
		first->m_pPre->m_pNext = last;
		for (auto iter = first; iter != last;)
		{
			delete static_cast<Node*>(iter++);
			m_size--;
		}
		//delete (Node*)first;
		return ret;
	}

	template<class T>
	void List<T>::swap(List<T>& right) noexcept
	{
		auto temp = right;
		right = *this;
		*this = temp;
	}

	template<class T>
	void List<T>::clear() noexcept
	{
		for (auto iter = begin(); iter != end();)
		{
			delete static_cast<Node*>(iter++);
		}
		m_size = 0;
		m_node.m_pNext = &m_node;
		m_node.m_pPre = &m_node;
	}

}


