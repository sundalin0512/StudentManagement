#pragma once
#include <fstream>
#include "../MyString/MyString.h"
#include "../DataStruct/Stack.hpp"
#include <cassert>
using std::fstream;

namespace FileTest
{
	class FileTest;
}

namespace Sdalin
{
	class FileBase
	{
		public:
		FileBase();

		FileBase(const String fileName, const bool trunc = false);

		FileBase(const FileBase&) = delete;

		virtual ~FileBase();

		bool open(String fileName, const bool trunc = false);

		bool write(const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite);

		bool read(const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead);


		protected:
		fstream stream;
	};


	class UsedFile : public FileBase
	{
		public:
		UsedFile(String fileName, const bool trunc = false);

		UsedFile(const UsedFile&) = delete;
	};


	class UnusedFile : public FileBase
	{
		public:
		friend class FileTest::FileTest;
		private:
		struct Node
		{
			size_t m_offset;
			size_t m_length;
			int    m_leftChild_offset;
			int    m_rightChild_offset;
			int    m_parent_offset;
			int    m_height_offset;
			int    m_leftChild_length;
			int    m_rightChild_length;
			int    m_parent_length;
			int    m_height_length;
			int    m_inFileOffset;
			enum class OFFSET
			{
				m_offset = 0,
				m_length,
				m_leftChild_offset,
				m_rightChild_offset,
				m_parent_offset,
				m_height_offset,
				m_leftChild_length,
				m_rightChild_length,
				m_parent_length,
				m_height_length
			};

			Node();

			Node(const size_t offset, const size_t length);

			static size_t size();
		};
		struct FileHead
		{
			int nodeSize;
			int firstEmptyNodeOffset;
			int rootNodeOffset_length;
			int rootNodeOffset_offset;
			int reserve[sizeof(Node) / sizeof(int) - 1 - 4];
		};
		struct EmptyNode
		{
			int nextNodeOffset;
			int reserve[sizeof(Node) / sizeof(int) - 1 - 1];
		};

		public:
		UnusedFile(String fileName, const bool trunc = false);

		UnusedFile(const UnusedFile&) = delete;

		bool empty() const;

		//返回插入到文件中的起始位置
		int insert(const size_t offset, const size_t length);

		int insert(Node& node);

		int getEnoughPlace(const size_t length, size_t& rOffset, size_t& rLength)
		{
			int offset = query(length);
			if (offset == -1)
				return -1;
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
			return 0;
		}

		int query(const int data)
		{
			Node parent = readNode(m_head.rootNodeOffset_length);
			if (parent.m_inFileOffset == -1)
				return -1;
			Node nextNode = parent;
			while (nextNode.m_inFileOffset != -1)
			{
				parent = nextNode;
				if (parent.m_length >= data)
				{
					return parent.m_inFileOffset;
				}
				else if (parent.m_length < data)
				{
					nextNode = readNode(parent.m_rightChild_length);
				}
			}
			return -1;
		}

		private:

		//只删除，不平衡
		void erase(Node node, int& offset_length, int& offset_offset)
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
					// node is leaf node
					Node parent = readNode(node.m_parent_length);
					if (node.m_inFileOffset == parent.m_leftChild_length)
						parent.m_leftChild_length = -1;
					else
						parent.m_rightChild_length = -1;

					writeNode(parent);
					offset_length = parent.m_inFileOffset;
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
					// node is leaf node
					Node parent = readNode(node.m_parent_offset);
					if (node.m_inFileOffset == parent.m_leftChild_offset)
						parent.m_leftChild_offset = -1;
					else
						parent.m_rightChild_offset = -1;
					writeNode(parent);
					offset_offset = parent.m_inFileOffset;
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

		int lRotate_length(Node& node);

		int rRotata_length(Node& node);

		int balance_length(Node& p);

		int lRotate_offset(Node& node);

		int rRotata_offset(Node& node);

		int balance_offset(Node& p);

		bool insertNewNode(Node& node);

		bool writeNode(const size_t offset, const Node& node);

		bool writeNode(const Node& node);

		bool writeNode(const size_t offset, const EmptyNode& node);

		Node readNode(const size_t offset);

		bool readNode(const size_t offset, Node& node);

		bool readNode(const size_t offset, EmptyNode& node);

		bool writeHead();

		bool readHead();

		/// <summary>
		/// 写入第一个节点
		/// </summary>
		/// <param name="offset">在File中的偏移</param>
		/// <param name="length">在File中的长度</param>
		/// <returns>写入的文件偏移,<c>-1</c>失败</returns>
		int writeFirstNode(const size_t offset, const size_t length);

		int bfactor_offset(const Node& node);

		int bfactor_length(const Node& node);

		void fixHeight_offset(Node& node);

		void fixHeight_length(Node& node);

		int nodeHeight_offset(int m_offset);

		int nodeHeight_length(int m_offset);

		private:
		FileHead m_head;
		String ErrorCode;

	};


	class File : public FileBase
	{
		public:
		File() = delete;

		File(String fileName, const bool trunc = false);
		private:
		UsedFile m_used_file;
		UnusedFile m_unused_file;
	};
}
