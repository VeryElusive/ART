#include "file.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

#ifdef _WIN32
bool PositionedTransfer(void *Handle, u64 Offset, void *Buffer, Size_t Length, Size_t *Transferred, bool Writing);

bool ART::CPositionedFile::OpenRead(const wchar_t *Path)
{
	Close();
	Handle = CreateFileW(Path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	return Handle != INVALID_HANDLE_VALUE;
}

bool ART::CPositionedFile::CreateNew(const wchar_t *Path)
{
	Close();
	Handle = CreateFileW(Path, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	return Handle != INVALID_HANDLE_VALUE;
}

bool ART::CPositionedFile::ReadAt(u64 Offset, void *Buffer, Size_t Length, Size_t *Transferred)
{
	return PositionedTransfer(Handle, Offset, Buffer, Length, Transferred, FALSE);
}

bool ART::CPositionedFile::WriteAt(u64 Offset, const void *Buffer, Size_t Length)
{
	Size_t Transferred = 0;
	return PositionedTransfer(Handle, Offset, (void *)Buffer, Length, &Transferred, TRUE)
		&& Transferred == Length;
}

bool ART::CPositionedFile::GetSize(u64 *Size)
{
	LARGE_INTEGER Result{};
	if(Size == NULL || !GetFileSizeEx(Handle, &Result) || Result.QuadPart < 0)
	{
		return FALSE;
	}
	*Size = (u64)Result.QuadPart;
	return TRUE;
}

bool ART::CPositionedFile::Flush()
{
	return FlushFileBuffers(Handle) != FALSE;
}

void ART::CPositionedFile::Close()
{
	if(Handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(Handle);
		Handle = INVALID_HANDLE_VALUE;
	}
}
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

bool ART::CFile::Open(const wchar_t *FilePath, EOpenMode Mode)
{
#ifdef _WIN32
	DWORD Access = 0;
	DWORD Creation = 0;

	DWORD Share = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;

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

	File = CreateFileW(
		FilePath,
		Access,
		Share,
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
		return 0;
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

#ifdef _WIN32
bool PositionedTransfer(void *Handle, u64 Offset, void *Buffer, Size_t Length, Size_t *Transferred, bool Writing)
{
	if(Transferred == NULL)
	{
		return FALSE;
	}
	*Transferred = 0;
	if(Handle == INVALID_HANDLE_VALUE || (Length != 0 && Buffer == NULL)
		|| Offset > 0x7FFFFFFFFFFFFFFFULL || Length > 0x7FFFFFFFFFFFFFFFULL - Offset)
	{
		return FALSE;
	}
	if(Length == 0)
	{
		return TRUE;
	}
	HANDLE Event = CreateEventW(NULL, TRUE, FALSE, NULL);
	if(Event == NULL)
	{
		return FALSE;
	}
	bool Success = TRUE;
	while(*Transferred < Length)
	{
		const Size_t Remaining = Length - *Transferred;
		const DWORD Request = (DWORD)(Remaining > 0x40000000 ? 0x40000000 : Remaining);
		const u64 Position = Offset + *Transferred;
		OVERLAPPED Operation{};
		Operation.Offset = (DWORD)Position;
		Operation.OffsetHigh = (DWORD)(Position >> 32);
		Operation.hEvent = Event;
		ResetEvent(Event);
		DWORD Completed = 0;
		const BOOL Started = Writing
			? WriteFile(Handle, (u8 *)Buffer + *Transferred, Request, &Completed, &Operation)
			: ReadFile(Handle, (u8 *)Buffer + *Transferred, Request, &Completed, &Operation);
		DWORD Error = Started ? ERROR_SUCCESS : GetLastError();
		if(Error == ERROR_IO_PENDING)
		{
			Error = GetOverlappedResult(Handle, &Operation, &Completed, TRUE)
				? ERROR_SUCCESS : GetLastError();
		}
		if(!Writing && Error == ERROR_HANDLE_EOF)
		{
			break;
		}
		if(Error != ERROR_SUCCESS)
		{
			Success = FALSE;
			break;
		}
		*Transferred += Completed;
		if(Completed != Request)
		{
			Success = !Writing;
			break;
		}
	}
	CloseHandle(Event);
	return Success;
}
#endif
