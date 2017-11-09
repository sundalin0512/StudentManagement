#pragma once

enum class ReturnType
{
	RET_SUCCESS = 0,
	RET_ERROR,
	RET_NOSPACE,
	RET_ILLEGAL_INDEX,
	RET_NO_HEAP_SPACE
};

struct Student
{
	char* ID;
	char* name;
};

struct Course
{
	char *ID;
	char* name;
};

struct StudentCourse
{
	char* studentID;
	char* courseID;
	int grade;
};