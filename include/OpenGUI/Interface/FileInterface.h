#pragma once

namespace OGUI
{
    using FileHandle = void*;
    
	struct FileInterface
	{
		virtual ~FileInterface();
		virtual FileHandle Open(const char* path);
		virtual void Close(FileHandle file);
		virtual size_t Read(void* buffer, size_t size, FileHandle file);
		virtual int Seek(FileHandle stream, size_t offset, int fromwhere);
		virtual size_t Tell(const FileHandle file);
		virtual size_t Length(const FileHandle file);
	};
}