#include "stdafx.h"
#include "CppUnitTest.h"
#include "File.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sdalin;
namespace FileTest
{		
	TEST_CLASS(FileTest)
	{
	public:
		
		TEST_METHOD(WriteRead)
		{
			//文件成功生成
			FileBase file("test1.bin",true);
			file.write(0, "123", 4);
			char c[4];
			file.read(0, c, 4);
			Assert::AreEqual("123", c);
		}


		TEST_METHOD(ReadWrite)
		{
			//文件重新打开时没有被覆盖
			{
				File file("test2.bin",true);
				file.write(0, "123", 4);
				char c[4];
				file.read(0, c, 4);
				Assert::AreEqual("123", c);
			}
			{
				File file("test2.bin");
				char c[4];
				file.read(0, c, 4);
				Assert::AreEqual("123", c);
			}
		}

	};
}