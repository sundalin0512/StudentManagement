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

		TEST_METHOD(UnusedFileInit)
		{
			{
				File file("UnusedFileInit.bin", true);
			}
			UnusedFile file("unusedUnusedFileInit.bin");
			int c[4];
			file.read(0, c, 4*sizeof(int));
			Assert::AreEqual(0, c[0]);
			Assert::AreEqual(-1, c[1]);
			Assert::AreEqual(-1, c[2]);
		}
		TEST_METHOD(UnusedFileInsert)
		{
			UnusedFile file("UnusedFileInsert.bin", true);
			file.insert(55, 55);
			file.insert(15, 15);
			file.insert(135, 135);
			UnusedFile::Node node = file.readNode(0x28);
			Assert::AreEqual(size_t(55), node.m_offset);
			Assert::AreEqual(size_t(55), node.m_length);
			Assert::AreEqual(0x50, node.m_leftChild_length);
			Assert::AreEqual(0x78, node.m_rightChild_length);
			Assert::AreEqual(-1, node.m_parent_length);
			Assert::AreEqual(0x50, node.m_leftChild_offset);
			Assert::AreEqual(0x78, node.m_rightChild_offset);
			Assert::AreEqual(-1, node.m_parent_offset);

			node = file.readNode(0x50);
			Assert::AreEqual(size_t(15), node.m_offset);
			Assert::AreEqual(size_t(15), node.m_length);
			Assert::AreEqual(-1, node.m_leftChild_length);
			Assert::AreEqual(-1, node.m_rightChild_length);
			Assert::AreEqual(0x28, node.m_parent_length);
			Assert::AreEqual(-1, node.m_leftChild_offset);
			Assert::AreEqual(-1, node.m_rightChild_offset);
			Assert::AreEqual(0x28, node.m_parent_offset);

			node = file.readNode(0x78);
			Assert::AreEqual(size_t(135), node.m_offset);
			Assert::AreEqual(size_t(135), node.m_length);
			Assert::AreEqual(-1, node.m_leftChild_length);
			Assert::AreEqual(-1, node.m_rightChild_length);
			Assert::AreEqual(0x28, node.m_parent_length);
			Assert::AreEqual(-1, node.m_leftChild_offset);
			Assert::AreEqual(-1, node.m_rightChild_offset);
			Assert::AreEqual(0x28, node.m_parent_offset);
		}
		TEST_METHOD(UnusedFileInsertBalance)
		{
			UnusedFile file("UnusedFileInsertBalance.bin", true);
			file.insert(1, 1);
			file.insert(10, 10);
			file.insert(30, 30);
			UnusedFile::Node node = file.readNode(0x28);
			Assert::AreEqual(size_t(1), node.m_offset);
			Assert::AreEqual(size_t(1), node.m_length);
			Assert::AreEqual(-1, node.m_leftChild_length);
			Assert::AreEqual(-1, node.m_rightChild_length);
			Assert::AreEqual(0x50, node.m_parent_length);
			Assert::AreEqual(-1, node.m_leftChild_offset);
			Assert::AreEqual(-1, node.m_rightChild_offset);
			Assert::AreEqual(0x50, node.m_parent_offset);

			node = file.readNode(0x50);
			Assert::AreEqual(size_t(10), node.m_offset);
			Assert::AreEqual(size_t(10), node.m_length);
			Assert::AreEqual(0x28, node.m_leftChild_length);
			Assert::AreEqual(0x78, node.m_rightChild_length);
			Assert::AreEqual(-1, node.m_parent_length);
			Assert::AreEqual(0x28, node.m_leftChild_offset);
			Assert::AreEqual(0x78, node.m_rightChild_offset);
			Assert::AreEqual(-1, node.m_parent_offset);

			node = file.readNode(0x78);
			Assert::AreEqual(size_t(30), node.m_offset);
			Assert::AreEqual(size_t(30), node.m_length);
			Assert::AreEqual(-1, node.m_leftChild_length);
			Assert::AreEqual(-1, node.m_rightChild_length);
			Assert::AreEqual(0x50, node.m_parent_length);
			Assert::AreEqual(-1, node.m_leftChild_offset);
			Assert::AreEqual(-1, node.m_rightChild_offset);
			Assert::AreEqual(0x50, node.m_parent_offset);
		}
		TEST_METHOD(UnusedFileEraseBalance)
		{
			UnusedFile file("UnusedFileEraseBalance.bin", true);
			file.insert(135, 135);
			file.insert(170, 120);
			file.insert(120, 170);
			file.insert(150, 87);
			file.insert(190, 130);
			file.insert(87,150);
			file.insert(130,190);
			
			
			size_t offset, length;
			file.getEnoughPlace(120,offset,length);
			Assert::AreEqual(size_t(135), offset);
			Assert::AreEqual(size_t(135), length);
			file.getEnoughPlace(120, offset, length);
			Assert::AreEqual(size_t(190), offset);
			Assert::AreEqual(size_t(130), length);
			file.getEnoughPlace(120, offset, length);
			Assert::AreEqual(size_t(170), offset);
			Assert::AreEqual(size_t(120), length);
			file.getEnoughPlace(87, offset, length);
			Assert::AreEqual(size_t(120), offset);
			Assert::AreEqual(size_t(170), length);
		}

		TEST_METHOD(UnusedFileCombine)
		{
			UnusedFile file("UnusedFileCombine.bin", true);
			file.insert(10, 120);
			file.insert(130, 50);
			file.insert(0, 10);
			file.insert(180, 20);
			file.insert(200, 90);
			file.insert(290, 50);
			Assert::AreEqual(1,file.m_head.nodeSize);
			Assert::AreEqual(size_t(0), file.readNode(file.m_head.rootNodeOffset_length).m_offset);
			Assert::AreEqual(size_t(340), file.readNode(file.m_head.rootNodeOffset_length).m_length);

		}
	};
}