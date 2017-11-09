#include "stdafx.h"
#include "CppUnitTest.h"
#include "storageIO.h"
#include "storage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StorageIOTest
{
	TEST_CLASS(StorageIOTest)
	{
	public:
		FILE *fp;
		File file;
		StorageIOTest()
		{
			fp = file.CreateFile("test.bin", 1024 * 1024, NULL);
		}
		~StorageIOTest()
		{
			file.FileClose();
		}
		TEST_METHOD(AppendData_Test1)
		{

			char *szHello = "00000000000000000";
			int index = 0;
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 0);
		}

		TEST_METHOD(AppendData_Test2)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 0);
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 1);
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 2);
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			Assert::AreEqual(index, 3);
		}

		TEST_METHOD(InsertData_Test1)
		{
			file.InsertData(0, "00000000", 9);
			file.InsertData(0, "11111111", 9);

			char szTest[] = "00000000""\0""11111111";
			char *buffer = new char[sizeof(szTest)];
			file.FileRead(sizeof(FileInfo), buffer, sizeof(szTest));
			for (int i = 0; i < sizeof(szTest); i++)
			{
				Assert::AreEqual(szTest[i], buffer[i]);
			}
			FileInfo fileinfo;
			file.FileRead(0, &fileinfo, sizeof(fileinfo));
			Assert::AreEqual(size_t(sizeof(FileInfo) + 18), fileinfo.offsetUsed);
			Assert::AreEqual(size_t(sizeof(FileInfo) + 18 + 20), fileinfo.offsetUnused);
		}

		TEST_METHOD(DeleteData_Test1)
		{
			file.InsertData(0, "00000000", 9);
			file.InsertData(0, "11111111", 9);
			file.DeleteData(1);
			file.InsertData(0, "00000000", 9);

			char szTest[] = "00000000""\0""11111111";
			char *buffer = new char[sizeof(szTest)];
			file.FileRead(sizeof(FileInfo), buffer, sizeof(szTest));
			for (int i = 0; i < sizeof(szTest); i++)
			{
				Assert::AreEqual(szTest[i], buffer[i]);
			}
			FileInfo fileinfo;
			file.FileRead(0, &fileinfo, sizeof(fileinfo));
			Assert::AreEqual(size_t(sizeof(FileInfo) + 18), fileinfo.offsetUsed);
			Assert::AreEqual(size_t(sizeof(FileInfo) + 18 + 20), fileinfo.offsetUnused);
		}

		TEST_METHOD(Defragment_Test1)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			file.AppendData(szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			file.DeleteData(9);

			file.DeleteData(7);
			file.DeleteData(5);
			file.DeleteData(3);
			file.DeleteData(3);
			file.DeleteData(0);
			file.DeleteData(0);
			file.DeleteData(0);
			file.DeleteData(0);
			file.DeleteData(0);

			file.Defragment();
			int buffer[2];
			file.FileRead(sizeof(FileInfo), buffer, 2 * sizeof(size_t));
			Assert::AreEqual(0, buffer[1]);
			Assert::AreEqual(0, buffer[0]);

		}

		TEST_METHOD(Defragment_Test2)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			file.AppendData(szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			file.DeleteData(9);

			file.DeleteData(7);
			file.DeleteData(5);
			file.DeleteData(3);
			file.DeleteData(3);

			file.Defragment();
			char szTest[] = "00000000000000000\00011111111111111111\0002222222222222222222222222222\00066666666666666666666\0008888888888888888888888888888888888888888888";
			char *buffer = new char[sizeof(szTest)];
			file.FileRead(sizeof(FileInfo), buffer, sizeof(szTest));
			for (int i = 0; i < sizeof(szTest); i++)
			{
				Assert::AreEqual(szTest[i], buffer[i]);
			}
			FileInfo fileinfo;
			file.FileRead(0, &fileinfo, sizeof(fileinfo));
			Assert::AreEqual(size_t(sizeof(FileInfo) + 130), fileinfo.offsetUsed);
			Assert::AreEqual(size_t(sizeof(FileInfo) + 130 + 44), fileinfo.offsetUnused);
		}

		TEST_METHOD(GetData_Test1)
		{
			char *szHello[10];
			szHello[0] = "00000000000000000";
			int index = 0;
			file.AppendData(szHello[0], strlen(szHello[0]) + 1, &index);
			szHello[1] = "11111111111111111";
			file.AppendData(szHello[1], strlen(szHello[1]) + 1, &index);
			szHello[2] = "2222222222222222222222222222";
			file.AppendData(szHello[2], strlen(szHello[2]) + 1, &index);


			szHello[3] = "33333333333333333333333";
			file.AppendData(szHello[3], strlen(szHello[3]) + 1, &index);
			szHello[4] = "444444444444";
			file.AppendData(szHello[4], strlen(szHello[4]) + 1, &index);
			szHello[5] = "555555555555555555555555555555555";
			file.AppendData(szHello[5], strlen(szHello[5]) + 1, &index);
			szHello[6] = "66666666666666666666";
			file.AppendData(szHello[6], strlen(szHello[6]) + 1, &index);
			szHello[7] = "7";
			file.AppendData(szHello[7], strlen(szHello[7]) + 1, &index);
			szHello[8] = "8888888888888888888888888888888888888888888";
			file.AppendData(szHello[8], strlen(szHello[8]) + 1, &index);

			szHello[9] = "999999999999999";
			file.AppendData(szHello[9], strlen(szHello[9]) + 1, &index);

			char *data = nullptr;
			size_t size;
			for (int j = 0; j < 10; j++)
			{
				file.GetData((void**)&data, &size, j);
				for (int i = 0; i < size; i++)
				{
					Assert::AreEqual(szHello[j][i], data[i]);
				}
				free(data);
			}
		}

		TEST_METHOD(GetData_Test2)
		{
			char *szHello[10];
			szHello[0] = "00000000000000000";
			int index = 0;
			file.AppendData(szHello[0], strlen(szHello[0]) + 1, &index);
			szHello[1] = "11111111111111111";
			file.AppendData(szHello[1], strlen(szHello[1]) + 1, &index);
			szHello[2] = "2222222222222222222222222222";
			file.AppendData(szHello[2], strlen(szHello[2]) + 1, &index);


			szHello[3] = "33333333333333333333333";
			file.AppendData(szHello[3], strlen(szHello[3]) + 1, &index);
			szHello[4] = "444444444444";
			file.AppendData(szHello[4], strlen(szHello[4]) + 1, &index);
			szHello[5] = "555555555555555555555555555555555";
			file.AppendData(szHello[5], strlen(szHello[5]) + 1, &index);
			szHello[6] = "66666666666666666666";
			file.AppendData(szHello[6], strlen(szHello[6]) + 1, &index);
			szHello[7] = "7";
			file.AppendData(szHello[7], strlen(szHello[7]) + 1, &index);
			szHello[8] = "8888888888888888888888888888888888888888888";
			file.AppendData(szHello[8], strlen(szHello[8]) + 1, &index);

			szHello[9] = "999999999999999";
			file.AppendData(szHello[9], strlen(szHello[9]) + 1, &index);

			file.DeleteData(9);

			file.DeleteData(7);
			file.DeleteData(5);
			file.DeleteData(3);
			file.DeleteData(3);

			char *data = nullptr;
			size_t size;
			int count = 0;
			for (int j = 0; j < 10; j++)
			{
				if (j == 3 || j == 4 || j == 5 || j == 7 || j == 9)
					continue;
				file.GetData((void**)&data, &size, count);
				count++;
				for (int i = 0; i < size; i++)
				{
					Assert::AreEqual(szHello[j][i], data[i]);
				}
				free(data);
			}
		}

		TEST_METHOD(GetData_Test3)
		{
			char *szHello[10];
			szHello[0] = "00000000000000000";
			int index = 0;
			file.AppendData(szHello[0], strlen(szHello[0]) + 1, &index);
			szHello[1] = "11111111111111111";
			file.AppendData(szHello[1], strlen(szHello[1]) + 1, &index);
			szHello[2] = "2222222222222222222222222222";
			file.AppendData(szHello[2], strlen(szHello[2]) + 1, &index);


			szHello[3] = "33333333333333333333333";
			file.AppendData(szHello[3], strlen(szHello[3]) + 1, &index);
			szHello[4] = "444444444444";
			file.AppendData(szHello[4], strlen(szHello[4]) + 1, &index);
			szHello[5] = "555555555555555555555555555555555";
			file.AppendData(szHello[5], strlen(szHello[5]) + 1, &index);
			szHello[6] = "66666666666666666666";
			file.AppendData(szHello[6], strlen(szHello[6]) + 1, &index);
			szHello[7] = "7";
			file.AppendData(szHello[7], strlen(szHello[7]) + 1, &index);
			szHello[8] = "8888888888888888888888888888888888888888888";
			file.AppendData(szHello[8], strlen(szHello[8]) + 1, &index);

			szHello[9] = "999999999999999";
			file.AppendData(szHello[9], strlen(szHello[9]) + 1, &index);

			file.DeleteData(9);

			file.DeleteData(7);
			file.DeleteData(5);
			file.DeleteData(3);
			file.DeleteData(3);
			file.Defragment();
			char *data = nullptr;
			size_t size;
			int count = 0;
			for (int j = 0; j < 10; j++)
			{
				if (j == 3 || j == 4 || j == 5 || j == 7 || j == 9)
					continue;
				file.GetData((void**)&data, &size, count);
				count++;
				for (int i = 0; i < size; i++)
				{
					Assert::AreEqual(szHello[j][i], data[i]);
				}
				free(data);
			}
		}

		TEST_METHOD(GetDataNumber_Test1)
		{
			file.InsertData(0, "00000000", 9);
			Assert::AreEqual(1, (int)file.GetDataNumber());
			file.InsertData(0, "00000000", 9);
			Assert::AreEqual(2, (int)file.GetDataNumber());
			file.InsertData(0, "00000000", 9);
			Assert::AreEqual(3, (int)file.GetDataNumber());
			file.InsertData(0, "00000000", 9);
			Assert::AreEqual(4, (int)file.GetDataNumber());
			file.InsertData(0, "00000000", 9);
			Assert::AreEqual(5, (int)file.GetDataNumber());
			file.InsertData(0, "00000000", 9);
			Assert::AreEqual(6, (int)file.GetDataNumber());
			file.InsertData(0, "00000000", 9);
			Assert::AreEqual(7, (int)file.GetDataNumber());
			file.DeleteData(1);
			Assert::AreEqual(6, (int)file.GetDataNumber());
			file.DeleteData(1);
			Assert::AreEqual(5, (int)file.GetDataNumber());
			file.DeleteData(1);
			Assert::AreEqual(4, (int)file.GetDataNumber());
			file.DeleteData(1);
			Assert::AreEqual(3, (int)file.GetDataNumber());
			file.DeleteData(1);
			Assert::AreEqual(2, (int)file.GetDataNumber());
			file.DeleteData(1);
			Assert::AreEqual(1, (int)file.GetDataNumber());
			file.DeleteData(0);
			Assert::AreEqual(0, (int)file.GetDataNumber());
		}

		TEST_METHOD(GetFileStatus_Test1)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			file.DeleteData(1);
			int size[3] = { 18,18,0 };
			int used[3] = { 1,0,1 };
			int *sizeList;
			int *statusList;
			int ListSize;
			file.GetFileStatus(&sizeList, &statusList, &ListSize);
			Assert::AreEqual(3, ListSize);
			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}

		TEST_METHOD(GetFileStatus_Test2)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			int size[3] = { 18 };
			int used[3] = { 1 };
			int *sizeList;
			int *statusList;
			int ListSize;
			file.GetFileStatus(&sizeList, &statusList, &ListSize);
			Assert::AreEqual(1, ListSize);
			for (int i = 0; i < 1; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}
		TEST_METHOD(GetFileStatus_Test3)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			file.AppendData(szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			file.DeleteData(9);

			file.DeleteData(7);
			file.DeleteData(5);
			file.DeleteData(3);
			file.DeleteData(3);

			//Defragment(fp);
			int size[7] = { 65, 71, 21, 2, 44, 16, 0 };
			int used[7] = { 1 ,0, 1, 0, 1, 0, 1 };
			int *sizeList;
			int *statusList;
			int ListSize;
			file.GetFileStatus(&sizeList, &statusList, &ListSize);
			Assert::AreEqual(7, ListSize);
			for (int i = 0; i < 7; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}

		TEST_METHOD(GetFileStatus_Test4)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			file.AppendData(szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			file.DeleteData(9);

			file.DeleteData(7);
			file.DeleteData(5);
			file.DeleteData(3);
			file.DeleteData(3);
			file.DeleteData(0);
			file.DeleteData(0);
			file.DeleteData(0);
			file.DeleteData(0);
			file.DeleteData(0);

			//Defragment(fp);
			int size[3] = { 0, 65 + 71 + 21 + 2 + 44 + 16,0 };
			int used[3] = { 1 ,0, 1 };
			int *sizeList;
			int *statusList;
			int ListSize;
			file.GetFileStatus(&sizeList, &statusList, &ListSize);
			Assert::AreEqual(3, ListSize);
			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}

		TEST_METHOD(GetFileStatus_Test5)
		{
			char *szHello = "00000000000000000";
			int index = 0;
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "11111111111111111";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "2222222222222222222222222222";
			file.AppendData(szHello, strlen(szHello) + 1, &index);


			szHello = "33333333333333333333333";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "444444444444";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "555555555555555555555555555555555";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "66666666666666666666";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "7";
			file.AppendData(szHello, strlen(szHello) + 1, &index);
			szHello = "8888888888888888888888888888888888888888888";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			szHello = "999999999999999";
			file.AppendData(szHello, strlen(szHello) + 1, &index);

			file.DeleteData(9);
			file.DeleteData(7);
			file.DeleteData(5);
			file.DeleteData(3);
			file.DeleteData(3);
			file.DeleteData(0);
			file.DeleteData(0);
			file.DeleteData(0);
			file.DeleteData(0);
			file.DeleteData(0);

			file.Defragment();
			int size[1] = { 0 };
			int used[1] = { 0 };
			int *sizeList;
			int *statusList;
			int ListSize;
			file.GetFileStatus(&sizeList, &statusList, &ListSize);
			Assert::AreEqual(1, ListSize);
			for (int i = 0; i < 1; i++)
			{
				Assert::AreEqual(size[i], sizeList[i]);
				Assert::AreEqual(used[i], statusList[i]);
			}
		}

	};

}