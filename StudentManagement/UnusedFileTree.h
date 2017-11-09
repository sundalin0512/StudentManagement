#pragma once

class UnusedFileTree
{
	struct Node
	{
		size_t offset;
		size_t length;
		int lChild_off;
		int rChild_off;
		int parent_off;
		int lChild_length;
		int rChild_length;
		int parent_length;
		int m_height;
		Node()
			: offset(0), length(0), lChild_off(-1), rChild_off(-1), parent_off(-1),
			  lChild_length(-1), rChild_length(-1), parent_length(-1), m_height(0)
		{
		}
		Node(const size_t offset, const size_t length) : Node()
		{
			this->offset = offset;
			this->length = length;
		}
		int bfactor_off() const
		{
			return height(rChild_off) - height(lChild_off);
		}
		int bfactor_length() const
		{
			return height(rChild_length) - height(lChild_length);
		}

		void fixHeight_off()
		{
			const int lHeight = height(lChild_off);
			const int rHeight = height(rChild_off);
			m_height = (lHeight > rHeight ? lHeight : rHeight) + 1;
		}
		void fixHeight_length()
		{
			const int lHeight = height(lChild_length);
			const int rHeight = height(rChild_length);
			m_height = (lHeight > rHeight ? lHeight : rHeight) + 1;
		}
	private:
		static int height(int offset)
		{
			return p != nullptr ? p->m_height : 0;
		}
	};
	};
};