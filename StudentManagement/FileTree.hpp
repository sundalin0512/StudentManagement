#pragma once
#include "../DataStruct/Queue.hpp"
#include "../DataStruct/Stack.hpp"
#include "types.h"

namespace Sdalin {
	template<class _Ty = void>
	struct lessTag
	{	// functor for operator<

		constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator< to operands
			return (_Left < _Right);
		}
	};

	template<class _Ty = void>
	struct equalTag
	{
		constexpr bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator< to operands
			return (_Left == _Right);
		}
	};

	template<class _Ty = void>
	struct HashTag
	{
		int operator()(const _Ty& t) const
		{
			return (t.Hash());
		}
	};

	int HashTag<int> ::operator()(const int& t)const
	{
		return t;
	}



	template <class Value, class Hash = HashTag<Value>, class less = lessTag<Value>, class equal = equalTag<Value>>
	class Pair
	{
		public:
		int key;
		Value value;
		Pair() {}
		Pair(Value& value)
		{
			this->value = value;
			key = Hash(value);
		}
		~Pair()
		{
			
		}
		Pair& operator = (const Pair& other)
		{
			this->key = other.key;
			this->value = other.value;
			return *this;
		}
		bool operator ==(const Pair& other) const
		{
			return key == other.key && equal(value , other.value);
		}
		bool operator >(const Pair& other) const
		{
			return key > other.key && !equal(value, other.value)&& !less(value, other.value);
		}
		bool operator <(const Pair& other) const
		{
			return key < other.key && 
				less(value , 
				other.value);
		}
		operator Value()
		{
			return value;
		}
		operator Value() const
		{
			return value;
		}

	};


	template <class T>
	struct TreeHash
	{
		T& t;
		TreeHash(T& t) : t(t) {}
		operator int()
		{
			if (t.m_size == 0)
				return -1;
			return t.root()->m_data.key;
		}
	};

	template <class Value = int, class Hash = HashTag<Value>, class less = lessTag<Value>, class equal = equalTag<Value>, class T = Pair<Value, Hash, less,equal>>
	class FileTree
	{
		//using T = int;
		public:
		friend struct TreeHash<FileTree<StudentCourse, HashCourseID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>>>;
		friend struct TreeHash<FileTree<StudentCourse, HashStudentID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>>>;

		struct Node
		{
			size_t offset;
			size_t length;
			size_t dataOffset;
			size_t dataLength;
			Node* m_parent;
			Node* m_left;
			Node* m_right;
			int m_height;
			T m_data;
			Node() :m_data(T(Value()))
			{
				m_parent = nullptr;
				m_left = nullptr;
				m_right = nullptr;
				offset = -1;
				length = -1;
				dataOffset = -1;
				dataLength = -1;
				m_height = 0;
			}

			explicit Node(const Node& other) : Node()
			{
				m_data = other.m_data;
			}
			explicit Node(const T& data) : Node()
			{
				m_data = data;
			}

			int bfactor() const
			{
				return nodeHeight(m_right) - nodeHeight(m_left);
			}

			void fixHeight()
			{
				const int lHeight = nodeHeight(m_left);
				const int rHeight = nodeHeight(m_right);
				m_height = (lHeight > rHeight ? lHeight : rHeight) + 1;
			}

			operator T() const
			{
				return m_data;
			}
			operator T&()
			{
				return m_data;
			}
			~Node() {}
			private:
			static int nodeHeight(Node* p)
			{
				return p != nullptr ? p->m_height : 0;
			}
		};
		Node* m_root;
		size_t m_size;

		public:
		FileTree()
			: m_root(new Node), m_size(0)
		{
			root() = nullptr;
		}
		FileTree(const FileTree& other):FileTree()
		{
			m_size = 0;
			Queue<Node*> nodes = other.layer();
			while (!nodes.empty())
			{
				insert(*(nodes.front()));
				nodes.pop();
			}
		}
		~FileTree()
		{
			Queue<Node*> nodes = layer();
			while (!nodes.empty())
			{
				delete nodes.front();
				nodes.pop();
			}
			delete m_root;
		}
		FileTree& operator=(const FileTree& other)
		{
			Queue<Node*> nodes = layer();
			while (!nodes.empty())
			{
				delete nodes.front();
				nodes.pop();
			}
			nodes = other.layer();
			while (!nodes.empty())
			{
				insert(*(nodes.front()));
				nodes.pop();
			}
			return *this;
		}
		bool isEmpty() const
		{
			return m_size == 0;
		}

		Node* insert(const T& data)
		{
			Node* node = new Node(data);
			if (root() == nullptr)
			{
				root() = node;
				root()->m_parent = m_root;
				m_size++;
				return root();
			}
			Node* parent = root();
			Node* nextNode = parent;
			Stack<Node*> balanceNodes;
			while (nextNode != nullptr)
			{
				parent = nextNode;
				balanceNodes.push(parent);
				if (parent->m_data == data)
				{
					return parent;
				}
				else if (parent->m_data > data)
				{
					nextNode = parent->m_left;
				}
				else if (parent->m_data < data)
				{
					nextNode = parent->m_right;
				}
			}
			if (parent->m_data > data)
			{
				parent->m_left = node;
				node->m_parent = parent;
			}
			else if (parent->m_data < data)
			{
				parent->m_right = node;
				node->m_parent = parent;
			}
			m_size++;
			balance(node);
			while (!balanceNodes.empty())
			{
				balance(balanceNodes.top());
				balanceNodes.pop();
			}
			return node;
		}
		bool erase(const T& data)
		{
			Node* node = query(data);
			if (node == nullptr)
				return false;
			Node* enbalanceNode = erase(node);
			while (m_root != enbalanceNode)
			{
				enbalanceNode = balance(enbalanceNode)->m_parent;
			}
			return true;
		}

		bool modify(const T& oldData, const T& newData)
		{
			if (!erase(oldData))
			{
				return false;
			}
			insert(newData);
			return true;
		}

		Node* query(const T& data) const
		{
			Node* parent = root();
			if (parent == nullptr)
				return nullptr;
			Node* nextNode = parent;
			while (nextNode != nullptr)
			{
				parent = nextNode;
				if (parent->m_data == data)
				{
					return parent;
				}
				else if (parent->m_data > data)
				{
					nextNode = parent->m_left;
				}
				else if (parent->m_data < data)
				{
					nextNode = parent->m_right;
				}
			}
			return nullptr;
		}

		//层序遍历
		Queue<Node*> layer() const
		{
			Queue<Node*> myQueue;
			Queue<Node*> retQueue;
			if (root() == nullptr || m_size == 0)
			{
				return myQueue;
			}

			myQueue.push(root());
			retQueue.push(root());

			while (!myQueue.empty())
			{
				Node* pNode = myQueue.front();
				if (pNode->m_left != nullptr)
				{
					myQueue.push(pNode->m_left);
					retQueue.push(pNode->m_left);
				}

				if (pNode->m_right != nullptr)
				{
					myQueue.push(pNode->m_right);
					retQueue.push(pNode->m_right);
				}

				myQueue.pop();
			}
			return retQueue;

		}

		//先序遍历
		Queue<Node*> DLR() const
		{
			Stack<Node*> myStack;
			Queue<Node*> retQueue;
			if (root() == nullptr)
			{
				return retQueue;
			}
			myStack.push(root());
			Node* pNode = myStack.top();
			while (!myStack.empty())
			{
				while (pNode != nullptr)
				{
					retQueue.push(pNode);
					myStack.push(pNode);
					pNode = pNode->m_left;
				}
				if (!myStack.empty())
				{
					pNode = myStack.top();
					myStack.pop();
					pNode = pNode->m_right;
				}
			}
			return retQueue;
		}

		//中序遍历
		Queue<Node*> LDR() const
		{
			Stack<Node*> myStack;
			Queue<Node*> retQueue;
			if (root() == nullptr)
			{
				return retQueue;
			}
			myStack.push(root());
			Node* pNode = myStack.top();
			while (!myStack.empty())
			{
				while (pNode != nullptr)
				{
					myStack.push(pNode);
					pNode = pNode->m_left;
				}
				if (!myStack.empty())
				{
					pNode = myStack.top();
					retQueue.push(pNode);
					myStack.pop();
					pNode = pNode->m_right;
				}
			}
			//retQueue.pop();
			return retQueue;
		}

		//后序遍历
		Queue<Node*> LRD() const
		{
			Stack<Node*> myStack;
			Stack<Node*> myStack2;
			Queue<Node*> retQueue;
			if (root() == nullptr)
			{
				return retQueue;
			}
			myStack.push(root());
			Node* pNode = myStack.top();
			while (!myStack.empty())
			{
				while (pNode != nullptr)
				{
					myStack.push(pNode);
					myStack2.push(pNode);
					pNode = pNode->m_right;
				}

				if (!myStack.empty())
				{
					pNode = myStack.top();
					myStack.pop();
					pNode = pNode->m_left;

				}
			}
			while (!myStack2.empty())
			{
				retQueue.push(myStack2.top());
				myStack2.pop();
			}
			return retQueue;
		}

		private:
		//只删除，不平衡
		Node* erase(Node* node)
		{
			Node* maxLeftNode = node->m_left;
			Node* retNode;
			if (maxLeftNode == nullptr)
			{
				if (node->m_right != nullptr)
				{
					// node only has right child
					Node* right = node->m_right;
					if (node == root())
					{
						right->m_parent = m_root;
						root() = right;
					}
					else
					{
						right->m_parent = node->m_parent;
						Node* parent = node->m_parent;
						if (parent->m_left == node)
						{
							parent->m_left = right;
						}
						else
						{
							parent->m_right = right;
						}
					}
					retNode = right;
					goto End;
				}
				else
				{
					// node is leaf node
					Node* parent = node->m_parent;
					if (node == parent->m_left)
					{
						parent->m_left = nullptr;
					}
					else
					{
						parent->m_right = nullptr;
					}

					retNode = parent;
					goto End;
				}
			}
			while (maxLeftNode->m_right != nullptr)
			{
				// search max left child
				maxLeftNode = maxLeftNode->m_right;
			}
			if (maxLeftNode->m_parent == node)
			{
				// max_left_child's parent is node
				Node* parent = node->m_parent;
				maxLeftNode->m_parent = parent;
				if (node == parent->m_left)
				{
					parent->m_left = maxLeftNode;
				}
				else
				{
					parent->m_right = maxLeftNode;
				}
				maxLeftNode->m_right = node->m_right;
				if (maxLeftNode->m_right != nullptr)
				{
					Node* right = maxLeftNode->m_right;
					right->m_parent = maxLeftNode;
				}
				if (node != root())
				{
					retNode = parent;
				}
				else
				{
					root() = maxLeftNode;
					maxLeftNode->m_parent = m_root;
					retNode = root();
				}

			}
			else
			{
				// max_left_child's parent is not node
				Node* parent = maxLeftNode->m_parent;
				Node* left = maxLeftNode->m_left;
				if (left != nullptr)
					left->m_parent = maxLeftNode->m_parent;
				parent->m_right = left;

				maxLeftNode->m_left = node->m_left;
				maxLeftNode->m_right = node->m_right;
				Node* nodeLeft = node->m_left;
				Node* nodeRight = node->m_right;

				nodeLeft->m_parent = maxLeftNode;
				if (nodeRight != nullptr)
					nodeRight->m_parent = maxLeftNode;

				Node* nodeParent = node->m_parent;
				if (node != root())
				{
					maxLeftNode->m_parent = nodeParent;
					if (node == nodeParent->m_left)
					{
						nodeParent->m_left = maxLeftNode;
					}
					else
					{
						nodeParent->m_right = maxLeftNode;
					}
				}
				retNode = parent;

			}
		End:
			m_size--;
			delete node;
			return retNode;
		}

		Node*& root() const
		{
			return m_root->m_parent;
		}

		Node* lRotate(Node* node) const
		{
			Node* pnode = node->m_right;
			node->m_right = pnode->m_left;

			if (pnode->m_left != nullptr)
			{
				pnode->m_left->m_parent = node;
			}

			pnode->m_parent = node->m_parent;

			if (node == root())
			{
				root() = pnode;
			}
			else if (node == node->m_parent->m_left)
			{
				node->m_parent->m_left = pnode;
			}
			else
			{
				node->m_parent->m_right = pnode;
			}

			pnode->m_left = node;
			node->m_parent = pnode;
			node->fixHeight();
			pnode->fixHeight();
			return pnode;
		}

		Node* rRotata(Node* node) const
		{
			Node* pnode = node->m_left;
			node->m_left = pnode->m_right;

			if (pnode->m_right != nullptr)
				pnode->m_right->m_parent = node;
			pnode->m_parent = node->m_parent;

			if (node == root())
			{
				root() = pnode;
			}
			else if (node == node->m_parent->m_right)
			{
				node->m_parent->m_right = pnode;
			}
			else
			{
				node->m_parent->m_left = pnode;
			}

			pnode->m_right = node;
			node->m_parent = pnode;
			node->fixHeight();
			pnode->fixHeight();
			return pnode;
		}
		Node* balance(Node* p) const
		{
			p->fixHeight();
			if (p->bfactor() == 2)
			{
				if (p->m_right->bfactor() < 0)
					p->m_right = rRotata(p->m_right);
				return lRotate(p);
			}
			if (p->bfactor() == -2)
			{
				if (p->m_left->bfactor() > 0)
					p->m_left = lRotate(p->m_left);
				return rRotata(p);
			}
			return p; // balancing is not required
		}
		public:
		//friend int Pair<int, FileTree<int, Student>>::Hash()
		//{

		//}

	};

}
