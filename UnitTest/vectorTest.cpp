#include "stdafx.h"
#include "CppUnitTest.h"
#include "Vector.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace VectorTest
{
	TEST_CLASS(VectorIntTest)
	{
	public:
		Sdalin::Vector<int> vec;
		VectorIntTest()
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
		~VectorIntTest()
		{

		}
	public:
		TEST_METHOD(ConstructorSizeTest)
		{
			Sdalin::Vector<int> vec1(20);
			Assert::AreEqual(size_t(20), vec1.size());
		}

		TEST_METHOD(AssignTest)
		{
			vec.assign(12, 3);
			Assert::AreEqual(size_t(12), vec.size());
			for (size_t i = 0; i < 12; i++)
			{
				Assert::AreEqual(3, vec[i]);
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
			Assert::AreEqual(55, vec[18]);
			Assert::AreNotEqual(55, vec[3]);
		}
		TEST_METHOD(EmptyTest)
		{
			Assert::IsFalse(vec.empty());
			vec.clear();
			Assert::IsTrue(vec.empty());
		}
		TEST_METHOD(PushbackTest)
		{
			Sdalin::Vector<int> vec2;
			vec2.push_back(0);
			Assert::AreEqual(size_t(1), vec2.size());
			vec2.push_back(1);
			Assert::AreEqual(size_t(2), vec2.size());
			vec2.push_back(2);
			Assert::AreEqual(size_t(3), vec2.size());
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
			vec.insert(3, 12);
			Assert::AreEqual(12, vec[3]);
			Assert::AreEqual(3, vec[4]);
			Assert::AreEqual(size + 1, vec.size());
		}
		TEST_METHOD(InsertNTest)
		{
			const size_t size = vec.size();
			vec.insert(3, 2, 12);
			Assert::AreEqual(12, vec[3]);
			Assert::AreEqual(12, vec[4]);
			Assert::AreEqual(3, vec[5]);
			Assert::AreEqual(size + 2, vec.size());
		}
		TEST_METHOD(EraseTest)
		{
			const size_t size = vec.size();
			vec.erase(4);
			Assert::AreEqual(3, vec[3]);
			Assert::AreEqual(5, vec[4]);
			Assert::AreEqual(size - 1, vec.size());
		}
		TEST_METHOD(EraseNTest)
		{
			const size_t size = vec.size();
			vec.erase(4, 6);
			Assert::AreEqual(3, vec[3]);
			Assert::AreEqual(6, vec[4]);
			Assert::AreEqual(size - 2, vec.size());
		}
	};
}