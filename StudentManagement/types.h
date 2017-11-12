#pragma once
#include "../MyString/MyString.h"
using Sdalin::String;

template<class T>
struct HashID 
{
	T& t;
	HashID(T& t) : t(t){}
	operator int()
	{
		return t.hashID();
	}

};
template<class T>
struct HashName
{
	T& t;
	HashName(T& t) : t(t) {}
	operator int()
	{
		return t.hashName();
	}

};

template <class T>
struct lessID
{
	T& t1;
	T& t2;
	lessID(T& t1, T& t2): t1(t1),t2(t2){}
	operator bool ()
	{
		return t1.ID < t2.ID;
	}
};
template <class T>
struct equalID
{
	T& t1;
	T& t2;
	equalID(T& t1, T& t2) : t1(t1), t2(t2) {}
	operator bool()
	{
		return t1.ID == t2.ID;
	}
};
template <class T>
struct lessName
{
	T& t1;
	T& t2;
	lessName(T& t1, T& t2) : t1(t1), t2(t2) {}
	operator bool()
	{
		return t1.name < t2.name;
	}
};
template <class T>
struct equalName
{
	T& t1;
	T& t2;
	equalName(T& t1, T& t2) : t1(t1), t2(t2) {}
	operator bool()
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
	int grade;
	
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
	T& t;
	HashStudentID(T& t) : t(t) {}
	operator int()
	{
		return t.hashStudentID();
	}
};
template <class T>
struct HashCourseID
{
	T& t;
	HashCourseID(T& t) : t(t) {}
	operator int()
	{
		return t.hashCourseID();
	}
};

template <class T>
struct lessStudentID
{
	T& t1;
	T& t2;
	lessStudentID(T& t1, T& t2) : t1(t1), t2(t2) {}
	operator bool()
	{
		return t1.studentID < t2.studentID;
	}
};
template <class T>
struct equalStudentID
{
	T& t1;
	T& t2;
	equalStudentID(T& t1, T& t2) : t1(t1), t2(t2) {}
	operator bool()
	{
		return t1.studentID == t2.studentID;
	}
};
template <class T>
struct lessCourseID
{
	T& t1;
	T& t2;
	lessCourseID(T& t1, T& t2) : t1(t1), t2(t2) {}
	operator bool()
	{
		return t1.courseID < t2.courseID;
	}
};
template <class T>
struct equalCourseID
{
	T& t1;
	T& t2;
	equalCourseID(T& t1, T& t2) : t1(t1), t2(t2) {}
	operator bool()
	{
		return t1.courseID == t2.courseID;
	}
};