#include "stdafx.h"
#include "CppUnitTest.h"
#include "Manage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Sdalin;
namespace ManageTest
{
	TEST_CLASS(FileTest)
	{
		public:

		TEST_METHOD(StudentInsertTest)
		{
			manage.insertStudent("123456", "aassdd");
			int a = 0;
		}
	};
}