#include "File.h"
namespace Sdalin
{
	struct FileInfo
	{
		size_t fileSize;			// 0x00 文件大小
		size_t offsetBegin;			// 0x04 本文件开始位置
		int hasNextFile;			// 0x08 是否有下一个文件
		int hasPreFile;				// 0x0C 是否有上一个文件
		char nextFileName[0x20];	// 0x10 下一个文件相对目录
		char preFileName[0x20];		// 0x30 上一个文件相对目录
		size_t offsetUsed;			// 0x50 已用节点偏移，在未用节点前
		size_t offsetUnused;		// 0x54 未用节点偏移，在已用节点后
		int reserved[2];			// 0x58 保留
									//void *data;				// 0x60 数据
	};


	File::File()
	{
	}

	File::~File()
	{
	}
}
