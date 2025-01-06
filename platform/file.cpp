#include "file.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

bool ART::CFile::Open(const char *FilePath, EOpenMode Mode)
{
#ifdef _WIN32
	DWORD Access = 0;
	DWORD Creation = 0;

	switch(Mode)
	{
	case EOpenMode::READ:
		Access = GENERIC_READ;
		Creation = OPEN_EXISTING;
		break;
	case EOpenMode::WRITE:
		Access = GENERIC_WRITE;
		Creation = CREATE_ALWAYS;
		break;
	case EOpenMode::WRITE_APPEND:
		Access = FILE_APPEND_DATA;
		Creation = OPEN_ALWAYS;
		break;
	case EOpenMode::READ_WRITE:
		Access = GENERIC_READ | GENERIC_WRITE;
		Creation = OPEN_ALWAYS;
		break;
	}

	File = CreateFileA(
		FilePath,
		Access,
		0,
		NULL,
		Creation,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	return File != (void *)INVALID_HANDLE_VALUE;
#else
	int Flags = 0;

	switch(Mode)
	{
	case EOpenMode::READ:
		Flags = O_RDONLY;
		break;
	case EOpenMode::WRITE:
		Flags = O_WRONLY | O_CREAT | O_TRUNC;
		break;
	case EOpenMode::WRITE_APPEND:
		Flags = O_WRONLY | O_CREAT | O_APPEND;
		break;
	case EOpenMode::READ_WRITE:
		Flags = O_RDWR | O_CREAT;
		break;
	}

	File = open(FilePath, Flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	return File != -1;
#endif
}

void ART::CFile::Close()
{
#ifdef _WIN32
	if(File != (void *)INVALID_HANDLE_VALUE)
	{
		CloseHandle(File);
		File = (void *)INVALID_HANDLE_VALUE;
	}
#else
	if(File != -1)
	{
		close(File);
		File = -1;
	}
#endif
}

int ART::CFile::Read(void *Buffer, Size_t Size)
{
#ifdef _WIN32
	DWORD BytesRead = 0;
	if(ReadFile(File, Buffer, Size, &BytesRead, NULL))
	{
		return BytesRead;
	}
	return -1;
#else
	return read(File, Buffer, Size);
#endif
}

int ART::CFile::Write(void *Buffer, Size_t Size)
{
#ifdef _WIN32
	DWORD BytesWritten = 0;
	if(WriteFile(File, Buffer, Size, &BytesWritten, NULL))
	{
		return BytesWritten;
	}

	return -1;
#else
	return write(File, Buffer, Size);
#endif
}

Size_t ART::CFile::GetFileSize()
{
#ifdef _WIN32
	if(File == (void *)INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	LARGE_INTEGER size;
	if(GetFileSizeEx(File, &size))
	{
		return (Size_t)(size.QuadPart);
	}

	return -1;
#else
	if(FileDescriptor == -1)
	{
		return -1;
	}

	struct stat FileStat;
	if(fstat(File, &FileStat) == 0)
	{
		return (Size_t)(FileStat.st_size);
	}
	return -1;
#endif
}