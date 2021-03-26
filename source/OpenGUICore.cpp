#include "OpenGUI/Interface/Interfaces.h"

namespace OGUI
{
RenderInterface::~RenderInterface()
{
    
}

InputInterface::~InputInterface()
{

}

SystemInterface::~SystemInterface()
{

}

FileInterface::~FileInterface()
{

}

FileHandle FileInterface::Open(const char* path)
{
    return ::fopen(path, "rb");
}

void FileInterface::Close(FileHandle file)
{
    fclose((FILE*) file);
}

size_t FileInterface::Read(void* buffer, size_t size, FileHandle file)
{
    return fread(buffer, 1, size, (FILE*) file);
}

int FileInterface::Seek(FileHandle file, size_t offset, int fromwhere)
{
    return fseek((FILE*)file, offset, fromwhere);
}

size_t FileInterface::Tell(const FileHandle file)
{
    return ftell((FILE*) file);
}

size_t FileInterface::Length(const FileHandle file)
{
    size_t current_position = Tell(file);
    Seek( file, 0, SEEK_END);
    size_t length = Tell( file);
    Seek( file, (long)current_position, SEEK_SET);
    return length;
}
}