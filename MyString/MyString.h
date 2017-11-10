#pragma once


#include "RefCount.h"

namespace Sdalin
{
	class String
	{
	public:
		String();

		~String();

		String(const String& other);

		String(const char* str);

		void SetData(char* data);

		char* GetData() const;

		void SetSize(size_t size);

		size_t GetSize() const;

		void SetLength(size_t m_length);

		size_t GetLength() const;

		const char* c_str() const;

		int Compare(const char* other) const;

		char* Append(const char* other);

		char* Set(const char* str);

		char* Lower() const;

		char* Upper() const;

		char* Copy(const char* str);

		void Format(const char* pszFmt, ...);

		int Hash() const;

		String& operator =(const String& obj);

		String operator +(const char* str) const;

		const char& operator [](const int index) const;

		char& operator [](const int index);

		bool operator ==(const String& obj) const;

		bool operator !=(const String& obj) const;

		bool operator >(const String& obj) const;

		bool operator <(const String& obj) const;
	private:
		char *m_data;
		size_t m_size;
		size_t m_length;

	};


}