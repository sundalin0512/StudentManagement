#pragma once

#include "Queue.hpp"
#include "Stack.hpp"


namespace Sdalin {
	template<class T>
	class Tree
	{

	public:
		struct Node
		{
			Node* m_parent;
			Node* m_left;
			Node* m_right;
			T m_data;
			Node()
			{
				m_parent = nullptr;
				m_left = nullptr;
				m_right = nullptr;
			}

			explicit Node(const Node& other) : Node()
			{
				m_data = other.m_data;
			}
			explicit Node(const T& data) : Node()
			{
				m_data = data;
			}
			~Node() = default;
		};
		Node* m_root;
		size_t m_size;

	public:
		Tree()
			: m_root(new Node), m_size(0)
		{
			root() = nullptr;
		}
		Tree(const Tree& other)
		{
			m_size = 0;
			Queue<Node*> nodes = other.layer();
			while (!nodes.empty())
			{
				insert(*(nodes.front()));
				nodes.pop();
			}
		}
		~Tree()
		{
			Queue<Node*> nodes = layer();
			while (!nodes.empty())
			{
				delete nodes.front();
				nodes.pop();
			}
			delete m_root;
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
			return node;
		}

		bool erase(const T& data)
		{
			Node* node = query(data);
			if (node == nullptr)
				return false;
			erase(node);
			m_size--;
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

		Node* query(const T& data)
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
		Queue<Node*> layer()
		{
			Queue<Node*> myQueue;
			Queue<Node*> retQueue;
			if (root() == nullptr)
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
		Queue<Node*> DLR()
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
		Queue<Node*> LDR()
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
		Queue<Node*> LRD()
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
		void erase(Node* node)
		{
			Node* maxLeftNode = node->m_left;
			if (maxLeftNode == nullptr)
			{
				if(node->m_right != nullptr)
				{
					// node only has right child
					node->m_data = node->m_right->m_data;
					node->m_right = node->m_right->m_right;
					node->m_left = node->m_right->m_left;
					delete node->m_right;
					return;
				}
				else
				{
					// node is leaf node
					if (node == node->m_parent->m_left)
						node->m_parent->m_left = nullptr;
					else
						node->m_parent->m_right = nullptr;
					delete node;
					return;
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
				node->m_data = maxLeftNode->m_data;
				node->m_right = maxLeftNode->m_right;
				node->m_left = maxLeftNode->m_left;
				delete maxLeftNode;
			}
			else
			{
				// max_left_child's parent is not node
				node->m_data = maxLeftNode->m_data;
				maxLeftNode->m_parent->m_right = maxLeftNode->m_left;
				maxLeftNode->m_left->m_parent = maxLeftNode->m_parent;
				delete maxLeftNode;
			}
		}

		Node*& root()
		{
			return m_root->m_parent;
		}
	};
}
