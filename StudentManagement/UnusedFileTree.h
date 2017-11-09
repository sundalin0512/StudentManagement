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
		int height;
		Node()
			: offset(0), length(0), lChild_off(-1), rChild_off(-1), parent_off(-1),
			  lChild_length(-1), rChild_length(-1), parent_length(-1), height(0)
		{
		}
		Node(const size_t offset, const size_t length) : Node()
		{
			this->offset = offset;
			this->length = length;
		}
	};
};