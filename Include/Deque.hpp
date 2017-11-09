#pragma once
//namespace Sdalin
//{
//	template <class T>
//	class Deque {
//	public:
//		class iterator
//		{
//			
//		};
//		// construct/copy/destroy:
//		Deque();
//		explicit Deque(size_t n);
//		Deque(size_t n, const T& value);
//		Deque(const Deque&);
//
//		~Deque();
//		Deque<T>& operator=(const Deque<T>& x);
//		void assign(size_t n, const T& t);
//
//		// iterators:
//		iterator                begin() noexcept;
//		iterator          begin() const noexcept;
//		iterator                end() noexcept;
//		iterator          end() const noexcept;
//
//		// capacity:
//		size_t size() const noexcept;
//		size_t max_size() const noexcept;
//		void      resize(size_t sz);
//		void      resize(size_t sz, const T& c);
//		void      shrink_to_fit();
//		bool      empty() const noexcept;
//
//		// element access:
//		T&       operator[](size_t n);
//		const T& operator[](size_t n) const;
//		T&       at(size_t n);
//		const T& at(size_t n) const;
//		T&       front();
//		const T& front() const;
//		T&       back();
//		const T& back() const;
//
//		// modifiers:
//		template <class... Args> void emplace_front(Args&&... args);
//		template <class... Args> void emplace_back(Args&&... args);
//		template <class... Args> iterator emplace(iterator position, Args&&... args);
//
//		void push_front(const T& x);
//		void push_front(T&& x);
//		void push_back(const T& x);
//		void push_back(T&& x);
//
//		iterator insert(iterator position, const T& x);
//		iterator insert(iterator position, T&& x);
//		iterator insert(iterator position, size_t n, const T& x);
//
//		void pop_front();
//		void pop_back();
//
//		iterator erase(iterator position);
//		iterator erase(iterator first, iterator last);
//		void     swap(Deque<T>&);
//		void     clear() noexcept;
//
//	protected:
//
//
//
//	protected:
//		static const size_t minDequeMapSize = 8;	/* minimum map size, at least 1 */
//		static const size_t dequeBlockSize =
//				 (sizeof(T) <= 1 ? 16
//				: sizeof(T) <= 2 ? 8
//				: sizeof(T) <= 4 ? 4
//				: sizeof(T) <= 8 ? 2
//				: 1);	/* elements per block (a power of 2) */
//		
//	};
//}