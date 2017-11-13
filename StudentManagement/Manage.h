#pragma once
#include "FileTree.hpp"
#include "types.h"
#include "File.h"

namespace Sdalin
{



	class Manage
	{
		struct RootOffset
		{
			size_t studentNameTree;
			size_t studentIDTree;
			size_t courseIDTree;
			size_t courseNameTree;
			size_t studentCourseTree;
			size_t courseStudentTree;
		}rootOffset;
		public:
		Manage() = delete;
		Manage(String fileName, bool trunc) : file(fileName, trunc)
		{
			size_t offset = 0;
			file.insert(&rootOffset, sizeof(rootOffset), offset);
			studentNameTree.writeRoot();
			studentIDTree.writeRoot();
			courseIDTree.writeRoot();
			courseNameTree.writeRoot();
			studentCourseTree.writeRoot();
			courseStudentTree.writeRoot();
			rootOffset.studentNameTree = studentNameTree.m_root->offset;
			rootOffset.studentIDTree = studentIDTree.m_root->offset;
			rootOffset.courseIDTree = courseIDTree.m_root->offset;
			rootOffset.courseNameTree = courseNameTree.m_root->offset;
			rootOffset.studentCourseTree = studentCourseTree.m_root->offset;
			rootOffset.courseStudentTree = courseStudentTree.m_root->offset;
			file.write(&rootOffset, 0, sizeof(rootOffset));
		}

		~Manage()
		{

		}
		void insertStudent(String ID, String name)
		{
			Student stu{ ID,name };
			studentNameInternalTree a;
			a.insert(stu);
			studentIDTree.insert(stu);
			auto tree = studentNameTree.query(a);
			if(tree == nullptr)
				studentNameTree.insert(a);
			else
			{
				tree->m_data.value.insert(stu);
			}
			//studentNameTree.insert(stu);
			
		}
		bool deleteStudentFromID(String ID)
		{
			Student stu{ ID };
			auto node = studentIDTree.query(stu);
			if (node == nullptr)
				return false;
			stu.name = node->m_data.value.name;
			bool success = studentIDTree.erase(stu);
			if (!success)
			{
				return success;
			}
			//success = studentNameTree.erase(stu);
			return success;

		}
		File file;
		private:

		private:
		//typedef FileTree<Student, HashID<Student>, lessID<Student>, equalID<Student>> studentNameInternalTree;
		//typedef FileTree<StudentCourse, HashCourseID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>> studentCourseInternalTree;
		//typedef FileTree<StudentCourse, HashStudentID<StudentCourse>, lessCourseID<StudentCourse>, equalCourseID<StudentCourse>> courseStudentInternalTree;

		FileTree<studentNameInternalTree, HashName<Student>, lessName<Student>, equalName<Student>> studentNameTree;

		FileTree<Student, HashID<Student>, lessID<Student>, equalID<Student>> studentIDTree;
		FileTree<Course, HashID<Course>, lessID<Course>, equalID<Course>> courseIDTree;
		FileTree<Course, HashName<Course>, lessName<Course>, equalName<Course>> courseNameTree;

		FileTree<studentCourseInternalTree, TreeHash<studentCourseInternalTree>> studentCourseTree;

		FileTree<courseStudentInternalTree, TreeHash<courseStudentInternalTree>> courseStudentTree;

	};
	extern Manage manage;
}
