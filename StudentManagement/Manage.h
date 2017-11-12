#pragma once
#include "FileTree.hpp"
#include "types.h"

namespace Sdalin
{
	class Manage
	{
		public:
		Manage()
		{

		}
		~Manage()
		{

		}
		private:
		FileTree<Student, HashName<Student>,lessID<Student>, equalID<Student>> studentNameTree;
		FileTree<Student, HashID<Student>, lessName<Student>, equalName<Student>> studentIDTree;
		FileTree<Course, HashID<Course>, lessName<Course>, equalName<Course>> courseIDTree;
		FileTree<Course, HashName<Course>, lessID<Course>, equalID<Course>> courseNameTree;
		FileTree<FileTree<StudentCourse, HashCourseID<StudentCourse>,lessStudentID<StudentCourse>,equalStudentID<StudentCourse>>, TreeHash<FileTree<StudentCourse, HashCourseID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>>>> studentCourseTree;
		FileTree<FileTree<StudentCourse, HashStudentID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>>, TreeHash<FileTree<StudentCourse, HashStudentID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>>>> courseStudentTree;

	};

}
