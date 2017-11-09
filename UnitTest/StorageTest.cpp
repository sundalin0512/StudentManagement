#include "stdafx.h"
#include "CppUnitTest.h"
#include "storageIO.h"
#include "storage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StorageTest
{
	TEST_CLASS(StorageTest)
	{
	public:
		Student student[10];
		char *fileName;
		File file;
		StudentFile	studentFile;
		StorageTest()
		{
			student[0].ID = "2013084117";
			student[0].name = "sundalin";
			//student[0].sexual = "ÄÐ         ";
			//student[0].grade_C = "90";
			//student[0].Birthday = { 1993,5,12 };

			student[1].ID = "2013081234";
			student[1].name = "sssdalin";
			//student[1].sexual = "Å®         ";
			//student[1].grade_C = "60";
			//student[1].Birthday = { 1995,2,18 };
		}
		~StorageTest()
		{

		}
		TEST_METHOD(AddStudent_Test1)
		{
			fileName = "AddStudent_Test1.bin";
			studentFile.SetStudentFile(fileName);
			FILE *fp = file.CreateFile(fileName, 1000, nullptr);
			file.FileClose();
			studentFile.AddStudent(0, &student[0]);

			fp = file.FileOpen(fileName);
			char *buffer;
			size_t size;
			file.GetData((void**)&buffer, &size, 0);

			Assert::AreEqual(11, ((int*)buffer)[0]);
			Assert::AreEqual(0x1C, (int)size);
		}

		TEST_METHOD(AddStudent_Test2)
		{
			fileName = "AddStudent_Test2.bin";
			studentFile.SetStudentFile(fileName);
			FILE *fp = file.CreateFile(fileName, 1000, nullptr);
			file.FileClose();
			studentFile.AddStudent(0, &student[0]);
			studentFile.AddStudent(1, &student[0]);

			fp = file.FileOpen(fileName);
			char *buffer;
			size_t size;
			file.GetData((void**)&buffer, &size, 0);

			Assert::AreEqual(11, ((int*)buffer)[0]);
			Assert::AreEqual(0x1C, (int)size);

			free(buffer);
			file.GetData((void**)&buffer, &size, 1);

			Assert::AreEqual(11, ((int*)buffer)[0]);
			Assert::AreEqual(0x1C, (int)size);
			free(buffer);
		}

		TEST_METHOD(GetStudent_Test1)
		{
			fileName = "GetStudent_Test1.bin";
			studentFile.SetStudentFile(fileName);
			FILE *fp = file.CreateFile(fileName, 1000, nullptr);
			file.FileClose();
			studentFile.AddStudent(0, &student[1]);
			studentFile.AddStudent(0, &student[0]);

			Student tmpStudent;
			studentFile.GetStudent(0, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			//for (int i = 0; i < strlen(student[0].sexual) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].sexual[i], tmpStudent.sexual[i]);
			//}
			//for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			//}
			//Assert::AreEqual(student[0].Birthday.year, tmpStudent.Birthday.year);
			//Assert::AreEqual(student[0].Birthday.month, tmpStudent.Birthday.month);
			//Assert::AreEqual(student[0].Birthday.day, tmpStudent.Birthday.day);
		}

		TEST_METHOD(GetStudent_Test2)
		{
			fileName = "GetStudent_Test2.bin";
			studentFile.SetStudentFile(fileName);
			FILE *fp = file.CreateFile(fileName, 1000, nullptr);
			file.FileClose();
			studentFile.AddStudent(0, &student[0]);
			studentFile.AddStudent(0, &student[0]);

			Student tmpStudent;
			studentFile.GetStudent(1, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			//for (int i = 0; i < strlen(student[0].sexual) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].sexual[i], tmpStudent.sexual[i]);
			//}
			//for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			//}
			//Assert::AreEqual(student[0].Birthday.year, tmpStudent.Birthday.year);
			//Assert::AreEqual(student[0].Birthday.month, tmpStudent.Birthday.month);
			//Assert::AreEqual(student[0].Birthday.day, tmpStudent.Birthday.day);
		}

		TEST_METHOD(GetStudent_Test3)
		{
			fileName = "GetStudent_Test3.bin";
			studentFile.SetStudentFile(fileName);
			FILE *fp = file.CreateFile(fileName, 1000, nullptr);
			file.FileClose();
			studentFile.AddStudent(0, &student[0]);

			Assert::AreEqual((int)RET_ILLEGAL_INDEX, (int)studentFile.AddStudent(10, &student[0]));
		}

		TEST_METHOD(DeleteStudent_Test1)
		{
			fileName = "DeleteStudent_Test1.bin";
			studentFile.SetStudentFile(fileName);
			FILE *fp = file.CreateFile(fileName, 1000, nullptr);
			file.FileClose();
			studentFile.AddStudent(0, &student[1]);
			studentFile.AddStudent(1, &student[0]);
			studentFile.DeleteStudent(0);
			Student tmpStudent;
			studentFile.GetStudent(0, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			//for (int i = 0; i < strlen(student[0].sexual) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].sexual[i], tmpStudent.sexual[i]);
			//}
			//for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			//}
		}

		TEST_METHOD(ModifyStudent_Test1)
		{
			fileName = "ModifyStudent_Test1.bin";
			studentFile.SetStudentFile(fileName);
			FILE *fp = file.CreateFile(fileName, 1000, nullptr);
			file.FileClose();
			studentFile.AddStudent(0, &student[1]);
			studentFile.AddStudent(1, &student[1]);
			studentFile.AddStudent(1, &student[1]);
			studentFile.ModifyStudent(0, &student[0]);
			Student tmpStudent;
			studentFile.GetStudent(0, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			//for (int i = 0; i < strlen(student[0].sexual) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].sexual[i], tmpStudent.sexual[i]);
			//}
			//for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			//}
		}

		TEST_METHOD(AppendStudent_Test1)
		{
			fileName = "AppendStudent_Test1.bin";
			studentFile.SetStudentFile(fileName);
			FILE *fp = file.CreateFile(fileName, 1000, nullptr);
			file.FileClose();
			studentFile.AppendStudent(&student[1]);
			studentFile.AppendStudent(&student[1]);
			studentFile.AppendStudent(&student[0]);
			studentFile.AppendStudent(&student[1]);
			studentFile.AppendStudent(&student[0]);
			studentFile.AppendStudent(&student[0]);
			studentFile.AppendStudent(&student[1]);
			studentFile.AppendStudent(&student[0]);
			studentFile.DeleteStudent(0);
			studentFile.DeleteStudent(0);
			studentFile.DeleteStudent(0);
			studentFile.DeleteStudent(0);
			studentFile.DeleteStudent(0);
			studentFile.DeleteStudent(0);


			Student tmpStudent;
			studentFile.GetStudent(1, &tmpStudent);
			for (int i = 0; i < strlen(student[0].ID) + 1; i++)
			{
				Assert::AreEqual(student[0].ID[i], tmpStudent.ID[i]);
			}
			for (int i = 0; i < strlen(student[0].name) + 1; i++)
			{
				Assert::AreEqual(student[0].name[i], tmpStudent.name[i]);
			}
			//for (int i = 0; i < strlen(student[0].sexual) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].sexual[i], tmpStudent.sexual[i]);
			//}
			//for (int i = 0; i < strlen(student[0].grade_C) + 1; i++)
			//{
			//	Assert::AreEqual(student[0].grade_C[i], tmpStudent.grade_C[i]);
			//}
		}

		TEST_METHOD(GetStudentSize_Test1)
		{
			fileName = "GetStudentSize_Test1.bin";
			studentFile.SetStudentFile(fileName);
			FILE *fp = file.CreateFile(fileName, 1000, nullptr);
			file.FileClose();
			studentFile.AppendStudent(&student[1]);
			Assert::AreEqual(1, (int)studentFile.GetStudentSize());
			studentFile.AppendStudent(&student[1]);
			Assert::AreEqual(2, (int)studentFile.GetStudentSize());
			studentFile.AppendStudent(&student[0]);
			Assert::AreEqual(3, (int)studentFile.GetStudentSize());
			studentFile.AppendStudent(&student[1]);
			Assert::AreEqual(4, (int)studentFile.GetStudentSize());
			studentFile.AppendStudent(&student[0]);
			Assert::AreEqual(5, (int)studentFile.GetStudentSize());
			studentFile.AppendStudent(&student[0]);
			Assert::AreEqual(6, (int)studentFile.GetStudentSize());
			studentFile.AppendStudent(&student[1]);
			Assert::AreEqual(7, (int)studentFile.GetStudentSize());
			studentFile.AppendStudent(&student[0]);
			Assert::AreEqual(8, (int)studentFile.GetStudentSize());

			studentFile.DeleteStudent(0);
			Assert::AreEqual(7, (int)studentFile.GetStudentSize());
			studentFile.DeleteStudent(0);
			Assert::AreEqual(6, (int)studentFile.GetStudentSize());
			studentFile.DeleteStudent(0);
			Assert::AreEqual(5, (int)studentFile.GetStudentSize());
			studentFile.DeleteStudent(0);
			Assert::AreEqual(4, (int)studentFile.GetStudentSize());
			studentFile.DeleteStudent(0);
			Assert::AreEqual(3, (int)studentFile.GetStudentSize());
			studentFile.DeleteStudent(0);
			Assert::AreEqual(2, (int)studentFile.GetStudentSize());
			studentFile.DeleteStudent(0);
			Assert::AreEqual(1, (int)studentFile.GetStudentSize());
			studentFile.DeleteStudent(0);
			Assert::AreEqual(0, (int)studentFile.GetStudentSize());
		}
	};
}