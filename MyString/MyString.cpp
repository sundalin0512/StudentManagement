#include "MyString.h"

void MyString_SetData(MyString * mystring, char * data)
{
	mystring->data = data;
}

char * MyString_GetData(MyString * mystring)
{
	return mystring->data;
}

void MyString_SetSize(MyString * mystring, size_t size)
{
	mystring->size = size;
}

size_t MyString_GetSize(MyString * mystring)
{
	return mystring->size;
}

void MyString_SetLength(MyString * mystring, size_t length)
{
	mystring->length = length;
}

size_t MyString_GetLength(MyString * mystring)
{
	return mystring->length;
}

void MyString_Init(MyString * mystring)
{
	char *data = (char*)calloc(1, sizeof(char));
	data[0] = '\0';
	MyString_SetData(mystring, data);
	MyString_SetSize(mystring, 1);
	MyString_SetLength(mystring, 0);
}

const char * MyString_Cstr(MyString * mystring)
{
	return MyString_GetData(mystring);
}

int MyString_Compare(MyString * mystring, const char * other)
{
	return strcmp(MyString_GetData(mystring), other);
}

char * MyString_Append(MyString * mystring, const char * other)
{
	MyString_SetLength(mystring, strlen(MyString_GetData(mystring)) + strlen(other));
	if (MyString_GetLength(mystring) + 1 > MyString_GetSize(mystring))
	{
		MyString_SetSize(mystring, MyString_GetLength(mystring) + 1);
		char *tmp = (char*)realloc(MyString_GetData(mystring), MyString_GetSize(mystring));
		MyString_SetData(mystring, tmp);
	}
	return strcat(MyString_GetData(mystring), other);
}

char * MyString_Set(MyString * mystring, const char * str)
{
	MyString_SetLength(mystring, strlen(str));
	if (MyString_GetLength(mystring) + 1 > MyString_GetSize(mystring))
	{
		MyString_SetSize(mystring, MyString_GetLength(mystring) + 1);
		char *tmp = (char*)realloc(MyString_GetData(mystring), MyString_GetSize(mystring));
		MyString_SetData(mystring, tmp);
	}
	return strcpy(MyString_GetData(mystring), str);
}

char * MyString_Lower(MyString * mystring)
{
	return strlwr(MyString_GetData(mystring));
}

char * MyString_Upper(MyString * mystring)
{
	return strupr(MyString_GetData(mystring));
}

char * MyString_Copy(MyString * mystring, const char * str)
{
	return MyString_Set(mystring, str);
}

void MyString_Format(MyString * mystring, const char * pszFmt, ...)
{
	char szBuff[0x1000];
	va_list args;
	va_start(args, pszFmt);
	vsnprintf(szBuff, sizeof(szBuff), pszFmt, args);
	va_end(args);
	MyString_Copy(mystring, szBuff);
}
