#include "File.h"
namespace Sdalin
{
	struct FileInfo
	{
		size_t fileSize;			// 0x00 �ļ���С
		size_t offsetBegin;			// 0x04 ���ļ���ʼλ��
		int hasNextFile;			// 0x08 �Ƿ�����һ���ļ�
		int hasPreFile;				// 0x0C �Ƿ�����һ���ļ�
		char nextFileName[0x20];	// 0x10 ��һ���ļ����Ŀ¼
		char preFileName[0x20];		// 0x30 ��һ���ļ����Ŀ¼
		size_t offsetUsed;			// 0x50 ���ýڵ�ƫ�ƣ���δ�ýڵ�ǰ
		size_t offsetUnused;		// 0x54 δ�ýڵ�ƫ�ƣ������ýڵ��
		int reserved[2];			// 0x58 ����
									//void *data;				// 0x60 ����
	};


	File::File()
	{
	}

	File::~File()
	{
	}
}
