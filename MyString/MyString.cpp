#include "MyString.h"
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>

namespace Sdalin
{
	String::String()
	{
		char* data = (char*)calloc(1, sizeof(char));
		data[0] = '\0';
		SetData(data);
		SetSize(1);
		SetLength(0);
	}

	String::~String()
	{
	}

	String::String(const String& other) : String()
	{
		Set(other.GetData());
		SetSize(other.GetSize());
		SetLength(other.GetLength());
	}

	String::String(const char* str) : String()
	{
		Set(str);
	}

	void String::SetData(char* data)
	{
		m_data = data;
	}

	char* String::GetData() const
	{
		return m_data;
	}

	void String::SetSize(size_t size)
	{
		m_size = size;
	}

	size_t String::GetSize() const
	{
		return m_size;
	}

	void String::SetLength(size_t length)
	{
		m_length = length;
	}

	size_t String::GetLength() const
	{
		return m_length;
	}

	const char* String::c_str() const
	{
		return GetData();
	}

	int String::Compare(const char* other) const
	{
		return strcmp(GetData(), other);
	}

	char* String::Append(const char* other)
	{
		SetLength(strlen(GetData()) + strlen(other));
		if (GetLength() + 1 > GetSize())
		{
			SetSize(GetLength() + 1);
			char* tmp = (char*)realloc(GetData(), GetSize());
			SetData(tmp);
		}
		return strcat(GetData(), other);
	}

	char* String::Set(const char* str)
	{
		SetLength(strlen(str));
		if (GetLength() + 1 > GetSize())
		{
			SetSize(GetLength() + 1);
			char* tmp = (char*)realloc(GetData(), GetSize());
			SetData(tmp);
		}
		return strcpy(GetData(), str);
	}

	char* String::Lower() const
	{
		return strlwr(GetData());
	}

	char* String::Upper() const
	{
		return strupr(GetData());
	}

	char* String::Copy(const char* str)
	{
		return Set(str);
	}

	void String::Format(const char* pszFmt, ...)
	{
		char szBuff[0x1000];
		va_list args;
		va_start(args, pszFmt);
		vsnprintf(szBuff, sizeof(szBuff), pszFmt, args);
		va_end(args);
		Copy(szBuff);
	}

	int String::Hash() const
	{
		int ret = 4581523;
		for (int i = 0; i <= GetLength(); i++)
		{
			ret = (ret + int(m_data[i])) * int(m_data[i] == 0 ? 1 : m_data[i]);
		}
		return ret;
	}

	String& String::operator=(const String& obj)
	{
		this->Copy(obj.GetData());
		return *this;
	}

	String String::operator+(const char* str) const
	{
		return String(*this).Append(str);
	}

	const char& String::operator[](const int index) const
	{
		return m_data[index];
	}

	char& String::operator[](const int index)
	{
		return m_data[index];
	}

	bool String::operator==(const String& obj) const
	{
		return strcmp(this->GetData(), obj.GetData()) == 0;
	}

	bool String::operator!=(const String& obj) const
	{
		return strcmp(this->GetData(), obj.GetData()) != 0;
	}

	bool String::operator>(const String& obj) const
	{
		return strcmp(this->GetData(), obj.GetData()) > 0;
	}

	bool String::operator<(const String& obj) const
	{
		return strcmp(this->GetData(), obj.GetData()) < 0;
	}
}
