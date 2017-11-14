#pragma once
#include "../MyString/MyString.h"
using Sdalin::String;

namespace Sdalin {

	template<class T>
	struct HashID
	{
		const T& t;
		HashID(const T& t) : t(t) {}
		operator int()const
		{
			return t.hashID();
		}

	};
	template<class T>
	struct HashName
	{
		const T& t;
		HashName(const T& t) : t(t) {}
		operator int()const
		{
			return t.hashName();
		}

	};





	template <class T>
	struct lessID
	{
		const T& t1;
		const T& t2;
		lessID(const T& t1, const T& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.ID < t2.ID;
		}
	};
	template <class T>
	struct equalID
	{
		const T& t1;
		const T& t2;
		equalID(const T& t1, const T& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.ID == t2.ID;
		}
	};
	template <class T>
	struct lessName
	{
		const T& t1;
		const T& t2;
		lessName(const T& t1, const T& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.name < t2.name;
		}
	};
	template <class T>
	struct equalName
	{
		const T& t1;
		const T& t2;
		equalName(const T& t1, const T& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.name == t2.name;
		}
	};
	struct Student
	{
		String ID;
		String name;

		int hashID() const
		{
			return ID.Hash();
		}
		int hashName() const
		{
			return name.Hash();
		}
	};

	struct Course
	{
		String ID;
		String name;
		int hashID() const
		{
			return ID.Hash();
		}
		int hashName() const
		{
			return name.Hash();
		}
	};



	struct StudentCourse
	{
		String studentID;
		String courseID;
		int grade = -1;

		int hashStudentID() const
		{
			return studentID.Hash();
		}
		int hashCourseID() const
		{
			return courseID.Hash();
		}
	};
	template <class T>
	struct HashStudentID
	{
		const T& t;
		HashStudentID(const T& t) : t(t) {}
		operator int()const
		{
			return t.hashStudentID();
		}
	};
	template <class T>
	struct HashCourseID
	{
		const T& t;
		HashCourseID(const T& t) : t(t) {}
		operator int()const
		{
			return t.hashCourseID();
		}
	};

	template <class T>
	struct lessStudentID
	{
		const T& t1;
		const T& t2;
		lessStudentID(const T& t1, const T& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.studentID < t2.studentID;
		}
	};
	template <class T>
	struct equalStudentID
	{
		const T& t1;
		const T& t2;
		equalStudentID(const T& t1, const T& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.studentID == t2.studentID;
		}
	};
	template <class T>
	struct lessCourseID
	{
		const T& t1;
		const T& t2;
		lessCourseID(const T& t1, const T& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.courseID < t2.courseID;
		}
	};
	template <class T>
	struct equalCourseID
	{
		const T& t1;
		const T& t2;
		equalCourseID(const T& t1, const T& t2) : t1(t1), t2(t2) {}
		operator bool()const
		{
			return t1.courseID == t2.courseID;
		}
	};

}