#include "stdafx.h"
#include "CppUnitTest.h"
#include "List.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ListTest
{
	TEST_CLASS(ListIntTest)
	{
	public:
		Sdalin::List<int> vec;
		ListIntTest()
		{
			vec.push_back(0);
			vec.push_back(1);
			vec.push_back(2);
			vec.push_back(3);
			vec.push_back(4);
			vec.push_back(5);
			vec.push_back(6);
			vec.push_back(7);
			vec.push_back(8);
			vec.push_back(9);
		}
		~ListIntTest()
		{

		}
	public:
		TEST_METHOD(ConstructorSizeTest)
		{
			Sdalin::List<int> vec1(20);
			Assert::AreEqual(size_t(20), vec1.size());
		}

		TEST_METHOD(AssignTest)
		{
			vec.assign(12, 3);
			Assert::AreEqual(size_t(12), vec.size());
			for (Sdalin::List<int>::iterator i = vec.begin(); i != vec.end(); ++i)
			{
				Assert::AreEqual(3, *i);
			}
		}
		TEST_METHOD(ResizeTest)
		{
			vec.resize(3);
			Assert::AreEqual(size_t(3), vec.size());
		}
		TEST_METHOD(ResizeTest2)
		{
			vec.resize(20, 55);
			Assert::AreEqual(size_t(20), vec.size());
			Sdalin::List<int>::iterator i = vec.begin();
			for (size_t count = 0; count != 18; ++i, count++) {}
			Assert::AreEqual(55, *i);
			i = vec.begin();
			for (size_t count = 0; count != 3; ++i, count++) {}
			Assert::AreNotEqual(55, *i);
		}
		TEST_METHOD(EmptyTest)
		{
			Assert::IsFalse(vec.empty());
			vec.clear();
			Assert::IsTrue(vec.empty());
		}
		TEST_METHOD(PushfrontTest)
		{
			Sdalin::List<int> vec2;
			vec2.push_front(0);
			Assert::AreEqual(size_t(1), vec2.size());
			vec2.push_front(1);
			Assert::AreEqual(size_t(2), vec2.size());
			vec2.push_front(2);
			Assert::AreEqual(size_t(3), vec2.size());
		}
		TEST_METHOD(PushbackTest)
		{
			Sdalin::List<int> vec2;
			vec2.push_back(0);
			Assert::AreEqual(size_t(1), vec2.size());
			vec2.push_back(1);
			Assert::AreEqual(size_t(2), vec2.size());
			vec2.push_back(2);
			Assert::AreEqual(size_t(3), vec2.size());
		}
		TEST_METHOD(PopfrontTest)
		{
			size_t size = vec.size();
			while (size > 0)
			{
				vec.pop_front();
				size--;
				Assert::AreEqual(size, vec.size());
			}
			try
			{
				vec.pop_front();
				throw 1;
			}
			catch (int)
			{
				Assert::Fail();
			}
			catch (...)
			{

			}
		}
		TEST_METHOD(PopbackTest)
		{
			size_t size = vec.size();
			while (size > 0)
			{
				vec.pop_back();
				size--;
				Assert::AreEqual(size, vec.size());
			}
			try
			{
				vec.pop_back();
				throw 1;
			}
			catch (int)
			{
				Assert::Fail();
			}
			catch (...)
			{

			}
		}
		TEST_METHOD(InsertTest)
		{
			const size_t size = vec.size();
			Sdalin::List<int>::iterator i = vec.begin();
			for (size_t count = 0; count != 3; ++i, count++) {}
			vec.insert(i, 12);
			i = vec.begin();
			for (size_t count = 0; count != 3; ++i, count++) {}
			Assert::AreEqual(12, *i);
			i = vec.begin();
			for (size_t count = 0; count != 4; ++i, count++) {}
			Assert::AreEqual(3, *i);
			Assert::AreEqual(size + 1, vec.size());
		}
		TEST_METHOD(InsertNTest)
		{
			const size_t size = vec.size();
			Sdalin::List<int>::iterator i = vec.begin();
			for (size_t count = 0; count != 3; ++i, count++) {}
			vec.insert(i, 2, 12);
			i = vec.begin();
			for (size_t count = 0; count != 3; ++i, count++) {}
			Assert::AreEqual(12, *i);
			i = vec.begin();
			for (size_t count = 0; count != 4; ++i, count++) {}
			Assert::AreEqual(12, *i);
			i = vec.begin();
			for (size_t count = 0; count != 5; ++i, count++) {}
			Assert::AreEqual(3, *i);
			Assert::AreEqual(size + 2, vec.size());
		}
		TEST_METHOD(EraseTest)
		{
			const size_t size = vec.size();
			Sdalin::List<int>::iterator i = vec.begin();
			for (size_t count = 0; count != 4; ++i, count++) {}
			vec.erase(i);
			i = vec.begin();
			for (size_t count = 0; count != 3; ++i, count++) {}
			Assert::AreEqual(3, *i);
			i = vec.begin();
			for (size_t count = 0; count != 4; ++i, count++) {}
			Assert::AreEqual(5, *i);
			Assert::AreEqual(size - 1, vec.size());
		}
		TEST_METHOD(EraseNTest)
		{
			const size_t size = vec.size();
			Sdalin::List<int>::iterator i = vec.begin();
			for (size_t count = 0; count != 4; ++i, count++) {}
			Sdalin::List<int>::iterator i2 = vec.begin();
			for (size_t count = 0; count != 6; ++i2, count++) {}
			vec.erase(i, i2);
			i = vec.begin();
			for (size_t count = 0; count != 3; ++i, count++) {}
			Assert::AreEqual(3, *i);
			i = vec.begin();
			for (size_t count = 0; count != 4; ++i, count++) {}
			Assert::AreEqual(6, *i);
			Assert::AreEqual(size - 2, vec.size());
		}
	};
}