#pragma once
#include <cstring>
#include <malloc.h>
#include <stdarg.h>
#include <cstdio>

#include "RefCount.h"

class CMyString
{
public:
	CMyString()
		:refCount(_data)
	{
		char *data = (char*)calloc(1, sizeof(char));
		data[0] = '\0';
		SetData(data);
		SetSize(1);
		SetLength(0);
	}

	~CMyString()
	{
	}

	CMyString(const CMyString &other)
		:refCount(refCount)
	{
		SetData(other.GetData());
		SetSize(other.GetSize());
		SetLength(other.GetLength());
	}

	CMyString(const char *str)
		:refCount(_data)
	{
		Set(str);
		refCount.refBuffer = _data;
	}

	void SetData(char* data)
	{
		_data = data;
	}

	char* GetData() const
	{
			return _data;
		}

	void SetSize(size_t size)
	{
		_size = size;
	}

	size_t GetSize() const
	{
		return _size;
	}

	void SetLength(size_t length)
	{
		_length = length;
	}

	size_t GetLength() const
	{
		return _length;
	}

	const char* c_str() const
	{
		return GetData();
	}

	int Compare(const char *other) const
	{
		return strcmp(GetData(), other);
	}

	char* Append(const char* other)
	{
		SetLength(strlen(GetData()) + strlen(other));
		if (GetLength() + 1 > GetSize())
		{
			SetSize(GetLength() + 1);
			char *tmp = (char*)realloc(GetData(), GetSize());
			SetData(tmp);
		}
		return strcat(GetData(), other);
	}

	char *Set(const char* str)
	{
		SetLength(strlen(str));
		if (GetLength() + 1 > GetSize())
		{
			SetSize(GetLength() + 1);
			char *tmp = (char*)realloc(GetData(), GetSize());
			SetData(tmp);
		}
		return strcpy(GetData(), str);
	}

	char *Lower() const
	{
		return strlwr(GetData());
	}

	char *Upper() const
	{
		return strupr(GetData());
	}

	char* Copy(const char* str)
	{
		return Set(str);
	}

	void Format(const char* pszFmt, ...)
	{
		char szBuff[0x1000];
		va_list args;
		va_start(args, pszFmt);
		vsnprintf(szBuff, sizeof(szBuff), pszFmt, args);
		va_end(args);
		Copy(szBuff);
	}

	CMyString operator = (const CMyString& obj)
	{
		this->Copy(obj.GetData());
	}

	CMyString operator + (const char* str)
	{
		return CMyString(*this).Append(str);
	}

	char operator [] (const int index)
	{
		return _data[index];
	}

	bool operator == (const CMyString& obj)
	{
		return strcmp(this->GetData(), obj.GetData()) == 0;
	}

	bool operator != (const CMyString& obj)
	{
		return strcmp(this->GetData(), obj.GetData()) != 0;
	}

	bool operator > (const CMyString& obj)
	{
		return strcmp(this->GetData(), obj.GetData()) > 0;
	}

	bool operator < (const CMyString& obj)
	{
		return strcmp(this->GetData(), obj.GetData()) < 0;
	}
	private:
		char *_data;
		size_t _size;
		size_t _length;
		
		RefCount refCount;
};


