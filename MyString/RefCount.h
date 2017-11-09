#pragma once
class RefCount
{
private:
	friend class CMyString; //���ֹ���˽��
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

	int AddRef()  //�������ü���
	{
		return ++count;
	}
	int DecRef()  //�������ü���
	{
		return --count;
	}
private:
	int count; //���ü���
	void *refBuffer;
};
