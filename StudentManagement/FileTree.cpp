//#pragma once
//#include "File.h"
//namespace Sdalin
//{
//	class UnusedFile::UnusedFileTree
//	{
//		struct Node
//		{
//			size_t m_offset;
//			size_t m_length;
//			int m_leftChild_offset;
//			int m_rightChild_offset;
//			int m_parent_offset;
//			int m_leftChild_length;
//			int m_rightChild_length;
//			int m_parent_length;
//			int m_height;
//			Node()
//				: m_offset(0), m_length(0), m_leftChild_offset(-1), m_rightChild_offset(-1), m_parent_offset(-1),
//				m_leftChild_length(-1), m_rightChild_length(-1), m_parent_length(-1), m_height(0)
//			{
//			}
//			Node(const size_t m_offset, const size_t m_length) : Node()
//			{
//				this->m_offset = m_offset;
//				this->m_length = m_length;
//			}
//			int bfactor_offset() const
//			{
//				return nodeHeight(m_rightChild_offset) - nodeHeight(m_leftChild_offset);
//			}
//			int bfactor_length() const
//			{
//				return nodeHeight(m_rightChild_length) - nodeHeight(m_leftChild_length);
//			}
//
//			void fixHeight_offset()
//			{
//				const int lHeight = nodeHeight(m_leftChild_offset);
//				const int rHeight = nodeHeight(m_rightChild_offset);
//				m_height = (lHeight > rHeight ? lHeight : rHeight) + 1;
//			}
//			void fixHeight_length()
//			{
//				const int lHeight = nodeHeight(m_leftChild_length);
//				const int rHeight = nodeHeight(m_rightChild_length);
//				m_height = (lHeight > rHeight ? lHeight : rHeight) + 1;
//			}
//		private:
//			int nodeHeight(int m_offset) const
//			{
//				int ret;
//				if (m_offset == -1)
//					return 0;
//				read(m_offset + sizeof(Node) - sizeof(int), &ret, sizeof(int));
//				return ret;
//				//return p != nullptr ? p->m_height : 0;
//			}
//		};
//	};
//}