#include "File.h"
#include "../DataStruct/Stack.hpp"

namespace Sdalin
{
	FileBase::FileBase()
	{

	}

	FileBase::FileBase(const String fileName, const bool trunc)
		: stream(fileName.c_str(), fstream::in | fstream::out | fstream::binary | (trunc ? fstream::trunc : NULL))
	{

	}

	FileBase::~FileBase()
	{

	}

	bool FileBase::open(String fileName, const bool trunc)
	{
		stream.open(fileName.c_str(), fstream::in | fstream::out | fstream::binary | (trunc ? fstream::trunc : NULL));
		if (stream.is_open())
			return true;
		return false;
	}

	bool FileBase::_write(const size_t offsetFromFileStart, const void * buffer, const size_t bytesToWrite)
	{
		stream.seekp(offsetFromFileStart);
		stream.write((char*)buffer, bytesToWrite);
		if (stream.bad())
			return false;
		//stream.flush();
		return true;
	}

	bool FileBase::_read(const size_t offsetFromFileStart, void * buffer, const size_t bytesToRead)
	{
		stream.seekg(offsetFromFileStart);
		stream.read((char*)buffer, bytesToRead);
		if (stream.fail() || stream.eof())
			return false;
		return true;
	}

	bool FileBase::append(const void* buffer, const size_t bytesToWrite, size_t& offsetWrite)
	{
		stream.seekp(0, fstream::end);
		offsetWrite = stream.tellp();
		return _write(offsetWrite, buffer, bytesToWrite);
	}

	UsedFile::Node::Node() : m_offset(0), m_length(0), m_leftChild(-1), m_rightChild(-1),
		m_parent(-1), m_height(0), m_inFileOffset(-1)
	{
	}

	UsedFile::Node::Node(const size_t offset, const size_t length) : Node()
	{
		m_offset = offset;
		m_length = length;
	}

	size_t UsedFile::Node::size()
	{
		return sizeof(Node) - sizeof(size_t);
	}

	UsedFile::UsedFile(String fileName, const bool trunc) : FileBase(fileName, trunc), m_head{ 0,-1,-1 }
	{
		if (trunc)
			writeHead();
		else
			readHead();
	}

	bool UsedFile::empty() const
	{
		return m_head.nodeSize == 0;
	}

	int UsedFile::insert(const size_t offset, const size_t length)
	{
		return insert(Node(offset, length));
	}

	int UsedFile::erase(const size_t _offset)
	{
		int offset = _query(_offset);
		if (offset == -1)
			return -1;
		Node node = readNode(offset);
		int ret = node.m_length;
		int enbalanceNodeOffset = 0;
		erase(node, enbalanceNodeOffset);
		Node enbalanceNode = readNode(enbalanceNodeOffset);
		while (enbalanceNode.m_inFileOffset != -1)
		{
			enbalanceNodeOffset = balance(enbalanceNode);
			enbalanceNode = readNode(enbalanceNodeOffset);
		}
		return ret;
	}

	bool UsedFile::query(const size_t offset, size_t& length)
	{
		int fileOffset = _query(offset);
		length = 0;
		if (fileOffset != -1)
		{
			length = readNode(fileOffset).m_length;
			return true;
		}
		return false;
	}

	int UsedFile::insert(Node& node)
	{
		const int offset = node.m_offset;
		const int length = node.m_length;
		if (length <= 0)
			throw "invalid length";

		if (empty())
		{
			insertNewNode(node);
			m_head.rootNodeOffset = node.m_inFileOffset;
			writeHead();
			return node.m_inFileOffset;
		}
		else
		{
			Node thisNode = readNode(m_head.rootNodeOffset);
			Node nextNode = thisNode;
			Stack<int> balanceNodesOffset;
			while (nextNode.m_offset != -1)
			{
				thisNode = nextNode;
				balanceNodesOffset.push(thisNode.m_inFileOffset);
				if (thisNode.m_offset == offset)
				{
					if (thisNode.m_leftChild != -1)
						nextNode = readNode(thisNode.m_leftChild);
					else
						nextNode.m_offset = -1; //用offset=-1来标记结束
				}
				else if (thisNode.m_offset > offset)
				{
					if (thisNode.m_leftChild != -1)
						nextNode = readNode(thisNode.m_leftChild);
					else
						nextNode.m_offset = -1; //用offset=-1来标记结束
				}
				else if (thisNode.m_offset < offset)
				{
					if (thisNode.m_rightChild != -1)
						nextNode = readNode(thisNode.m_rightChild);
					else
						nextNode.m_offset = -1; //用offset=-1来标记结束
				}
			}
			insertNewNode(node);
			if (thisNode.m_offset > offset)
			{
				node.m_parent = thisNode.m_inFileOffset;
				thisNode.m_leftChild = node.m_inFileOffset;
			}
			else if (thisNode.m_offset < offset)
			{
				node.m_parent = thisNode.m_inFileOffset;
				thisNode.m_rightChild = node.m_inFileOffset;
			}
			writeNode(node);
			writeNode(thisNode);
			writeHead();
			balance(node);
			while (!balanceNodesOffset.empty())
			{
				balance(readNode(balanceNodesOffset.top()));
				balanceNodesOffset.pop();
			}
			return node.m_inFileOffset;
		}
	}

	int UsedFile::_query(const size_t offset)
	{
		Node parent = readNode(m_head.rootNodeOffset);
		if (parent.m_inFileOffset == -1)
			return -1;
		Node nextNode = parent;
		while (nextNode.m_inFileOffset != -1)
		{
			parent = nextNode;
			if (parent.m_offset == offset)
			{
				return parent.m_inFileOffset;
			}
			else if (parent.m_offset > offset)
			{
				nextNode = readNode(parent.m_leftChild);
			}
			else if (parent.m_offset < offset)
			{
				nextNode = readNode(parent.m_rightChild);
			}
		}
		return -1;
	}

	void UsedFile::erase(Node node, int& offset)
	{
		Node maxLeftNode = readNode(node.m_leftChild);
		if (maxLeftNode.m_inFileOffset == -1)
		{
			if (node.m_rightChild != -1)
			{
				// node only has right child
				Node right = readNode(node.m_rightChild);

				if (node.m_inFileOffset == m_head.rootNodeOffset)
				{
					right.m_parent = -1;
					m_head.rootNodeOffset = right.m_inFileOffset;
				}
				else
				{
					right.m_parent = node.m_parent;
					Node parent = readNode(node.m_parent);
					if (parent.m_leftChild == node.m_inFileOffset)
						parent.m_leftChild = right.m_inFileOffset;
					else
						parent.m_rightChild = right.m_inFileOffset;
					writeNode(parent);
				}
				writeNode(right);
				offset = right.m_inFileOffset;
				goto End;
			}
			else
			{
				size_t left = node.m_leftChild;
				if (node.m_parent == -1)
				{
					if (left != -1)
					{
						Node leftNode = readNode(left);
						leftNode.m_parent = -1;
						writeNode(leftNode);
					}
					m_head.rootNodeOffset = left;
					writeHead();
					offset = m_head.rootNodeOffset;
				}
				else
				{
					// node is leaf node
					Node parent = readNode(node.m_parent);
					if (node.m_inFileOffset == parent.m_leftChild)
						parent.m_leftChild = -1;
					else
						parent.m_rightChild = -1;
					writeNode(parent);
					offset = parent.m_inFileOffset;
				}
				goto End;
			}
		}
		while (maxLeftNode.m_rightChild != -1)
		{
			// search max left child
			maxLeftNode = readNode(maxLeftNode.m_rightChild);
		}
		if (maxLeftNode.m_parent == node.m_inFileOffset)
		{
			// max_left_child's parent is node
			Node parent = readNode(node.m_parent);
			maxLeftNode.m_parent = parent.m_inFileOffset;
			if (node.m_inFileOffset == parent.m_leftChild)
				parent.m_leftChild = maxLeftNode.m_inFileOffset;
			else
				parent.m_rightChild = maxLeftNode.m_inFileOffset;
			maxLeftNode.m_rightChild = node.m_rightChild;
			if (maxLeftNode.m_rightChild != -1)
			{
				Node right = readNode(maxLeftNode.m_rightChild);
				right.m_parent = maxLeftNode.m_inFileOffset;
				writeNode(right);
			}
			writeNode(maxLeftNode);
			if (parent.m_inFileOffset != -1)
			{
				writeNode(parent);
				offset = parent.m_inFileOffset;
			}
			else
			{
				m_head.rootNodeOffset = maxLeftNode.m_inFileOffset;
				offset = m_head.rootNodeOffset;
			}
		}
		else
		{
			// max_left_child's parent is not node
			Node parent = readNode(maxLeftNode.m_parent);
			Node left = readNode(maxLeftNode.m_leftChild);
			left.m_parent = maxLeftNode.m_parent;
			parent.m_rightChild = left.m_inFileOffset;

			maxLeftNode.m_leftChild = node.m_leftChild;
			maxLeftNode.m_rightChild = node.m_rightChild;
			Node nodeLeft = readNode(node.m_leftChild);
			Node nodeRight = readNode(node.m_rightChild);
			if (nodeLeft.m_inFileOffset == parent.m_inFileOffset)
				nodeLeft = parent;
			nodeLeft.m_parent = maxLeftNode.m_inFileOffset;
			nodeRight.m_parent = maxLeftNode.m_inFileOffset;

			Node nodeParent = readNode(node.m_parent);
			maxLeftNode.m_parent = nodeParent.m_inFileOffset;
			if (node.m_inFileOffset == nodeParent.m_leftChild)
				nodeParent.m_leftChild = maxLeftNode.m_inFileOffset;
			else
				nodeParent.m_rightChild = maxLeftNode.m_inFileOffset;

			writeNode(maxLeftNode);
			writeNode(parent);
			if (left.m_inFileOffset != -1)
				writeNode(left);
			writeNode(nodeLeft);
			if (nodeRight.m_inFileOffset != -1)
				writeNode(nodeRight);
			if (nodeParent.m_inFileOffset != -1)
			{
				writeNode(nodeParent);
			}
			else
			{
				m_head.rootNodeOffset = maxLeftNode.m_inFileOffset;
			}
			offset = parent.m_inFileOffset;
		}
	End:
		EmptyNode* emptyNode = (EmptyNode*)&node;
		//memset(emptyNode, 0, Node::size());
		emptyNode->nextNodeOffset = m_head.firstEmptyNodeOffset;
		m_head.firstEmptyNodeOffset = node.m_inFileOffset;
		writeNode(node);
		m_head.nodeSize--;
		writeHead();
	}

	int UsedFile::lRotate(Node& node)
	{
		Node pNode = readNode(node.m_rightChild);
		node.m_rightChild = pNode.m_leftChild;

		if (pNode.m_leftChild != -1)
		{
			Node tmp = readNode(pNode.m_leftChild);
			tmp.m_parent = node.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_parent = node.m_parent;

		if (node.m_parent == -1)
		{
			m_head.rootNodeOffset = pNode.m_inFileOffset;
		}
		else if (node.m_inFileOffset == readNode(node.m_parent).m_leftChild)
		{
			Node tmp = readNode(node.m_parent);
			tmp.m_leftChild = pNode.m_inFileOffset;
			writeNode(tmp);
		}
		else
		{
			Node tmp = readNode(node.m_parent);
			tmp.m_rightChild = pNode.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_leftChild = node.m_inFileOffset;
		node.m_parent = pNode.m_inFileOffset;
		fixHeight(node);
		fixHeight(pNode);
		writeHead();
		writeNode(node);
		writeNode(pNode);
		return pNode.m_inFileOffset;
	}

	int UsedFile::rRotate(Node& node)
	{
		Node pNode = readNode(node.m_leftChild);
		node.m_leftChild = pNode.m_rightChild;

		if (pNode.m_rightChild != -1)
		{
			Node tmp = readNode(pNode.m_rightChild);
			tmp.m_parent = node.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_parent = node.m_parent;

		if (node.m_parent == -1)
		{
			m_head.rootNodeOffset = pNode.m_inFileOffset;
		}
		else if (node.m_inFileOffset == readNode(node.m_parent).m_rightChild)
		{
			Node tmp = readNode(node.m_parent);
			tmp.m_rightChild = pNode.m_inFileOffset;
			writeNode(tmp);
		}
		else
		{
			Node tmp = readNode(node.m_parent);
			tmp.m_leftChild = pNode.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_rightChild = node.m_inFileOffset;
		node.m_parent = pNode.m_inFileOffset;
		fixHeight(node);
		fixHeight(pNode);
		writeHead();
		writeNode(node);
		writeNode(pNode);
		return pNode.m_inFileOffset;
	}

	int UsedFile::balance(Node& p)
	{
		fixHeight(p);
		//assert(p.m_rightChild != -1 || p.m_leftChild != -1);
		Node pRight = readNode(p.m_rightChild);
		Node pLeft = readNode(p.m_leftChild);
		if (bfactor(p) == 2)
		{
			if (bfactor(pRight) < 0)
				p.m_rightChild = rRotate(pRight);
			return lRotate(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(pLeft) > 0)
				p.m_leftChild = lRotate(pLeft);
			return rRotate(p);
		}
		writeNode(p);
		writeNode(pLeft);
		writeNode(pRight);
		return p.m_parent; // balancing is not required}
	}

	bool UsedFile::insertNewNode(Node& node)
	{
		int ret;
		if (m_head.firstEmptyNodeOffset == -1)
		{
			stream.seekp(0, fstream::end);
			int offset = stream.tellp();
			ret = writeNode(offset, node);
			if (ret == -1)
				return false;
			m_head.nodeSize++;
			node.m_inFileOffset = offset;
			ret = writeHead();
		}
		else
		{
			int writeOffset = m_head.firstEmptyNodeOffset;
			EmptyNode emptyNode;
			ret = readNode(m_head.firstEmptyNodeOffset, emptyNode);
			if (ret == -1)
				return false;
			int nextEmptyNodeOffset = emptyNode.nextNodeOffset;
			ret = writeNode(writeOffset, node);
			node.m_inFileOffset = writeOffset;
			if (ret == -1)
				return false;
			m_head.nodeSize++;
			m_head.firstEmptyNodeOffset = nextEmptyNodeOffset;
			ret = writeHead();
			if (ret == -1)
				return false;
		}

		return true;
	}

	bool UsedFile::writeNode(const size_t offset, const Node& node)
	{
		if (offset == -1)
			return false;
		return _write(offset, &node, Node::size());
	}

	bool UsedFile::writeNode(const Node& node)
	{
		return writeNode(node.m_inFileOffset, node);
	}

	bool UsedFile::writeNode(const size_t offset, const EmptyNode& node)
	{
		return _write(offset, &node, Node::size());
	}

	UsedFile::Node UsedFile::readNode(const size_t offset)
	{
		Node node;
		readNode(offset, node);
		return node;
	}

	bool UsedFile::readNode(const size_t offset, Node& node)
	{
		if (offset == -1)
		{
			node.m_inFileOffset = -1;
			return false;
		}
		int ret = _read(offset, &node, Node::size());
		node.m_inFileOffset = offset;
		return ret;
	}

	bool UsedFile::readNode(const size_t offset, EmptyNode& node)
	{
		return _read(offset, &node, Node::size());
	}

	bool UsedFile::writeHead()
	{
		return _write(0, &m_head, sizeof(m_head));
	}

	bool UsedFile::readHead()
	{
		return _read(0, &m_head, sizeof(m_head));
	}

	int UsedFile::writeFirstNode(const size_t offset, const size_t length)
	{
		Node node(offset, length);
		int writeOffset = Node::size();
		writeNode(writeOffset, node);
		return writeOffset;
	}

	int UsedFile::bfactor(const Node& node)
	{
		return nodeHeight(node.m_rightChild) - nodeHeight(node.m_leftChild);
	}

	void UsedFile::fixHeight(Node& node)
	{
		const int lHeight = nodeHeight(node.m_leftChild);
		const int rHeight = nodeHeight(node.m_rightChild);
		int height = (lHeight > rHeight ? lHeight : rHeight) + 1;
		if (node.m_height == height)
			return;
		node.m_height = height;
		writeNode(node);
	}

	int UsedFile::nodeHeight(int m_offset)
	{
		int ret;
		if (m_offset == -1)
			return 0;
		return readNode(m_offset).m_height;
	}


	UnusedFile::UnusedFile(String fileName, const bool trunc)
		: FileBase(fileName, trunc), m_head{ 0,-1,-1,-1 }
	{
		if (trunc)
			writeHead();
		else
			readHead();
	}

	UnusedFile::Node::Node() : m_offset(0), m_length(0), m_leftChild_offset(-1), m_rightChild_offset(-1), m_parent_offset(-1),
		m_height_offset(0),
		m_leftChild_length(-1), m_rightChild_length(-1), m_parent_length(-1), m_height_length(0),
		m_inFileOffset(-1)
	{
	}

	UnusedFile::Node::Node(const size_t offset, const size_t length) : Node()
	{
		m_offset = offset;
		m_length = length;
	}

	size_t UnusedFile::Node::size()
	{
		return sizeof(Node) - sizeof(size_t);
	}

	bool UnusedFile::empty() const
	{
		return m_head.nodeSize == 0;
	}

	int UnusedFile::insert(const size_t offset, const size_t length)
	{
		return insert(Node(offset, length));
	}

	int UnusedFile::insert(Node& node)
	{
		const size_t offset = node.m_offset;
		const size_t length = node.m_length;
		if (length <= 0)
			throw "invalid length";
		if (!empty())
		{
			tryCombinenNode(node);
		}

		if (empty())
		{
			insertNewNode(node);
			m_head.rootNodeOffset_length = node.m_inFileOffset;
			m_head.rootNodeOffset_offset = node.m_inFileOffset;
			writeHead();
			return node.m_inFileOffset;
		}
		else
		{
			Node thisNode_length = readNode(m_head.rootNodeOffset_length);
			Node nextNode_length = thisNode_length;
			Stack<int> balanceNodesOffset_length;
			while (nextNode_length.m_length != -1)
			{
				thisNode_length = nextNode_length;
				balanceNodesOffset_length.push(thisNode_length.m_inFileOffset);
				if (thisNode_length.m_length == length)
				{
					if (thisNode_length.m_leftChild_length != -1)
						nextNode_length = readNode(thisNode_length.m_leftChild_length);
					else
						nextNode_length.m_length = -1; //用length=-1来标记结束
				}
				else if (thisNode_length.m_length > length)
				{
					if (thisNode_length.m_leftChild_length != -1)
						nextNode_length = readNode(thisNode_length.m_leftChild_length);
					else
						nextNode_length.m_length = -1; //用length=-1来标记结束
				}
				else if (thisNode_length.m_length < length)
				{
					if (thisNode_length.m_rightChild_length != -1)
						nextNode_length = readNode(thisNode_length.m_rightChild_length);
					else
						nextNode_length.m_length = -1; //用length=-1来标记结束
				}
			}
			Node thisNode_offset = readNode(m_head.rootNodeOffset_offset);
			Node nextNode_offset = thisNode_offset;
			Stack<int> balanceNodesOffset_offset;
			while (nextNode_offset.m_offset != -1)
			{
				thisNode_offset = nextNode_offset;
				balanceNodesOffset_offset.push(thisNode_offset.m_inFileOffset);
				if (thisNode_offset.m_offset == offset)
				{
					if (thisNode_offset.m_leftChild_offset != -1)
						nextNode_offset = readNode(thisNode_offset.m_leftChild_offset);
					else
						nextNode_offset.m_offset = -1; //用offset=-1来标记结束
				}
				else if (thisNode_offset.m_offset > offset)
				{
					if (thisNode_offset.m_leftChild_offset != -1)
						nextNode_offset = readNode(thisNode_offset.m_leftChild_offset);
					else
						nextNode_offset.m_offset = -1; //用offset=-1来标记结束
				}
				else if (thisNode_offset.m_offset < offset)
				{
					if (thisNode_offset.m_rightChild_offset != -1)
						nextNode_offset = readNode(thisNode_offset.m_rightChild_offset);
					else
						nextNode_offset.m_offset = -1; //用offset=-1来标记结束
				}
			}
			insertNewNode(node);
			if (thisNode_length.m_length >= length)
			{
				node.m_parent_length = thisNode_length.m_inFileOffset;
				thisNode_length.m_leftChild_length = node.m_inFileOffset;
			}
			else if (thisNode_length.m_length < length)
			{
				node.m_parent_length = thisNode_length.m_inFileOffset;
				thisNode_length.m_rightChild_length = node.m_inFileOffset;
			}
			if (thisNode_length.m_inFileOffset == thisNode_offset.m_inFileOffset)
			{
				thisNode_offset = thisNode_length;
			}
			if (thisNode_offset.m_offset >= offset)
			{
				node.m_parent_offset = thisNode_offset.m_inFileOffset;
				thisNode_offset.m_leftChild_offset = node.m_inFileOffset;
			}
			else if (thisNode_offset.m_offset < offset)
			{
				node.m_parent_offset = thisNode_offset.m_inFileOffset;
				thisNode_offset.m_rightChild_offset = node.m_inFileOffset;
			}
			writeNode(node);
			writeNode(thisNode_length);
			writeNode(thisNode_offset);
			writeHead();
			balance_length(node);
			balance_offset(node);
			while (!balanceNodesOffset_length.empty())
			{
				balance_length(readNode(balanceNodesOffset_length.top()));
				balanceNodesOffset_length.pop();
			}
			while (!balanceNodesOffset_offset.empty())
			{
				balance_offset(readNode(balanceNodesOffset_offset.top()));
				balanceNodesOffset_offset.pop();
			}
			return node.m_inFileOffset;
		}
	}

	void UnusedFile::tryCombinenNode(Node& node)
	{
		int pre = 0;
		int next = 0;
		query_offset(node, pre, next);
		if (pre != -1)
		{
			Node preNode = readNode(pre);
			node.m_offset = preNode.m_offset;
			node.m_length += preNode.m_length;
			int tmp1, tmp2;
			erase(preNode, tmp1, tmp2);
		}
		if (next != -1)
		{
			Node nextNode = readNode(next);
			node.m_length += nextNode.m_length;
			int tmp1, tmp2;
			erase(nextNode, tmp1, tmp2);
		}
	}

	bool UnusedFile::getEnoughPlace(const size_t length, size_t& rOffset, size_t& rLength)
	{
		int offset = query_length(length);
		if (offset == -1)
			return false;
		Node node = readNode(offset);
		rOffset = node.m_offset;
		rLength = node.m_length;
		int enbalanceNodeOffset_length = 0;
		int enbalanceNodeOffset_offset = 0;
		erase(node, enbalanceNodeOffset_length, enbalanceNodeOffset_offset);
		Node enbalanceNode_length = readNode(enbalanceNodeOffset_length);
		while (enbalanceNode_length.m_inFileOffset != -1)
		{
			enbalanceNodeOffset_length = balance_length(enbalanceNode_length);
			enbalanceNode_length = readNode(enbalanceNodeOffset_length);
		}
		Node enbalanceNode_offset = readNode(enbalanceNodeOffset_offset);
		while (enbalanceNode_offset.m_inFileOffset != -1)
		{
			enbalanceNodeOffset_offset = balance_offset(enbalanceNode_offset);
			enbalanceNode_offset = readNode(enbalanceNodeOffset_offset);
		}
		return true;
	}

	int UnusedFile::query_length(const int length)
	{
		Node parent = readNode(m_head.rootNodeOffset_length);
		if (parent.m_inFileOffset == -1)
			return -1;
		Node nextNode = parent;
		while (nextNode.m_inFileOffset != -1)
		{
			parent = nextNode;
			if (parent.m_length >= length)
			{
				return parent.m_inFileOffset;
			}
			else if (parent.m_length < length)
			{
				nextNode = readNode(parent.m_rightChild_length);
			}
		}
		return -1;
	}

	void UnusedFile::query_offset(const Node& node, int& preOffset, int& nextOffset)
	{
		preOffset = -1;
		nextOffset = -1;
		Node parent = readNode(m_head.rootNodeOffset_offset);
		if (parent.m_inFileOffset == -1)
			return;
		Node nextNode = parent;
		while (nextNode.m_inFileOffset != -1)
		{
			parent = nextNode;
			if (parent.m_offset + parent.m_length == node.m_offset)
			{
				preOffset = parent.m_inFileOffset;
				break;
			}
			else if (parent.m_offset + parent.m_length > node.m_offset)
			{
				nextNode = readNode(parent.m_leftChild_offset);
			}
			else if (parent.m_offset + parent.m_length < node.m_offset)
			{
				nextNode = readNode(parent.m_rightChild_offset);
			}
		}

		parent = readNode(m_head.rootNodeOffset_offset);
		nextNode = parent;
		while (nextNode.m_inFileOffset != -1)
		{
			parent = nextNode;
			if (parent.m_offset == node.m_offset + node.m_length)
			{
				nextOffset = parent.m_inFileOffset;
				break;
			}
			else if (parent.m_offset > node.m_offset + node.m_length)
			{
				nextNode = readNode(parent.m_leftChild_offset);
			}
			else if (parent.m_offset < node.m_offset + node.m_length)
			{
				nextNode = readNode(parent.m_rightChild_offset);
			}
		}
	}

	void UnusedFile::erase(Node node, int& offset_length, int& offset_offset)
	{
		Node maxLeftNode_length = readNode(node.m_leftChild_length);
		if (maxLeftNode_length.m_inFileOffset == -1)
		{
			if (node.m_rightChild_length != -1)
			{
				// node only has right child
				Node right = readNode(node.m_rightChild_length);

				if (node.m_inFileOffset == m_head.rootNodeOffset_length)
				{
					right.m_parent_length = -1;
					m_head.rootNodeOffset_length = right.m_inFileOffset;
				}
				else
				{
					right.m_parent_length = node.m_parent_length;
					Node parent = readNode(node.m_parent_length);
					if (parent.m_leftChild_length == node.m_inFileOffset)
						parent.m_leftChild_length = right.m_inFileOffset;
					else
						parent.m_rightChild_length = right.m_inFileOffset;
					writeNode(parent);
				}
				writeNode(right);
				offset_length = right.m_inFileOffset;
				goto OffsetBegin;
			}
			else
			{
				size_t left = node.m_leftChild_length;
				if (node.m_parent_length == -1)
				{
					if (left != -1)
					{
						Node leftNode = readNode(left);
						leftNode.m_parent_length = -1;
						writeNode(leftNode);
					}
					m_head.rootNodeOffset_length = left;
					writeHead();
					offset_length = m_head.rootNodeOffset_length;
				}
				else
				{
					// node is leaf node
					Node parent = readNode(node.m_parent_length);
					if (node.m_inFileOffset == parent.m_leftChild_length)
						parent.m_leftChild_length = -1;
					else
						parent.m_rightChild_length = -1;

					writeNode(parent);
					offset_length = parent.m_inFileOffset;
				}
				goto OffsetBegin;
			}
		}
		while (maxLeftNode_length.m_rightChild_length != -1)
		{
			// search max left child
			maxLeftNode_length = readNode(maxLeftNode_length.m_rightChild_length);
		}
		if (maxLeftNode_length.m_parent_length == node.m_inFileOffset)
		{
			// max_left_child's parent is node
			Node parent = readNode(node.m_parent_length);
			maxLeftNode_length.m_parent_length = parent.m_inFileOffset;
			if (node.m_inFileOffset == parent.m_leftChild_length)
				parent.m_leftChild_length = maxLeftNode_length.m_inFileOffset;
			else
				parent.m_rightChild_length = maxLeftNode_length.m_inFileOffset;
			maxLeftNode_length.m_rightChild_length = node.m_rightChild_length;
			if (maxLeftNode_length.m_rightChild_length != -1)
			{
				Node right = readNode(maxLeftNode_length.m_rightChild_length);
				right.m_parent_length = maxLeftNode_length.m_inFileOffset;
				writeNode(right);
			}
			writeNode(maxLeftNode_length);
			if (parent.m_inFileOffset != -1)
			{
				writeNode(parent);
				offset_length = parent.m_inFileOffset;
			}
			else
			{
				m_head.rootNodeOffset_length = maxLeftNode_length.m_inFileOffset;
				offset_length = m_head.rootNodeOffset_length;
			}
		}
		else
		{
			// max_left_child's parent is not node
			Node parent = readNode(maxLeftNode_length.m_parent_length);
			Node left = readNode(maxLeftNode_length.m_leftChild_length);
			left.m_parent_length = maxLeftNode_length.m_parent_length;
			parent.m_rightChild_length = left.m_inFileOffset;

			maxLeftNode_length.m_leftChild_length = node.m_leftChild_length;
			maxLeftNode_length.m_rightChild_length = node.m_rightChild_length;
			Node nodeLeft = readNode(node.m_leftChild_length);
			Node nodeRight = readNode(node.m_rightChild_length);
			if (nodeLeft.m_inFileOffset == parent.m_inFileOffset)
				nodeLeft = parent;
			nodeLeft.m_parent_length = maxLeftNode_length.m_inFileOffset;
			nodeRight.m_parent_length = maxLeftNode_length.m_inFileOffset;

			Node nodeParent = readNode(node.m_parent_length);
			maxLeftNode_length.m_parent_length = nodeParent.m_inFileOffset;
			if (node.m_inFileOffset == nodeParent.m_leftChild_length)
				nodeParent.m_leftChild_length = maxLeftNode_length.m_inFileOffset;
			else
				nodeParent.m_rightChild_length = maxLeftNode_length.m_inFileOffset;

			writeNode(maxLeftNode_length);
			writeNode(parent);
			if (left.m_inFileOffset != -1)
				writeNode(left);
			writeNode(nodeLeft);
			if (nodeRight.m_inFileOffset != -1)
				writeNode(nodeRight);
			if (nodeParent.m_inFileOffset != -1)
			{
				writeNode(nodeParent);
			}
			else
			{
				m_head.rootNodeOffset_length = maxLeftNode_length.m_inFileOffset;
			}
			offset_length = parent.m_inFileOffset;
		}
	OffsetBegin:
		Node maxLeftNode_offset = readNode(node.m_leftChild_offset);
		if (maxLeftNode_offset.m_inFileOffset == -1)
		{
			if (node.m_rightChild_offset != -1)
			{
				// node only has right child
				Node right = readNode(node.m_rightChild_offset);

				if (node.m_inFileOffset == m_head.rootNodeOffset_offset)
				{
					right.m_parent_offset = -1;
					m_head.rootNodeOffset_offset = right.m_inFileOffset;
				}
				else
				{
					right.m_parent_offset = node.m_parent_offset;
					Node parent = readNode(node.m_parent_offset);
					if (parent.m_leftChild_offset == node.m_inFileOffset)
						parent.m_leftChild_offset = right.m_inFileOffset;
					else
						parent.m_rightChild_offset = right.m_inFileOffset;
					writeNode(parent);
				}
				writeNode(right);
				offset_offset = right.m_inFileOffset;
				goto End;
			}
			else
			{
				size_t left = node.m_leftChild_offset;
				if (node.m_parent_offset == -1)
				{
					if (left != -1)
					{
						Node leftNode = readNode(left);
						leftNode.m_parent_offset = -1;
						writeNode(leftNode);
					}
					m_head.rootNodeOffset_offset = left;
					writeHead();
					offset_offset = m_head.rootNodeOffset_offset;
				}
				else
				{
					// node is leaf node
					Node parent = readNode(node.m_parent_offset);
					if (node.m_inFileOffset == parent.m_leftChild_offset)
						parent.m_leftChild_offset = -1;
					else
						parent.m_rightChild_offset = -1;
					writeNode(parent);
					offset_offset = parent.m_inFileOffset;
				}
				goto End;
			}
		}
		while (maxLeftNode_offset.m_rightChild_offset != -1)
		{
			// search max left child
			maxLeftNode_offset = readNode(maxLeftNode_offset.m_rightChild_offset);
		}
		if (maxLeftNode_offset.m_parent_offset == node.m_inFileOffset)
		{
			// max_left_child's parent is node
			Node parent = readNode(node.m_parent_offset);
			maxLeftNode_offset.m_parent_offset = parent.m_inFileOffset;
			if (node.m_inFileOffset == parent.m_leftChild_offset)
				parent.m_leftChild_offset = maxLeftNode_offset.m_inFileOffset;
			else
				parent.m_rightChild_offset = maxLeftNode_offset.m_inFileOffset;
			maxLeftNode_offset.m_rightChild_offset = node.m_rightChild_offset;
			if (maxLeftNode_offset.m_rightChild_offset != -1)
			{
				Node right = readNode(maxLeftNode_offset.m_rightChild_offset);
				right.m_parent_offset = maxLeftNode_offset.m_inFileOffset;
				writeNode(right);
			}
			writeNode(maxLeftNode_offset);
			if (parent.m_inFileOffset != -1)
			{
				writeNode(parent);
				offset_offset = parent.m_inFileOffset;
			}
			else
			{
				m_head.rootNodeOffset_offset = maxLeftNode_offset.m_inFileOffset;
				offset_offset = m_head.rootNodeOffset_offset;
			}
		}
		else
		{
			// max_left_child's parent is not node
			Node parent = readNode(maxLeftNode_offset.m_parent_offset);
			Node left = readNode(maxLeftNode_offset.m_leftChild_offset);
			left.m_parent_offset = maxLeftNode_offset.m_parent_offset;
			parent.m_rightChild_offset = left.m_inFileOffset;

			maxLeftNode_offset.m_leftChild_offset = node.m_leftChild_offset;
			maxLeftNode_offset.m_rightChild_offset = node.m_rightChild_offset;
			Node nodeLeft = readNode(node.m_leftChild_offset);
			Node nodeRight = readNode(node.m_rightChild_offset);
			if (nodeLeft.m_inFileOffset == parent.m_inFileOffset)
				nodeLeft = parent;
			nodeLeft.m_parent_offset = maxLeftNode_offset.m_inFileOffset;
			nodeRight.m_parent_offset = maxLeftNode_offset.m_inFileOffset;

			Node nodeParent = readNode(node.m_parent_offset);
			maxLeftNode_offset.m_parent_offset = nodeParent.m_inFileOffset;
			if (node.m_inFileOffset == nodeParent.m_leftChild_offset)
				nodeParent.m_leftChild_offset = maxLeftNode_offset.m_inFileOffset;
			else
				nodeParent.m_rightChild_offset = maxLeftNode_offset.m_inFileOffset;

			writeNode(maxLeftNode_offset);
			writeNode(parent);
			if (left.m_inFileOffset != -1)
				writeNode(left);
			writeNode(nodeLeft);
			if (nodeRight.m_inFileOffset != -1)
				writeNode(nodeRight);
			if (nodeParent.m_inFileOffset != -1)
			{
				writeNode(nodeParent);
			}
			else
			{
				m_head.rootNodeOffset_offset = maxLeftNode_offset.m_inFileOffset;
			}
			offset_offset = parent.m_inFileOffset;
		}
	End:
		EmptyNode* emptyNode = (EmptyNode*)&node;
		memset(emptyNode, 0, Node::size());
		emptyNode->nextNodeOffset = m_head.firstEmptyNodeOffset;
		m_head.firstEmptyNodeOffset = node.m_inFileOffset;
		writeNode(node);
		m_head.nodeSize--;
		writeHead();
		return;
	}

	int UnusedFile::lRotate_length(Node& node)
	{
		Node pNode = readNode(node.m_rightChild_length);
		node.m_rightChild_length = pNode.m_leftChild_length;

		if (pNode.m_leftChild_length != -1)
		{
			Node tmp = readNode(pNode.m_leftChild_length);
			tmp.m_parent_length = node.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_parent_length = node.m_parent_length;

		if (node.m_parent_length == -1)
		{
			m_head.rootNodeOffset_length = pNode.m_inFileOffset;
		}
		else if (node.m_inFileOffset == readNode(pNode.m_parent_length).m_leftChild_length)
		{
			Node tmp = readNode(pNode.m_parent_length);
			tmp.m_leftChild_length = node.m_inFileOffset;
			writeNode(tmp);
		}
		else
		{
			Node tmp = readNode(pNode.m_parent_length);
			tmp.m_rightChild_length = node.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_leftChild_length = node.m_inFileOffset;
		node.m_parent_length = pNode.m_inFileOffset;
		fixHeight_length(node);
		fixHeight_length(pNode);
		writeHead();
		writeNode(node);
		writeNode(pNode);
		return pNode.m_inFileOffset;
	}

	int UnusedFile::rRotata_length(Node& node)
	{
		Node pNode = readNode(node.m_leftChild_length);
		node.m_leftChild_length = pNode.m_rightChild_length;

		if (pNode.m_rightChild_length != -1)
		{
			Node tmp = readNode(pNode.m_rightChild_length);
			tmp.m_parent_length = node.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_parent_length = node.m_parent_length;

		if (node.m_parent_length == -1)
		{
			m_head.rootNodeOffset_length = pNode.m_inFileOffset;
		}
		else if (node.m_inFileOffset == readNode(pNode.m_parent_length).m_rightChild_length)
		{
			Node tmp = readNode(pNode.m_parent_length);
			tmp.m_rightChild_length = node.m_inFileOffset;
			writeNode(tmp);
		}
		else
		{
			Node tmp = readNode(pNode.m_parent_length);
			tmp.m_leftChild_length = node.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_rightChild_length = node.m_inFileOffset;
		node.m_parent_length = pNode.m_inFileOffset;
		fixHeight_length(node);
		fixHeight_length(pNode);
		writeHead();
		writeNode(node);
		writeNode(pNode);
		return pNode.m_inFileOffset;
	}

	int UnusedFile::balance_length(Node& p)
	{
		fixHeight_length(p);
		Node pRight = readNode(p.m_rightChild_length);
		Node pLeft = readNode(p.m_leftChild_length);
		if (bfactor_length(p) == 2)
		{
			if (bfactor_length(pRight) < 0)
				p.m_rightChild_length = rRotata_length(pRight);
			return lRotate_length(p);
		}
		if (bfactor_length(p) == -2)
		{
			if (bfactor_length(pLeft) > 0)
				p.m_leftChild_length = lRotate_length(pLeft);
			return rRotata_length(p);
		}
		writeNode(p);
		writeNode(pLeft);
		writeNode(pRight);
		return p.m_parent_length; // balancing is not required
	}

	int UnusedFile::lRotate_offset(Node& node)
	{
		Node pNode = readNode(node.m_rightChild_offset);
		node.m_rightChild_offset = pNode.m_leftChild_offset;

		if (pNode.m_leftChild_offset != -1)
		{
			Node tmp = readNode(pNode.m_leftChild_offset);
			tmp.m_parent_offset = node.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_parent_offset = node.m_parent_offset;

		if (node.m_parent_offset == -1)
		{
			m_head.rootNodeOffset_offset = pNode.m_inFileOffset;
		}
		else if (node.m_inFileOffset == readNode(node.m_parent_offset).m_leftChild_offset)
		{
			Node tmp = readNode(node.m_parent_offset);
			tmp.m_leftChild_offset = pNode.m_inFileOffset;
			writeNode(tmp);
		}
		else
		{
			Node tmp = readNode(node.m_parent_offset);
			tmp.m_rightChild_offset = pNode.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_leftChild_offset = node.m_inFileOffset;
		node.m_parent_offset = pNode.m_inFileOffset;
		fixHeight_offset(node);
		fixHeight_offset(pNode);
		writeHead();
		writeNode(node);
		writeNode(pNode);
		return pNode.m_inFileOffset;
	}

	int UnusedFile::rRotata_offset(Node& node)
	{
		Node pNode = readNode(node.m_leftChild_offset);
		node.m_leftChild_offset = pNode.m_rightChild_offset;

		if (pNode.m_rightChild_offset != -1)
		{
			Node tmp = readNode(pNode.m_rightChild_offset);
			tmp.m_parent_offset = node.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_parent_offset = node.m_parent_offset;

		if (node.m_parent_offset == -1)
		{
			m_head.rootNodeOffset_offset = pNode.m_inFileOffset;
		}
		else if (node.m_inFileOffset == readNode(node.m_parent_offset).m_rightChild_offset)
		{
			Node tmp = readNode(node.m_parent_offset);
			tmp.m_rightChild_offset = pNode.m_inFileOffset;
			writeNode(tmp);
		}
		else
		{
			Node tmp = readNode(node.m_parent_offset);
			tmp.m_leftChild_offset = pNode.m_inFileOffset;
			writeNode(tmp);
		}

		pNode.m_rightChild_offset = node.m_inFileOffset;
		node.m_parent_offset = pNode.m_inFileOffset;
		fixHeight_offset(node);
		fixHeight_offset(pNode);
		writeHead();
		writeNode(node);
		writeNode(pNode);
		return pNode.m_inFileOffset;
	}

	int UnusedFile::balance_offset(Node& p)
	{
		fixHeight_offset(p);
		//assert(p.m_rightChild_offset != -1 || p.m_leftChild_offset != -1);
		Node pRight = readNode(p.m_rightChild_offset);
		Node pLeft = readNode(p.m_leftChild_offset);
		if (bfactor_offset(p) == 2)
		{
			if (bfactor_offset(pRight) < 0)
				p.m_rightChild_offset = rRotata_offset(pRight);
			return lRotate_offset(p);
		}
		if (bfactor_offset(p) == -2)
		{
			if (bfactor_offset(pLeft) > 0)
				p.m_leftChild_offset = lRotate_offset(pLeft);
			return rRotata_offset(p);
		}
		writeNode(p);
		writeNode(pLeft);
		writeNode(pRight);
		return p.m_parent_offset; // balancing is not required
	}

	bool UnusedFile::insertNewNode(Node& node)
	{
		int ret;
		if (m_head.firstEmptyNodeOffset == -1)
		{
			stream.seekp(0, fstream::end);
			int offset = stream.tellp();
			ret = writeNode(offset, node);
			if (ret == -1)
				return false;
			m_head.nodeSize++;
			node.m_inFileOffset = offset;
			ret = writeHead();
		}
		else
		{
			int writeOffset = m_head.firstEmptyNodeOffset;
			EmptyNode emptyNode;
			ret = readNode(m_head.firstEmptyNodeOffset, emptyNode);
			if (ret == -1)
				return false;
			int nextEmptyNodeOffset = emptyNode.nextNodeOffset;
			ret = writeNode(writeOffset, node);
			node.m_inFileOffset = writeOffset;
			if (ret == -1)
				return false;
			m_head.nodeSize++;
			m_head.firstEmptyNodeOffset = nextEmptyNodeOffset;
			ret = writeHead();
			if (ret == -1)
				return false;
		}

		return true;
	}

	bool UnusedFile::writeNode(const size_t offset, const Node& node)
	{
		if (offset == -1)
			return false;
		return _write(offset, &node, Node::size());
	}

	bool UnusedFile::writeNode(const Node& node)
	{
		return writeNode(node.m_inFileOffset, node);
	}

	bool UnusedFile::writeNode(const size_t offset, const EmptyNode& node)
	{
		return _write(offset, &node, Node::size());
	}

	UnusedFile::Node UnusedFile::readNode(const size_t offset)
	{
		Node node;
		readNode(offset, node);
		return node;
	}

	bool UnusedFile::readNode(const size_t offset, Node& node)
	{
		if (offset == -1)
		{
			node.m_inFileOffset = -1;
			return false;
		}
		int ret = _read(offset, &node, Node::size());
		node.m_inFileOffset = offset;
		return ret;
	}

	bool UnusedFile::readNode(const size_t offset, EmptyNode& node)
	{
		return _read(offset, &node, Node::size());
	}

	bool UnusedFile::writeHead()
	{
		return _write(0, &m_head, sizeof(m_head));
	}

	bool UnusedFile::readHead()
	{
		return _read(0, &m_head, sizeof(m_head));
	}

	int UnusedFile::writeFirstNode(const size_t offset, const size_t length)
	{
		Node node(offset, length);
		int writeOffset = Node::size();
		writeNode(writeOffset, node);
		return writeOffset;
	}

	int UnusedFile::bfactor_offset(const Node& node)
	{
		return nodeHeight_offset(node.m_rightChild_offset) - nodeHeight_offset(node.m_leftChild_offset);
	}

	int UnusedFile::bfactor_length(const Node& node)
	{
		return nodeHeight_length(node.m_rightChild_length) - nodeHeight_length(node.m_leftChild_length);
	}

	void UnusedFile::fixHeight_offset(Node& node)
	{
		const int lHeight = nodeHeight_offset(node.m_leftChild_offset);
		const int rHeight = nodeHeight_offset(node.m_rightChild_offset);
		int height = (lHeight > rHeight ? lHeight : rHeight) + 1;
		if (node.m_height_offset == height)
			return;
		node.m_height_offset = height;
		_write(node.m_inFileOffset + int(Node::OFFSET::m_height_offset) * 4, &height, sizeof(int));
	}

	void UnusedFile::fixHeight_length(Node& node)
	{
		const int lHeight = nodeHeight_length(node.m_leftChild_length);
		const int rHeight = nodeHeight_length(node.m_rightChild_length);
		int height = (lHeight > rHeight ? lHeight : rHeight) + 1;
		if (node.m_height_length == height)
			return;
		node.m_height_length = height;
		_write(node.m_inFileOffset + int(Node::OFFSET::m_height_length) * 4, &height, sizeof(int));
	}

	int UnusedFile::nodeHeight_offset(int m_offset)
	{
		int ret;
		if (m_offset == -1)
			return 0;
		_read(m_offset + int(Node::OFFSET::m_height_offset) * 4, &ret, sizeof(int));
		return ret;
	}

	int UnusedFile::nodeHeight_length(int m_offset)
	{
		int ret;
		if (m_offset == -1)
			return 0;
		_read(m_offset + int(Node::OFFSET::m_height_length) * 4, &ret, sizeof(int));
		return ret;
	}

	File::File(String fileName, const bool trunc)
		: FileBase(fileName, trunc),
		m_used_file(String("used") + fileName.c_str(), trunc),
		m_unused_file(String("unused") + fileName.c_str(), trunc)
	{

	}

	bool File::insert(void* data, const size_t size, size_t& offset)
	{
		size_t length = 0;
		if (m_unused_file.getEnoughPlace(size, offset, length))
		{
			if (length > size)
				m_unused_file.insert(offset + size, length - size);
			m_used_file.insert(offset, size);
			if (!_write(offset, data, size))
				return false;
		}
		else
		{
			if (!append(data, size, offset))
				return false;
			m_used_file.insert(offset, size);
		}
		return true;
	}

	bool File::erase(size_t offset)
	{
		int length = m_used_file.erase(offset);
		if (length > 0)
		{
			m_unused_file.insert(offset, length);
			return true;
		}
		return false;
	}

	bool File::modify(void* data, const size_t offset, const size_t size, size_t& newOffset)
	{
		if (erase(offset))
		{
			if (insert(data, size, newOffset))
				return true;
		}
		return false;
	}

	bool File::query(size_t offset, size_t& size)
	{
		if (m_used_file.query(offset, size))
		{
			return true;
		}
		return false;
	}

	bool File::read(void* data, const size_t offset, const size_t length, size_t& readSize)
	{
		if (m_used_file.query(offset, readSize))
		{
			if (length < readSize)
				_read(offset, data, length);
			else
				_read(offset, data, readSize);

			return true;
		}
		return false;
	}

	bool File::write(void* data, const size_t offset, const size_t size)
	{
		size_t length = 0;
		if (query(offset, length))
		{
			if (length < size)
			{
				return false;
			}
			else
			{
				_write(offset, data, size);
				return true;
			}
		}
		return false;
	}
}
