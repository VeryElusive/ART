#pragma once

#include "../common/decl.hpp"

namespace ART
{
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