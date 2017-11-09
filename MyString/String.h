#pragma once
#include <cstring>
#include <malloc.h>
#include <stdarg.h>
#include <cstdio>

#include "RefCount.h"

namespace Sdalin
{
	class String
	{
	public:
		String()
		{
			char *data = (char*)calloc(1, sizeof(char));
			data[0] = '\0';
			SetData(data);
			SetSize(1);
			SetLength(0);
		}

		~String()
		{
		}

		String(const String &other)
		{
			Set(other.GetData());
			SetSize(other.GetSize());
			SetLength(other.GetLength());
		}

		String(const char *str)
		{
			Set(str);
		}

		void SetData(char* data)
		{
			m_data = data;
		}

		char* GetData() const
		{
			return m_data;
		}

		void SetSize(size_t size)
		{
			m_size = size;
		}

		size_t GetSize() const
		{
			return m_size;
		}

		void SetLength(size_t length)
		{
			m_length = length;
		}

		size_t GetLength() const
		{
			return m_length;
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

		int Hash() const
		{
			int ret = 0;
			for (int i = 0; i < GetLength(); i++)
			{
				ret = (ret + m_data[i]) * m_data[i] == 0 ? 1 : m_data[i];
			}
			return ret;
		}

		String& operator = (const String& obj)
		{
			this->Copy(obj.GetData());
			return *this;
		}

		String operator + (const char* str) const
		{
			return String(*this).Append(str);
		}

		const char& operator [] (const int index) const
		{
			return m_data[index];
		}

		char& operator [] (const int index)
		{
			return m_data[index];
		}

		bool operator == (const String& obj) const
		{
			return strcmp(this->GetData(), obj.GetData()) == 0;
		}

		bool operator != (const String& obj) const
		{
			return strcmp(this->GetData(), obj.GetData()) != 0;
		}

		bool operator > (const String& obj) const
		{
			return strcmp(this->GetData(), obj.GetData()) > 0;
		}

		bool operator < (const String& obj) const
		{
			return strcmp(this->GetData(), obj.GetData()) < 0;
		}
	private:
		char *m_data;
		size_t m_size;
		size_t m_length;

	};


}