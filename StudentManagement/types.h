#pragma once

struct Student
{
	char* ID;
	char* name;
};

struct Course
{
	char* ID;
	char* name;
};

struct StudentCourse
{
	char* studentID;
	char* courseID;
	int grade;
};