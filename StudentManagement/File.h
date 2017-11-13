#pragma once
#include <fstream>
#include "../MyString/MyString.h"

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

		bool _write(const size_t offsetFromFileStart, const void *buffer, const size_t bytesToWrite);

		bool _read(const size_t offsetFromFileStart, void *buffer, const size_t bytesToRead);

		bool append(const void* buffer, const size_t bytesToWrite, size_t& offsetWrite);
		protected:
		fstream stream;
	};


	class UsedFile : public FileBase
	{
		public:
		friend class FileTest::FileTest;
		private:
		struct Node
		{
			size_t m_offset;
			size_t m_length;
			int m_leftChild;
			int m_rightChild;
			int m_parent;
			int m_height;

			int m_inFileOffset;
			Node();

			Node(const size_t offset, const size_t length);

			static size_t size();
		};
		struct FileHead
		{
			int nodeSize;
			int firstEmptyNodeOffset;
			int rootNodeOffset;
			int reserve[sizeof(Node) / sizeof(int) - 1 - 3];
		};
		struct EmptyNode
		{
			int nextNodeOffset;
			int reserve[sizeof(Node) / sizeof(int) - 1 - 1];
		};
		public:
		UsedFile(String fileName, const bool trunc);

		UsedFile(const UsedFile&) = delete;

		bool empty() const;

		//���ز��뵽�ļ��е���ʼλ��
		int insert(const size_t offset, const size_t length);


		int erase(const size_t offset);

		bool query(const size_t offset, size_t& length);
		private:

		int insert(Node& node);

		//������UsedFile�е�ƫ��
		int _query(const size_t offset);


		//ֻɾ������ƽ��
		void erase(Node node, int& offset);

		int lRotate(Node& node);

		int rRotate(Node& node);

		int balance(Node& p);

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
		/// д���һ���ڵ�
		/// </summary>
		/// <param name="offset">��File�е�ƫ��</param>
		/// <param name="length">��File�еĳ���</param>
		/// <returns>д����ļ�ƫ��,<c>-1</c>ʧ��</returns>
		int writeFirstNode(const size_t offset, const size_t length);

		int bfactor(const Node& node);

		void fixHeight(Node& node);

		int nodeHeight(int m_offset);

		private:
		FileHead m_head;
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

		//���ز��뵽�ļ��е���ʼλ��
		int insert(const size_t offset, const size_t length);


		bool getEnoughPlace(const size_t length, size_t& rOffset, size_t& rLength);


		private:

		int insert(Node& node);

		void tryCombinenNode(Node& node);

		int query_length(const int length);

		void query_offset(const Node& node, int& preOffset, int& nextOffset);

		//ֻɾ������ƽ��
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
		/// д���һ���ڵ�
		/// </summary>
		/// <param name="offset">��File�е�ƫ��</param>
		/// <param name="length">��File�еĳ���</param>
		/// <returns>д����ļ�ƫ��,<c>-1</c>ʧ��</returns>
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

		bool insert(void* data, const size_t size, size_t& offset);

		bool erase(size_t offset);

		bool modify(void* data, const size_t offset, const size_t size, size_t& newOffset);

		//����ƫ���Ҵ�С
		bool query(size_t offset, size_t& size);

		/// <summary>
		/// ���ļ��ж�ȡ����
		/// </summary>
		/// <param name="data">������</param>
		/// <param name="offset">���ļ��е�ƫ��</param>
		/// <param name="length">����������</param>
		/// <param name="readSize">���ļ��ж�ȡ�ĳ���</param>
		/// <returns><c>true</c>�ɹ���<c>false</c>ʧ��</returns>
		bool read(void* data, const size_t offset, const size_t length, size_t& readSize);

		/// <summary>
		/// ���ļ�������������д������
		/// </summary>
		/// <remarks>д����ֽ���������ԭ�ֽ���������ʧ��</remarks>
		/// <param name="data">Ҫд�������</param>
		/// <param name="offset">���ļ��е�ƫ��</param>
		/// <param name="size">Ҫд����ֽ���</param>
		/// <returns><c>true</c>�ɹ���<c>false</c>ʧ��</returns>
		bool write(void* data, const size_t offset, const size_t size);

		private:
		UsedFile m_used_file;
		UnusedFile m_unused_file;
	};
}
