#pragma once
#include <fstream>
#include "../MyString/MyString.h"
using std::fstream;

namespace Sdalin
{
	class FileBase
	{
	public:
		FileBase() {}

		FileBase(const String fileName, const bool trunc = false)
			: stream(fileName.c_str(), fstream::in | fstream::out | fstream::binary | (trunc ? fstream::trunc : NULL))
		{

		}

		virtual ~FileBase() {}

		bool open(String fileName, const bool trunc = false)
		{
			stream.open(fileName.c_str(), fstream::in | fstream::out | fstream::binary | (trunc ? fstream::trunc : NULL));
		}

		bool write(const size_t offsetFromFileStart, const char *buffer, const size_t bytesToWrite)
		{
			stream.seekp(offsetFromFileStart);
			stream.write(buffer, bytesToWrite);
			if (stream.bad())
				return false;
			return true;
		}

		bool read(const size_t offsetFromFileStart, char *buffer, const size_t bytesToRead)
		{
			stream.seekg(offsetFromFileStart);
			stream.read(buffer, bytesToRead);
			if (stream.fail() || stream.eof())
				return false;
			return true;
		}


	protected:
		fstream stream;
	};
	class UsedFile : public FileBase
	{
	public:
		UsedFile(String fileName, const bool trunc = false)
			: FileBase(fileName, trunc)
		{

		}
	};
	class UnusedFile : public FileBase
	{
	public:
		UnusedFile(String fileName, const bool trunc = false)
			: FileBase(fileName, trunc)
		{

		}
	};
	class File : public FileBase
	{
	public:
		File() = delete;
		File(String fileName, const bool trunc = false)
			: FileBase(fileName, trunc),
			m_used_file(String("used") + fileName.c_str(), trunc),
			m_unused_file(String("used") + fileName.c_str(), trunc)
		{

		}
	private:
		UsedFile m_used_file;
		UnusedFile m_unused_file;
	};
}
