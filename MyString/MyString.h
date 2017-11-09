#pragma once
#include <cstring>
#include <malloc.h>
#include <stdarg.h>
#include <cstdio>

typedef struct MyString
{
	char *data;
	size_t size;
	size_t length;
}MyString;

void MyString_SetData(MyString *mystring, char* data);

char* MyString_GetData(MyString *mystring);

void MyString_SetSize(MyString *mystring, size_t size);

size_t MyString_GetSize(MyString *mystring);

void MyString_SetLength(MyString *mystring, size_t length);

size_t MyString_GetLength(MyString *mystring);

void MyString_Init(MyString *mystring);

const char* MyString_Cstr(MyString *mystring);

int MyString_Compare(MyString *mystring, const char *other);

char* MyString_Append(MyString *mystring, const char* other);

char *MyString_Set(MyString *mystring, const char* str);

char *MyString_Lower(MyString *mystring);

char *MyString_Upper(MyString *mystring);

char* MyString_Copy(MyString *mystring, const char* str);

void MyString_Format(MyString *mystring, const char* pszFmt, ...);