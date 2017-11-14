#pragma once
#include "../DataStruct/Queue.hpp"
#include "../DataStruct/Stack.hpp"
#include "types.h"
#include "File.h"
#include <cassert>

namespace Sdalin {
	extern class Manage;
	extern class Manage manage;



	template<class _Ty = void>
	struct lessTag
	{	// functor for operator<
		const _Ty& t1;
		const _Ty& t2;
		lessTag(const _Ty& t1, const _Ty& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.ID < t2.ID;
		}
	};

	template<class _Ty = void>
	struct equalTag
	{
		const _Ty& t1;
		const _Ty& t2;
		equalTag(const _Ty& t1, const _Ty& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.ID < t2.ID;
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
			return key == other.key && equal(value, other.value);
		}
		bool operator >(const Pair& other) const
		{
			return key > other.key || (key == other.key) && (!equal(value, other.value) && !less(value, other.value));
		}
		bool operator <(const Pair& other) const
		{
			return key < other.key ||
				(key == other.key) && less(value,
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
		operator Value&()
		{
			return value;
		}
		operator Value&() const
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


	template <class Value = int, class Hash = HashTag<Value>, class less = lessTag<Value>, class equal = equalTag<Value>, class T = Pair<Value, Hash, less, equal>>
	class FileTree;

	typedef FileTree<Student, HashID<Student>, lessID<Student>, equalID<Student>> studentNameInternalTree;
	typedef FileTree<StudentCourse, HashStudentID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>> courseStudentInternalTree;
	typedef FileTree<StudentCourse, HashCourseID<StudentCourse>, lessCourseID<StudentCourse>, equalCourseID<StudentCourse>> studentCourseInternalTree;
	typedef FileTree<Course, HashID<Course>, lessID<Course>, equalID<Course>> courseNameInternalTree;


	template <class Value = int, class Hash = HashTag<Value>, class less = lessTag<Value>, class equal = equalTag<Value>, class T = Pair<Value, Hash, less, equal>>
	class FileTree
	{
		//using T = int;
		public:
		friend struct TreeHash<studentNameInternalTree>;
		friend struct TreeHash<studentCourseInternalTree>;
		friend struct TreeHash<courseStudentInternalTree>;
		friend struct TreeHash<courseNameInternalTree>;
		friend class Manage;
		struct Node
		{
			size_t dataOffset;
			size_t dataLength;
			Node* m_parent;
			Node* m_left;
			Node* m_right;
			int m_height;
			bool isReady = false;
			T m_data;
			size_t offset;
			Node() : m_data(T(Value()))
			{
				m_parent = nullptr;
				m_left = nullptr;
				m_right = nullptr;
				offset = -1;
				dataOffset = -1;
				dataLength = -1;
				m_height = 0;
			}

			//explicit Node(const Node& other) : Node()
			//{
			//	m_data = other.m_data;
			//}
			explicit Node(const T& data) : m_data(T(data))
			{
				m_parent = nullptr;
				m_left = nullptr;
				m_right = nullptr;
				offset = -1;
				dataOffset = -1;
				dataLength = -1;
				m_height = 0;
			}

			int bfactor() const;

			void fixHeight();

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
		private:


		File& file;

		public:
		operator Value()
		{
			if (root() == nullptr)
				return Value();
			return root()->m_data.value;
		}
		operator Value() const
		{
			if (root() == nullptr)
				return Value();
			return root()->m_data.value;
		}
		FileTree()
			: m_root(new Node), m_size(0), file(manage.file)
		{
			root() = nullptr;
		}
		FileTree(const FileTree& other) :FileTree()
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
		//bool operator < (const FileTree& other)
		//{
		//	return this->m_root->m_data
		//}

		static size_t nodeSize()
		{
			return sizeof(int) * 6;
		}
		bool readTreeFromFile()
		{
			if (m_size == 0)
				return false;
			Node *tmp = new Node;
			readNode(tmp, (size_t)m_root->m_parent);
			m_root->m_parent = tmp;
			m_root->m_parent->m_parent = m_root;
			readNodeFromFile(root());
			//while()
			return true;
		}
		bool readTreeTreeFromFile()
		{
			if (m_size == 0)
				return false;
			Node *tmp = new Node;
			readNode(tmp, (size_t)m_root->m_parent);
			m_root->m_parent = tmp;
			m_root->m_parent->m_parent = m_root;
			
			readNodeFromFile(root());
			if(root() != nullptr)
			{
				Queue<Node*> que = layer();
				while(!que.empty())
				{
					Node* p = que.front();
					p->m_data.value.readTreeFromFile();
					que.pop();
					int ppp = 0;
				}
			}
			//while()
			return true;
		}
		bool readNodeFromFile(Node* node)
		{
			if (node == nullptr)
				return false;
			readData(node);
			if (node->m_left != nullptr)
			{
				Node *tmp = new Node;
				readNode(tmp, (size_t)node->m_left);
				node->m_left = tmp;
				tmp->m_parent = node;
				readData(tmp);
				readNodeFromFile(tmp);
			}
			if (node->m_right != nullptr)
			{
				Node *tmp = new Node;
				readNode(tmp, (size_t)node->m_right);
				node->m_right = tmp;
				tmp->m_parent = node;
				readData(tmp);
				readNodeFromFile(tmp);
			}
		}
		bool writeRoot()
		{
			size_t offset = 0;
			size_t length = nodeSize() + sizeof(size_t);
			Node* node = (Node*)malloc(sizeof(Node));
			memcpy(node, m_root, sizeof(Node));
			if (node->m_parent != nullptr)
				node->m_parent = (Node*)node->m_parent->offset;
			if (node->m_left != nullptr)
				node->m_left = (Node*)node->m_left->offset;
			if (node->m_right != nullptr)
				node->m_right = (Node*)node->m_right->offset;
			*(int*)&(node->isReady) = m_size;
			if (m_root->offset == -1)
			{
				int ret = file.insert(node, length, offset);
				m_root->offset = offset;
				free(node);
				return ret;
			}
			int ret = file.write(node, m_root->offset, length);
			free(node);
			return ret;
		}
		bool readRoot(size_t offset)
		{
			size_t size = 0;
			Node* node = (Node*)malloc(sizeof(Node));
			bool ret = file.read(node, offset, nodeSize() + sizeof(size_t), size);
			if (!ret)
			{
				free(node);
				return ret;
			}
			memcpy(m_root, node, nodeSize());
			m_root->offset = offset;
			m_size = *(int*)&(node->isReady);
			free(node);
			return ret;

		}
		bool writeNode(Node* node)
		{
			size_t offset = 0;
			size_t length = nodeSize();
			Node* pnode = (Node*)malloc(sizeof(Node));
			memcpy(pnode, node, sizeof(Node));
			if (pnode->m_parent != nullptr)
				pnode->m_parent = (Node*)node->m_parent->offset;
			if (pnode->m_left != nullptr)
				pnode->m_left = (Node*)node->m_left->offset;
			if (pnode->m_right != nullptr)
				pnode->m_right = (Node*)node->m_right->offset;
			if (node->offset == -1)
			{
				int ret = file.insert(pnode, length, offset);
				node->offset = offset;
				free(pnode);
				return ret;
			}
			int ret = file.write(pnode, node->offset, length);
			free(pnode);
			return ret;
		}
		bool readNode(Node* node, size_t offset)
		{
			size_t size = 0;
			int ret = file.read(node, offset, nodeSize(), size);
			node->offset = offset;
			return ret;
		}
		bool writeData(Node* node)
		{
			size_t offset = 0;
			size_t length = nodeSize();
			char* buffer = getPairData(node->m_data.value, length);
			if (node->dataOffset == -1)
			{
				int ret = file.insert(buffer, length, offset);
				node->dataOffset = offset;
				node->dataLength = length;
				writeNode(node);
				free(buffer);
				return ret;
			}

			int ret = file.write(buffer, node->dataOffset, node->dataLength);
			free(buffer);
			return ret;
		}
		bool readData(Node* node)
		{
			if (node->dataLength == -1)
				return false;
			int* buffer = (int*)malloc(node->dataLength);
			size_t size;
			int ret = file.read(buffer, node->dataOffset, node->dataLength, size);
			setPairData(node->m_data, buffer, size);
			free(buffer);
			return false;
		}
		//template<class TT>
		//char* getPairData(TT pair, size_t& size);

		int* setPairData(studentNameInternalTree& pair, int* buffer, size_t& size)
		{
			Pair<studentNameInternalTree>* student = (Pair<studentNameInternalTree>*)(int*)(int(&pair) - 4);
			assert(size == 0x20);
			((int*)student)[0] = buffer[0];
			pair.m_root->dataOffset = buffer[1];
			pair.m_root->dataLength = buffer[2];
			*(int*)&(pair.m_root->m_parent) = buffer[3];
			*(int*)&(pair.m_root->m_left  ) = buffer[4];
			*(int*)&(pair.m_root->m_right )=  buffer[5];
			pair.m_root->m_height = buffer[6];
			pair.m_size = buffer[7];
			return buffer;
		}

		int* setPairData(courseNameInternalTree& pair, int* buffer, size_t& size)
		{
			Pair<courseNameInternalTree>* student = (Pair<courseNameInternalTree>*)(int*)(int(&pair) - 4);
			assert(size == 0x20);
			((int*)student)[0] = buffer[0];
			pair.m_root->dataOffset = buffer[1];
			pair.m_root->dataLength = buffer[2];
			*(int*)&(pair.m_root->m_parent) = buffer[3];
			*(int*)&(pair.m_root->m_left) = buffer[4];
			*(int*)&(pair.m_root->m_right) = buffer[5];
			pair.m_root->m_height = buffer[6];
			pair.m_size = buffer[7];
			return buffer;
		}

		int* setPairData(courseStudentInternalTree& pair, int* buffer, size_t& size)
		{
			Pair<courseStudentInternalTree>* student = (Pair<courseStudentInternalTree>*)(int*)(int(&pair) - 4);
			assert(size == 0x20);
			((int*)student)[0] = buffer[0];
			pair.m_root->dataOffset = buffer[1];
			pair.m_root->dataLength = buffer[2];
			*(int*)&(pair.m_root->m_parent) = buffer[3];
			*(int*)&(pair.m_root->m_left) = buffer[4];
			*(int*)&(pair.m_root->m_right) = buffer[5];
			pair.m_root->m_height = buffer[6];
			pair.m_size = buffer[7];
			return buffer;
		}

		int* setPairData(studentCourseInternalTree& pair, int* buffer, size_t& size)
		{
			Pair<studentCourseInternalTree>* student = (Pair<studentCourseInternalTree>*)(int*)(int(&pair) - 4);
			assert(size == 0x20);
			((int*)student)[0] = buffer[0];
			pair.m_root->dataOffset = buffer[1];
			pair.m_root->dataLength = buffer[2];
			*(int*)&(pair.m_root->m_parent) = buffer[3];
			*(int*)&(pair.m_root->m_left) = buffer[4];
			*(int*)&(pair.m_root->m_right) = buffer[5];
			pair.m_root->m_height = buffer[6];
			pair.m_size = buffer[7];
			return buffer;
		}

		int* setPairData(Student& pair, int* buffer, size_t& size)
		{
			Pair<Student>* student = (Pair<Student>*)(int*)(int(&pair) - 4);
			char* p = (char*)buffer;
			student->key = ((int*)p)[0];
			p += sizeof(int*);
			int length = ((int*)p)[0];
			p += sizeof(int*);
			pair.ID = p;
			assert(strlen(p) == pair.ID.GetLength());
		
			p += length;
			length = ((int*)p)[0];
			p += sizeof(int*);
			pair.name = p;
			assert(strlen(p) == pair.name.GetLength());
			assert(size == sizeof(int) + student->value.ID.GetLength() + 1 + student->value.name.GetLength() + 1 + 2 * sizeof(size_t));
			return buffer;
		}
		int* setPairData(Course& pair, int* buffer, size_t& size)
		{
			Pair<Course>* student = (Pair<Course>*)(int(&pair) - 4);
			char* p = (char*)buffer;
			student->key = ((int*)p)[0];
			p += sizeof(int*);
			int length = ((int*)p)[0];
			p += sizeof(int*);
			pair.ID = p;
			assert(strlen(p) == pair.ID.GetLength());

			p += length;
			length = ((int*)p)[0];
			p += sizeof(int*);
			pair.name = p;
			assert(strlen(p) == pair.name.GetLength());
			assert(size == sizeof(int) + student->value.ID.GetLength() + 1 + student->value.name.GetLength() + 1 + 2 * sizeof(size_t));
			return buffer;
		}
		int* setPairData(StudentCourse& pair, int* buffer, size_t& size)
		{
			Pair<StudentCourse>* student = (Pair<StudentCourse>*)(int(&pair) - 4);
			
			char* p = (char*)buffer;
			student->key = ((int*)p)[0];
			p += sizeof(int*);
			int length = ((int*)p)[0];
			p += sizeof(int*);
			pair.studentID = p;
			assert(strlen(p) == pair.studentID.GetLength());

			p += length;
			length = ((int*)p)[0];
			p += sizeof(int*);
			pair.courseID = p;
			assert(strlen(p) == pair.courseID.GetLength());
			p += length;
			pair.grade = ((int*)p)[0];
			assert(size == sizeof(int) + student->value.studentID.GetLength() + 1 + student->value.courseID.GetLength() + 1 + 3 * sizeof(size_t));
			return buffer;
		}

		char* getPairData(studentNameInternalTree& pair, size_t& size);

		char* getPairData(courseNameInternalTree& pair, size_t& size);

		char* getPairData(courseStudentInternalTree& pair, size_t& size);

		char* getPairData(studentCourseInternalTree& pair, size_t& size);

		char* getPairData(Student& pair, size_t& size);

		char* getPairData(Course& pair, size_t& size);

		char* getPairData(StudentCourse& pair, size_t& size);

		bool isEmpty() const;

		Node* insert(const T& data)
		{
			Node* node = new Node(data);
			writeData(node);
			if (root() == nullptr)
			{
				root() = node;
				root()->m_parent = m_root;
				m_size++;
				writeNode(node);
				writeRoot();
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
					file.erase(node->offset);
					file.erase(node->dataOffset);
					delete node;
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
			writeNode(node);
			return node;
		}

		Node* insertNoFile(const T& data)
		{
			Node* node = new Node(data);
			if (root() == nullptr)
			{
				root() = node;
				root()->m_parent = m_root;
				m_size++;
				writeRoot();
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
					delete node;
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
		Queue<Node*> layer() const;

		//先序遍历
		Queue<Node*> DLR() const;

		//中序遍历
		Queue<Node*> LDR() const;

		//后序遍历
		Queue<Node*> LRD() const;

		private:
		//只删除，不平衡
		Node* erase(Node* node);

		Node*& root() const
		{
			return m_root->m_parent;
		}

		Node* lRotate(Node* node);

		Node* rRotate(Node* node);

		Node* balance(Node* p);
		public:
		//friend int Pair<int, FileTree<int, Student>>::Hash()
		//{

		//}

	};

	template <class Value, class Hash, class less, class equal, class T>
	int FileTree<Value, Hash, less, equal, T>::Node::bfactor() const
	{
		return nodeHeight(m_right) - nodeHeight(m_left);
	}

	template <class Value, class Hash, class less, class equal, class T>
	void FileTree<Value, Hash, less, equal, T>::Node::fixHeight()
	{
		const int lHeight = nodeHeight(m_left);
		const int rHeight = nodeHeight(m_right);
		m_height = (lHeight > rHeight ? lHeight : rHeight) + 1;
	}

	template <class Value, class Hash, class less, class equal, class T>
	char* FileTree<Value, Hash, less, equal, T>::getPairData(studentNameInternalTree& pair, size_t& size)
	{
		Pair<studentNameInternalTree>* student = (Pair<studentNameInternalTree>*)(int*)(int(&pair) - 4);
		size = sizeof(int) * 8;
		int* buffer = (int*)malloc(size);
		buffer[0] = ((int*)student)[0];
		buffer[1] = pair.m_root->dataOffset;
		buffer[2] = pair.m_root->dataLength;
		buffer[3] = pair.m_root->m_parent != nullptr ? pair.m_root->m_parent->offset : -1;
		buffer[4] = pair.m_root->m_left != nullptr ? pair.m_root->m_left->offset : -1;
		buffer[5] = pair.m_root->m_right != nullptr ? pair.m_root->m_right->offset : -1;
		buffer[6] = pair.m_root->m_height;
		buffer[7] = pair.m_size;
		return (char*)buffer;
	}

	template <class Value, class Hash, class less, class equal, class T>
	char* FileTree<Value, Hash, less, equal, T>::getPairData(courseNameInternalTree& pair, size_t& size)
	{
		Pair<courseNameInternalTree>* student = (Pair<courseNameInternalTree>*)(int*)(int(&pair) - 4);
		size = sizeof(int) * 8;
		int* buffer = (int*)malloc(size);
		buffer[0] = ((int*)student)[0];
		buffer[1] = pair.m_root->dataOffset;
		buffer[2] = pair.m_root->dataLength;
		buffer[3] = pair.m_root->m_parent != nullptr ? pair.m_root->m_parent->offset : -1;
		buffer[4] = pair.m_root->m_left != nullptr ? pair.m_root->m_left->offset : -1;
		buffer[5] = pair.m_root->m_right != nullptr ? pair.m_root->m_right->offset : -1;
		buffer[6] = pair.m_root->m_height;
		buffer[7] = pair.m_size;
		return (char*)buffer;
	}

	template <class Value, class Hash, class less, class equal, class T>
	char* FileTree<Value, Hash, less, equal, T>::getPairData(courseStudentInternalTree& pair, size_t& size)
	{
		Pair<courseStudentInternalTree>* student = (Pair<courseStudentInternalTree>*)(int*)(int(&pair) - 4);
		size = sizeof(int) * 8;
		int* buffer = (int*)malloc(size);
		buffer[0] = ((int*)student)[0];
		buffer[1] = pair.m_root->dataOffset;
		buffer[2] = pair.m_root->dataLength;
		buffer[3] = pair.m_root->m_parent != nullptr ? pair.m_root->m_parent->offset : -1;
		buffer[4] = pair.m_root->m_left != nullptr ? pair.m_root->m_left->offset : -1;
		buffer[5] = pair.m_root->m_right != nullptr ? pair.m_root->m_right->offset : -1;
		buffer[6] = pair.m_root->m_height;
		buffer[7] = pair.m_size;
		return (char*)buffer;
	}

	template <class Value, class Hash, class less, class equal, class T>
	char* FileTree<Value, Hash, less, equal, T>::getPairData(studentCourseInternalTree& pair, size_t& size)
	{
		Pair<studentCourseInternalTree>* student = (Pair<studentCourseInternalTree>*)(int*)(int(&pair) - 4);
		size = sizeof(int) * 8;
		int* buffer = (int*)malloc(size);
		buffer[0] = ((int*)student)[0];
		buffer[1] = pair.m_root->dataOffset;
		buffer[2] = pair.m_root->dataLength;
		buffer[3] = pair.m_root->m_parent != nullptr ? pair.m_root->m_parent->offset : -1;
		buffer[4] = pair.m_root->m_left != nullptr ? pair.m_root->m_left->offset : -1;
		buffer[5] = pair.m_root->m_right != nullptr ? pair.m_root->m_right->offset : -1;
		buffer[6] = pair.m_root->m_height;
		buffer[7] = pair.m_size;
		return (char*)buffer;
	}

	template <class Value, class Hash, class less, class equal, class T>
	char* FileTree<Value, Hash, less, equal, T>::getPairData(Student& pair, size_t& size)
	{
		Pair<Student>* student = (Pair<Student>*)(int*)(int(&pair) - 4);
		size = sizeof(int) + student->value.ID.GetLength() + 1 + student->value.name.GetLength() + 1 + 2 * sizeof(size_t);
		char* buffer = (char*)malloc(size);
		char* p = buffer;
		((int*)p)[0] = student->key;
		p += sizeof(int*);
		int length = pair.ID.GetLength() + 1;
		((int*)p)[0] = length;
		p += sizeof(int*);
		memcpy(p, pair.ID.GetData(), length);
		p += length;
		length = pair.name.GetLength() + 1;
		((int*)p)[0] = length;
		p += sizeof(int*);
		memcpy(p, pair.name.GetData(), length);
		return buffer;
	}

	template <class Value, class Hash, class less, class equal, class T>
	char* FileTree<Value, Hash, less, equal, T>::getPairData(Course& pair, size_t& size)
	{
		Pair<Course>* student = (Pair<Course>*)(int(&pair) - 4);
		size = sizeof(int) + student->value.ID.GetLength() + 1 + student->value.name.GetLength() + 1 + 2 * sizeof(size_t);
		char* buffer = (char*)malloc(size);
		char* p = buffer;
		((int*)p)[0] = student->key;
		p += sizeof(int*);
		int length = pair.ID.GetLength() + 1;
		((int*)p)[0] = length;
		p += sizeof(int*);
		memcpy(p, pair.ID.GetData(), length);
		p += length;
		length = pair.name.GetLength() + 1;
		((int*)p)[0] = length;
		p += sizeof(int*);
		memcpy(p, pair.name.GetData(), length);
		return buffer;
	}

	template <class Value, class Hash, class less, class equal, class T>
	char* FileTree<Value, Hash, less, equal, T>::getPairData(StudentCourse& pair, size_t& size)
	{
		Pair<StudentCourse>* student = (Pair<StudentCourse>*)(int(&pair) - 4);
		size = sizeof(int) + student->value.studentID.GetLength() + 1 + student->value.courseID.GetLength() + 1 + 3 * sizeof(
			size_t);
		char* buffer = (char*)malloc(size);
		char* p = buffer;
		((int*)p)[0] = student->key;
		p += sizeof(int*);
		int length = pair.studentID.GetLength() + 1;
		((int*)p)[0] = length;
		p += sizeof(int*);
		memcpy(p, pair.studentID.GetData(), length);
		p += length;
		length = pair.courseID.GetLength() + 1;
		((int*)p)[0] = length;
		p += sizeof(int*);
		memcpy(p, pair.courseID.GetData(), length);
		p += length;
		((int*)p)[0] = pair.grade;
		return buffer;
	}

	template <class Value, class Hash, class less, class equal, class T>
	bool FileTree<Value, Hash, less, equal, T>::isEmpty() const
	{
		return m_size == 0;
	}

	template <class Value, class Hash, class less, class equal, class T>
	Queue<typename FileTree<Value, Hash, less, equal, T>::Node*> FileTree<Value, Hash, less, equal, T>::layer() const
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

	template <class Value, class Hash, class less, class equal, class T>
	Queue<typename FileTree<Value, Hash, less, equal, T>::Node*> FileTree<Value, Hash, less, equal, T>::DLR() const
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

	template <class Value, class Hash, class less, class equal, class T>
	Queue<typename FileTree<Value, Hash, less, equal, T>::Node*> FileTree<Value, Hash, less, equal, T>::LDR() const
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

	template <class Value, class Hash, class less, class equal, class T>
	Queue<typename FileTree<Value, Hash, less, equal, T>::Node*> FileTree<Value, Hash, less, equal, T>::LRD() const
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

	template <class Value, class Hash, class less, class equal, class T>
	typename FileTree<Value, Hash, less, equal, T>::Node* FileTree<Value, Hash, less, equal, T>::erase(Node* node)
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
					writeRoot();
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
					writeNode(parent);
				}
				writeNode(right);

				retNode = right;
				goto End;
			}
			else
			{
				Node* left = node->m_left;
				if (node == root())
				{
					if (left != nullptr)
					{
						left->m_parent = m_root;
						writeNode(left);
					}
					root() = left;
					writeRoot();
					retNode = m_root;
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
					writeNode(parent);
					retNode = parent;
				}
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
				writeNode(right);
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
			writeNode(parent);
			writeNode(maxLeftNode);
		}
		else
		{
			// max_left_child's parent is not node
			Node* parent = maxLeftNode->m_parent;
			Node* left = maxLeftNode->m_left;
			if (left != nullptr)
			{
				left->m_parent = maxLeftNode->m_parent;
				writeNode(left);
			}
			parent->m_right = left;

			maxLeftNode->m_left = node->m_left;
			maxLeftNode->m_right = node->m_right;
			Node* nodeLeft = node->m_left;
			Node* nodeRight = node->m_right;

			nodeLeft->m_parent = maxLeftNode;
			writeNode(nodeLeft);
			if (nodeRight != nullptr)
			{
				nodeRight->m_parent = maxLeftNode;
				writeNode(nodeRight);
			}

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
			writeNode(parent);
			writeNode(maxLeftNode);
			retNode = parent;
		}
	End:
		m_size--;
		file.erase(node->offset);
		file.erase(node->dataOffset);
		delete node;
		return retNode;
	}

	template <class Value, class Hash, class less, class equal, class T>
	typename FileTree<Value, Hash, less, equal, T>::Node* FileTree<Value, Hash, less, equal, T>::lRotate(Node* node)
	{
		Node* pnode = node->m_right;
		node->m_right = pnode->m_left;

		if (pnode->m_left != nullptr)
		{
			pnode->m_left->m_parent = node;
			writeNode(pnode->m_left);
		}

		pnode->m_parent = node->m_parent;

		if (node == root())
		{
			root() = pnode;
			writeRoot();
		}
		else if (node == node->m_parent->m_left)
		{
			node->m_parent->m_left = pnode;
			writeNode(node->m_parent);
		}
		else
		{
			node->m_parent->m_right = pnode;
			writeNode(node->m_parent);
		}

		pnode->m_left = node;
		node->m_parent = pnode;
		node->fixHeight();
		pnode->fixHeight();
		writeNode(node);
		writeNode(pnode);
		return pnode;
	}

	template <class Value, class Hash, class less, class equal, class T>
	typename FileTree<Value, Hash, less, equal, T>::Node* FileTree<Value, Hash, less, equal, T>::rRotate(Node* node)
	{
		Node* pnode = node->m_left;
		node->m_left = pnode->m_right;

		if (pnode->m_right != nullptr)
		{
			pnode->m_right->m_parent = node;
			writeNode(pnode->m_right);
		}
		pnode->m_parent = node->m_parent;

		if (node == root())
		{
			root() = pnode;
			writeRoot();
		}
		else if (node == node->m_parent->m_right)
		{
			node->m_parent->m_right = pnode;
			writeNode(node->m_parent);
		}
		else
		{
			node->m_parent->m_left = pnode;
			writeNode(node->m_parent);
		}

		pnode->m_right = node;
		node->m_parent = pnode;
		node->fixHeight();
		pnode->fixHeight();
		writeNode(node);
		writeNode(pnode);
		return pnode;
	}

	template <class Value, class Hash, class less, class equal, class T>
	typename FileTree<Value, Hash, less, equal, T>::Node* FileTree<Value, Hash, less, equal, T>::balance(Node* p)
	{
		p->fixHeight();
		if (p->bfactor() == 2)
		{
			if (p->m_right->bfactor() < 0)
				p->m_right = rRotate(p->m_right);
			return lRotate(p);
		}
		if (p->bfactor() == -2)
		{
			if (p->m_left->bfactor() > 0)
				p->m_left = lRotate(p->m_left);
			return rRotate(p);
		}
		return p; // balancing is not required
	}

}
