#pragma once

#include "../common/decl.hpp"

namespace ART
{
#ifdef _WIN32
	// Independent positioned operations; no shared seek cursor. Close requires
	// callers to have drained their reads. Read opens deny write/delete sharing.
	class CPositionedFile
	{
	public:
		CPositionedFile() : Handle((void *)-1) {}
		CPositionedFile(const CPositionedFile &) = delete;
		CPositionedFile &operator=(const CPositionedFile &) = delete;
		bool OpenRead(const wchar_t *Path);
		bool CreateNew(const wchar_t *Path);
		bool ReadAt(u64 Offset, void *Buffer, Size_t Length, Size_t *Transferred);
		bool WriteAt(u64 Offset, const void *Buffer, Size_t Length);
		bool GetSize(u64 *Size);
		bool Flush();
		void Close();
	private:
		void *Handle;
	};

#endif

	enum EOpenMode
	{
		READ = (1 << 0),
		WRITE = (1 << 1),
		WRITE_APPEND = (1 << 2),
		READ_WRITE = (1 << 3)
	};

	class CFile
	{
	public:
		CFile()
		{
			File = (void *)-1;
		}

		bool Open(const char *FilePath, EOpenMode Mode);
		bool Open(const wchar_t *FilePath, EOpenMode Mode);

		void Close();

		int Read(void *Buffer, Size_t Size);
		int Write(void *Buffer, Size_t Size);

		Size_t GetFileSize();
	private:
#ifdef _WIN32
		void *File;
#else
		int		File;
#endif
	};
}
