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
		friend class FileTest::FileTest;
		private:
		struct Node
		{
			size_t m_offset;
			size_t m_length;
			int childNum;
		};
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


		int getEnoughPlace(const size_t length, size_t& rOffset, size_t& rLength);


		private:

		int insert(Node& node);

		void tryCombinenNode(Node& node);

		int query_length(const int length);

		void query_offset(const Node& node, int& preOffset, int& nextOffset);

		//只删除，不平衡
		void erase(Node node, int& offset_length, int& offset_offset);

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
