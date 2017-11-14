#pragma once
#include "FileTree.hpp"
#include "types.h"
#include "File.h"
#include <cassert>

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
			if(trunc)
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
			else
			{
				size_t size;
				file.read(&rootOffset, 0, sizeof(rootOffset), size);
				assert(size == sizeof(rootOffset));
				studentNameTree.readRoot(rootOffset.studentNameTree);
				studentIDTree.readRoot(rootOffset.studentIDTree);
				courseIDTree.readRoot(rootOffset.courseIDTree);
				courseNameTree.readRoot(rootOffset.courseNameTree);
				studentCourseTree.readRoot(rootOffset.studentCourseTree);
				courseStudentTree.readRoot(rootOffset.courseStudentTree);
				studentNameTree.readTreeTreeFromFile();
				studentIDTree.readTreeFromFile();
				courseIDTree.readTreeFromFile();
				courseNameTree.readTreeTreeFromFile();
				studentCourseTree.readTreeTreeFromFile();
				courseStudentTree.readTreeTreeFromFile();

			}
		}

		~Manage()
		{

		}
		void insertStudent(String ID, String name)
		{
			Student stu{ ID,name };
			studentNameInternalTree a;
			a.insertNoFile(stu);
			studentIDTree.insert(stu);
			auto tree = studentNameTree.query(a);
			if (tree == nullptr)
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
			studentNameInternalTree a;
			a.insertNoFile(stu);
			auto todelete = studentNameTree.query(a);
			todelete->m_data.value.erase(stu);
			studentNameTree.writeNode(todelete);
			studentNameTree.writeData(todelete);
			//删除学生的选课
			{
				StudentCourse section{ ID };
				studentCourseInternalTree a;
				a.insertNoFile(section);
				auto node = studentCourseTree.query(a);
				if (node == nullptr)
					return false;
				auto que = node->m_data.value.layer();
				while (!que.empty())
				{
					StudentCourse p = que.front()->m_data.value;
					deleteSelection(p.studentID, p.courseID);
					que.pop();
				}
				return success;
			}
		}

		Queue<Student> queryStudentFromName(String name)
		{
			Student stu;
			stu.name = name;
			studentNameInternalTree a;
			a.insertNoFile(stu);
			auto node = studentNameTree.query(a);
			auto que = node->m_data.value.layer();
			Queue<Student> ret;
			while (!que.empty())
			{
				ret.push(que.front()->m_data.value);
				que.pop();
			}
			return ret;
		}

		void insertCourse(String ID, String name)
		{
			Course stu{ ID,name };
			courseNameInternalTree a;
			a.insertNoFile(stu);
			courseIDTree.insert(stu);
			auto tree = courseNameTree.query(a);
			if (tree == nullptr)
				courseNameTree.insert(a);
			else
			{
				tree->m_data.value.insert(stu);
			}
			//courseNameTree.insert(stu);

		}
		bool deleteCourseFromID(String ID)
		{
			Course stu{ ID };
			auto node = courseIDTree.query(stu);
			if (node == nullptr)
				return false;
			stu.name = node->m_data.value.name;
			bool success = courseIDTree.erase(stu);
			if (!success)
			{
				return success;
			}
			courseNameInternalTree a;
			a.insertNoFile(stu);
			auto todelete = courseNameTree.query(a);
			todelete->m_data.value.erase(stu);
			courseNameTree.writeNode(todelete);
			courseNameTree.writeData(todelete);
			//删除选课的学生
			{
				StudentCourse section{ ID };
				courseStudentInternalTree a;
				a.insertNoFile(section);
				auto node = courseStudentTree.query(a);
				if (node == nullptr)
					return false;
				auto que = node->m_data.value.layer();
				while (!que.empty())
				{
					StudentCourse p = que.front()->m_data.value;
					deleteSelection(p.studentID, p.courseID);
					que.pop();
				}
				return success;
			}
		}

		Queue<Course> queryCourseFromName(String name)
		{
			Course stu;
			stu.name = name;
			courseNameInternalTree a;
			a.insertNoFile(stu);
			auto node = courseNameTree.query(a);
			auto que = node->m_data.value.layer();
			Queue<Course> ret;
			while (!que.empty())
			{
				ret.push(que.front()->m_data.value);
				que.pop();
			}
			return ret;
		}

		//选课
		void insertSelection(String studentID, String CourseID)
		{
			StudentCourse stu{ studentID,CourseID };
			{
				studentCourseInternalTree a;
				a.insertNoFile(stu);
				auto tree = studentCourseTree.query(a);
				if (tree == nullptr)
					studentCourseTree.insert(a);
				else
				{
					tree->m_data.value.insert(stu);
				}
			}
			{
				courseStudentInternalTree a;
				a.insertNoFile(stu);
				auto tree = courseStudentTree.query(a);
				if (tree == nullptr)
					courseStudentTree.insert(a);
				else
				{
					tree->m_data.value.insert(stu);
				}
			}

		}
		bool deleteSelection(String studentID, String CourseID)
		{
			StudentCourse stu{ studentID,CourseID };
			studentCourseInternalTree a;
			a.insertNoFile(stu);
			auto node = studentCourseTree.query(a);
			if (node == nullptr)
				return false;
			bool success = node->m_data.value.erase(stu);
			if (!success)
			{
				return success;
			}
			studentCourseTree.writeNode(node);
			studentCourseTree.writeData(node);
			courseStudentInternalTree b;
			b.insertNoFile(stu);
			auto node2 = courseStudentTree.query(b);
			if (node2 == nullptr)
				return false;
			success = node2->m_data.value.erase(stu);
			if (!success)
			{
				return success;
			}
			courseStudentTree.writeNode(node2);
			courseStudentTree.writeData(node2);
			return success;
		}

		bool modifyGrade(String studentID, String CourseID, int grade)
		{
			{
				StudentCourse stu{ studentID,CourseID,grade };
				studentCourseInternalTree a;
				a.insertNoFile(stu);
				auto node = studentCourseTree.query(a);
				bool success = node->m_data.value.modify(stu, stu);
				if (!success)
				{
					return success;
				}
			}
			{
				StudentCourse stu{ studentID,CourseID,grade };
				courseStudentInternalTree a;
				a.insertNoFile(stu);
				auto node = courseStudentTree.query(a);
				return node->m_data.value.modify(stu, stu);
			}
		}


		File file;
		private:

		private:
		//typedef FileTree<Student, HashID<Student>, lessID<Student>, equalID<Student>> studentNameInternalTree;
		//typedef FileTree<StudentCourse, HashStudentID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>> courseStudentInternalTree;
		//typedef FileTree<StudentCourse, HashCourseID<StudentCourse>, lessCourseID<StudentCourse>, equalCourseID<StudentCourse>> studentCourseInternalTree;
		//typedef FileTree<Course, HashID<Course>, lessID<Course>, equalID<Course>> courseNameInternalTree;
		FileTree<studentNameInternalTree, HashName<Student>, lessName<Student>, equalName<Student>> studentNameTree;

		FileTree<Student, HashID<Student>, lessID<Student>, equalID<Student>> studentIDTree;
		FileTree<courseNameInternalTree, HashName<Course>, lessName<Course>, equalName<Course>> courseNameTree;
		FileTree<Course, HashID<Course>, lessID<Course>, equalID<Course>> courseIDTree;

		FileTree<studentCourseInternalTree, HashStudentID<StudentCourse>, lessStudentID<StudentCourse>, equalStudentID<StudentCourse>> studentCourseTree;

		FileTree<courseStudentInternalTree, HashCourseID<StudentCourse>, lessCourseID<StudentCourse>, equalCourseID<StudentCourse>> courseStudentTree;

	};
	extern Manage manage;
}
