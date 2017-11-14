#include <stdlib.h>
#include <string>
#include <iostream>
#include "Manage.h"

using Sdalin::manage;
void readData()
{
	std::ifstream in("百万选课记录.txt");
	int count = 0;
	while(!in.eof())
	{
		std::string studentId;
		std::string studentName;
		std::string courseID[12];
		std::string courseName[12];
		in >> studentId >> studentName;
		for(int i =0; i< 12; i++)
		{
			in >> courseID[i] >> courseName[i];
		}
		manage.insertStudent(studentId.c_str(), studentName.c_str());
		for (int i = 0; i< 12; i++)
		{
			manage.insertCourse(courseID[i].c_str(), courseName[i].c_str());
			manage.insertSelection(studentId.c_str(), courseID[i].c_str());
		}
		std::cout << count << std::endl;
		count++;
	}
}
int main()
{
	readData();
	
	manage.insertStudent("123457", "aassdd");
	auto a = manage.queryStudentFromName("aassdd");
	manage.deleteStudentFromID("123457");
	auto b = manage.queryStudentFromName("aassdd");


	manage.insertCourse("1111111", "数学");
	manage.insertCourse("2222222", "数学");
	auto c = manage.queryCourseFromName("数学");
	manage.deleteCourseFromID("1111111");
	auto d = manage.queryCourseFromName("数学");

	
	manage.insertSelection("123456", "2222222");
	manage.insertSelection("987654", "2222222");
	manage.insertSelection("123456", "3333333");

	manage.modifyGrade("123456", "2222222",16);
	

	system("pause");
	return 0;
}
