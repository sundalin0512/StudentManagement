#pragma once
#include "FileTree.hpp"
#include "types.h"
#include "File.h"

namespace Sdalin
{
	class Manage
	{
		public:
		Manage() = delete;
		Manage(String fileName, bool trunc) : file(fileName, trunc)
		{
			size_t offset = 0;
			file.insert(studentNameTree.m_root, sizeof(*studentNameTree.m_root), offset);
		}

		~Manage()
		{

		}
		void insertStudent(String ID, String name)
		{
			Student stu{ ID,name };
			studentNameTree.insert(stu);
		}
		private:
		FileTree<Student, HashName<Student>,lessID<Student>, equalID<Student>> studentNameTree;
		FileTree<Student, HashID<Student>, lessName<Student>, equalName<Student>> studentIDTree;
		FileTree<Course, HashID<Course>, lessName<Course>, equalName<Course>> courseIDTree;
		FileTree<Course, HashName<Course>, lessID<Course>, equalID<Course>> courseNameTree;
		FileTree<FileTree<StudentCourse, HashCourseID<StudentCourse>,lessStudentID<StudentCourse>,equalStudentID<StudentCourse>>, TreeHash<FileTree<StudentCourse, HashCourseID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>>>> studentCourseTree;
		FileTree<FileTree<StudentCourse, HashStudentID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>>, TreeHash<FileTree<StudentCourse, HashStudentID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>>>> courseStudentTree;
		File file;
	};

}
