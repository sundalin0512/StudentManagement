#pragma once
namespace Sdalin
{
	class RefCount
	{
	private:
		friend class String; 
		RefCount(void *refBuffer)
		{
			this->refBuffer = refBuffer;
			count = 0;
			AddRef();
		}
		RefCount(const RefCount& obj)
		{
			AddRef();
		}
		~RefCount()
		{
			if (DecRef() == 0)
			{
				if (refBuffer != nullptr)
				{
					delete[] refBuffer;
					refBuffer = nullptr;
				}
			}
		}

		int AddRef()  //增加引用计数
		{
			return ++count;
		}
		int DecRef()  //减少引用计数
		{
			return --count;
		}
	private:
		int count; //引用计数
		void *refBuffer;
	};
}