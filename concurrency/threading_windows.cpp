#include "threading_platform.hpp"

#include <windows.h>
#include <intrin.h>

namespace
{
	using ThreadEntry_t = ART::Threading::Platform::ThreadEntry_t;
	using CreateThread_t = HANDLE(WINAPI *)(
		LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE,
		LPVOID, DWORD, LPDWORD
	);
	using CreateSemaphoreW_t = HANDLE(WINAPI *)(
		LPSECURITY_ATTRIBUTES, LONG, LONG, LPCWSTR
	);
	using ReleaseSemaphore_t = BOOL(WINAPI *)(HANDLE, LONG, LPLONG);
	using WaitForSingleObject_t = DWORD(WINAPI *)(HANDLE, DWORD);
	using CloseHandle_t = BOOL(WINAPI *)(HANDLE);
	using GetActiveProcessorCount_t = DWORD(WINAPI *)(WORD);
	using WaitOnAddress_t = BOOL(WINAPI *)(volatile VOID *, PVOID, SIZE_T, DWORD);
	using RtlWakeAddressAll_t = VOID(NTAPI *)(PVOID);

	CreateThread_t CreateThreadFn = NULL;
	CreateSemaphoreW_t CreateSemaphoreWFn = NULL;
	ReleaseSemaphore_t ReleaseSemaphoreFn = NULL;
	WaitForSingleObject_t WaitForSingleObjectFn = NULL;
	CloseHandle_t CloseHandleFn = NULL;
	GetActiveProcessorCount_t GetActiveProcessorCountFn = NULL;
	WaitOnAddress_t WaitOnAddressFn = NULL;
	RtlWakeAddressAll_t RtlWakeAddressAllFn = NULL;
	volatile i32 ImportState = 0;
	volatile i32 ActiveProcessorCount = 0;

	struct ThreadStartContext
	{
		ThreadEntry_t Entry;
		void *Context;
	};

	struct SemaphoreStorage
	{
		HANDLE Handle;
	};

	static_assert(sizeof(ThreadStartContext) <= 32, "thread start storage is too small");
	static_assert(alignof(ThreadStartContext) <= 16, "thread start storage is misaligned");
	static_assert(sizeof(SemaphoreStorage) <= 64, "semaphore storage is too small");
	static_assert(alignof(SemaphoreStorage) <= 16, "semaphore storage is misaligned");

	struct UnicodeString
	{
		USHORT Length;
		USHORT MaximumLength;
		wchar_t *Buffer;
	};

	struct PebLdrData
	{
		ULONG Length;
		BOOLEAN Initialized;
		u8 Padding1[3];
		void *SsHandle;
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
	};

	struct LdrDataTableEntry
	{
		LIST_ENTRY InLoadOrderLinks;
		LIST_ENTRY InMemoryOrderLinks;
		LIST_ENTRY InInitializationOrderLinks;
		void *DllBase;
		void *EntryPoint;
		ULONG SizeOfImage;
#ifdef _WIN64
		u8 Padding1[4];
#endif
		UnicodeString FullDllName;
		UnicodeString BaseDllName;
	};

	struct Peb
	{
		u8 Padding1[2];
		u8 BeingDebugged;
		u8 Padding2[1];
		void *Padding3[2];
		PebLdrData *Ldr;
	};

	constexpr u32 HashOffset = 2166136261u;
	constexpr u32 HashPrime = 16777619u;

	constexpr char Lower(const char Character)
	{
		return Character >= 'A' && Character <= 'Z'
			? Character + ('a' - 'A')
			: Character;
	}

	constexpr wchar_t Lower(const wchar_t Character)
	{
		return Character >= L'A' && Character <= L'Z'
			? Character + (L'a' - L'A')
			: Character;
	}

	template<Size_t Length>
	constexpr u32 Hash(const char (&String)[Length])
	{
		u32 Value = HashOffset;
		for(Size_t i = 0; i + 1 < Length; i++)
		{
			Value = (Value ^ (u8)Lower(String[i])) * HashPrime;
		}
		return Value;
	}

	template<Size_t Length>
	constexpr u32 Hash(const wchar_t (&String)[Length])
	{
		u32 Value = HashOffset;
		for(Size_t i = 0; i + 1 < Length; i++)
		{
			Value = (Value ^ (u16)Lower(String[i])) * HashPrime;
		}
		return Value;
	}

	constexpr auto KernelBaseHash = Hash(L"kernelbase.dll");
	constexpr auto Kernel32Hash = Hash(L"kernel32.dll");
	constexpr auto NtdllHash = Hash(L"ntdll.dll");
	constexpr auto CreateThreadHash = Hash("CreateThread");
	constexpr auto CreateSemaphoreWHash = Hash("CreateSemaphoreW");
	constexpr auto ReleaseSemaphoreHash = Hash("ReleaseSemaphore");
	constexpr auto WaitForSingleObjectHash = Hash("WaitForSingleObject");
	constexpr auto CloseHandleHash = Hash("CloseHandle");
	constexpr auto GetActiveProcessorCountHash = Hash("GetActiveProcessorCount");
	constexpr auto WaitOnAddressHash = Hash("WaitOnAddress");
	constexpr auto RtlWakeAddressAllHash = Hash("RtlWakeAddressAll");

	u32 Hash(const char *String)
	{
		u32 Value = HashOffset;
		while(*String != '\0')
		{
			Value = (Value ^ (u8)Lower(*String++)) * HashPrime;
		}
		return Value;
	}

	u32 Hash(const wchar_t *String, const Size_t Length)
	{
		u32 Value = HashOffset;
		for(Size_t i = 0; i < Length; i++)
		{
			Value = (Value ^ (u16)Lower(String[i])) * HashPrime;
		}
		return Value;
	}

	void *GetPebAddress()
	{
#ifdef _WIN64
		return (void *)__readgsqword(0x60);
#else
		return (void *)__readfsdword(0x30);
#endif
	}

	void *GetModuleBase(const u32 ModuleHash)
	{
		const auto ProcessEnvironmentBlock = (Peb *)GetPebAddress();
		if(ProcessEnvironmentBlock == NULL || ProcessEnvironmentBlock->Ldr == NULL)
		{
			return NULL;
		}

		auto Head = &ProcessEnvironmentBlock->Ldr->InMemoryOrderModuleList;
		for(auto Link = Head->Flink; Link != Head; Link = Link->Flink)
		{
			auto Entry = CONTAINING_RECORD(
				Link, LdrDataTableEntry, InMemoryOrderLinks
			);
			if(Entry->BaseDllName.Buffer == NULL)
			{
				continue;
			}

			if(Hash(
				Entry->BaseDllName.Buffer,
				Entry->BaseDllName.Length / sizeof(wchar_t)
			) == ModuleHash)
			{
				return Entry->DllBase;
			}
		}

		return NULL;
	}

	void *GetModuleExport(const void *ModuleBase, const u32 ExportHash)
	{
		if(ModuleBase == NULL)
		{
			return NULL;
		}

		auto Base = (u8 *)ModuleBase;
		auto DosHeader = (IMAGE_DOS_HEADER *)Base;
		if(DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		{
			return NULL;
		}

		auto NtHeaders = (IMAGE_NT_HEADERS *)(Base + DosHeader->e_lfanew);
		if(NtHeaders->Signature != IMAGE_NT_SIGNATURE)
		{
			return NULL;
		}

		const auto &Directory = NtHeaders->OptionalHeader.DataDirectory[
			IMAGE_DIRECTORY_ENTRY_EXPORT
		];
		if(Directory.VirtualAddress == 0 || Directory.Size == 0)
		{
			return NULL;
		}

		auto Exports = (IMAGE_EXPORT_DIRECTORY *)(Base + Directory.VirtualAddress);
		auto Names = (DWORD *)(Base + Exports->AddressOfNames);
		auto Ordinals = (WORD *)(Base + Exports->AddressOfNameOrdinals);
		auto Functions = (DWORD *)(Base + Exports->AddressOfFunctions);

		for(DWORD i = 0; i < Exports->NumberOfNames; i++)
		{
			auto Name = (char *)(Base + Names[i]);
			if(Hash(Name) != ExportHash)
			{
				continue;
			}

			const auto FunctionRva = Functions[Ordinals[i]];
			if(FunctionRva >= Directory.VirtualAddress
				&& FunctionRva < Directory.VirtualAddress + Directory.Size)
			{
				return NULL;
			}

			return Base + FunctionRva;
		}

		return NULL;
	}

	bool ResolveImports()
	{
		const auto ExistingState = _InterlockedCompareExchange(
			(volatile long *)&ImportState, 1, 0
		);
		if(ExistingState != 0)
		{
			while(ImportState == 1)
			{
				_mm_pause();
			}
			return ImportState == 2;
		}

		const auto KernelBase = GetModuleBase(KernelBaseHash);
		const auto Kernel32 = GetModuleBase(Kernel32Hash);
		const auto Ntdll = GetModuleBase(NtdllHash);

		CreateThreadFn = (CreateThread_t)GetModuleExport(
			KernelBase, CreateThreadHash
		);
		CreateSemaphoreWFn = (CreateSemaphoreW_t)GetModuleExport(
			KernelBase, CreateSemaphoreWHash
		);
		ReleaseSemaphoreFn = (ReleaseSemaphore_t)GetModuleExport(
			KernelBase, ReleaseSemaphoreHash
		);
		WaitForSingleObjectFn = (WaitForSingleObject_t)GetModuleExport(
			KernelBase, WaitForSingleObjectHash
		);
		CloseHandleFn = (CloseHandle_t)GetModuleExport(
			KernelBase, CloseHandleHash
		);
		GetActiveProcessorCountFn = (GetActiveProcessorCount_t)GetModuleExport(
			Kernel32, GetActiveProcessorCountHash
		);
		WaitOnAddressFn = (WaitOnAddress_t)GetModuleExport(
			KernelBase, WaitOnAddressHash
		);
		RtlWakeAddressAllFn = (RtlWakeAddressAll_t)GetModuleExport(
			Ntdll, RtlWakeAddressAllHash
		);

		const auto Success = CreateThreadFn != NULL
			&& CreateSemaphoreWFn != NULL
			&& ReleaseSemaphoreFn != NULL
			&& WaitForSingleObjectFn != NULL
			&& CloseHandleFn != NULL
			&& GetActiveProcessorCountFn != NULL
			&& WaitOnAddressFn != NULL
			&& RtlWakeAddressAllFn != NULL;

		_InterlockedExchange(
			(volatile long *)&ImportState, Success ? 2 : -1
		);
		return Success;
	}

	DWORD WINAPI WindowsThreadEntry(void *Parameter)
	{
		auto StartContext = (ThreadStartContext *)Parameter;
		StartContext->Entry(StartContext->Context);
		return 0;
	}
}

bool ART::Threading::Platform::Initialize()
{
	return ResolveImports();
}

Size_t ART::Threading::Platform::GetProcessorCount()
{
	const auto CachedCount = AtomicCompareExchange(&ActiveProcessorCount, 0, 0);
	if(CachedCount != 0)
	{
		return (Size_t)CachedCount;
	}
	if(ResolveImports() == false)
	{
		return 1;
	}

	const auto Count = GetActiveProcessorCountFn(ALL_PROCESSOR_GROUPS);
	const auto ValidCount = Count == 0 ? 1 : (i32)Count;
	AtomicCompareExchange(&ActiveProcessorCount, ValidCount, 0);
	return (Size_t)AtomicCompareExchange(&ActiveProcessorCount, 0, 0);
}

i32 ART::Threading::Platform::AtomicCompareExchange(
	volatile i32 *Address, const i32 Exchange, const i32 Comparand
)
{
	return (i32)_InterlockedCompareExchange(
		(volatile long *)Address, (long)Exchange, (long)Comparand
	);
}

i32 ART::Threading::Platform::AtomicExchange(
	volatile i32 *Address, const i32 Value
)
{
	return (i32)_InterlockedExchange((volatile long *)Address, (long)Value);
}

i32 ART::Threading::Platform::AtomicIncrement(volatile i32 *Address)
{
	return (i32)_InterlockedIncrement((volatile long *)Address);
}

i32 ART::Threading::Platform::AtomicDecrement(volatile i32 *Address)
{
	return (i32)_InterlockedDecrement((volatile long *)Address);
}

void ART::Threading::Platform::Pause()
{
	_mm_pause();
}

bool ART::Threading::Platform::CreateThread(
	Ptr_t *Thread, void *StartContextStorage,
	const ThreadEntry_t Entry, void *Context
)
{
	if(Thread == NULL || StartContextStorage == NULL || Entry == NULL
		|| ResolveImports() == false)
	{
		return false;
	}

	auto StartContext = (ThreadStartContext *)StartContextStorage;
	StartContext->Entry = Entry;
	StartContext->Context = Context;
	const auto Handle = CreateThreadFn(
		NULL, 0, WindowsThreadEntry, StartContext, 0, NULL
	);
	if(Handle == NULL)
	{
		return false;
	}
	*Thread = (Ptr_t)Handle;
	return true;
}

void ART::Threading::Platform::JoinThread(const Ptr_t Thread)
{
	if(Thread == 0 || ResolveImports() == false)
	{
		return;
	}
	WaitForSingleObjectFn((HANDLE)Thread, INFINITE);
	CloseHandleFn((HANDLE)Thread);
}

bool ART::Threading::Platform::InitializeSemaphore(
	void *Storage, const i32 InitialCount, const i32 MaximumCount
)
{
	if(Storage == NULL || InitialCount < 0 || MaximumCount <= 0
		|| InitialCount > MaximumCount || ResolveImports() == false)
	{
		return false;
	}

	auto Semaphore = (SemaphoreStorage *)Storage;
	Semaphore->Handle = CreateSemaphoreWFn(
		NULL, (LONG)InitialCount, (LONG)MaximumCount, NULL
	);
	return Semaphore->Handle != NULL;
}

bool ART::Threading::Platform::WaitSemaphore(
	void *Storage, const bool Block
)
{
	if(Storage == NULL || WaitForSingleObjectFn == NULL)
	{
		return false;
	}
	const auto Semaphore = (SemaphoreStorage *)Storage;
	return WaitForSingleObjectFn(
		Semaphore->Handle, Block ? INFINITE : 0
	) == WAIT_OBJECT_0;
}

bool ART::Threading::Platform::ReleaseSemaphore(
	void *Storage, const i32 Count
)
{
	if(Storage == NULL || Count <= 0 || ReleaseSemaphoreFn == NULL)
	{
		return false;
	}
	const auto Semaphore = (SemaphoreStorage *)Storage;
	return ReleaseSemaphoreFn(
		Semaphore->Handle, (LONG)Count, NULL
	) != FALSE;
}

void ART::Threading::Platform::DestroySemaphore(void *Storage)
{
	if(Storage == NULL || CloseHandleFn == NULL)
	{
		return;
	}
	auto Semaphore = (SemaphoreStorage *)Storage;
	if(Semaphore->Handle != NULL)
	{
		CloseHandleFn(Semaphore->Handle);
		Semaphore->Handle = NULL;
	}
}

void ART::Threading::Platform::WaitAddress(
	volatile i32 *Address, const i32 Value
)
{
	if(WaitOnAddressFn != NULL)
	{
		WaitOnAddressFn(
			(volatile void *)Address, (void *)&Value, sizeof(Value), INFINITE
		);
	}
	else
	{
		Pause();
	}
}

void ART::Threading::Platform::WakeAddressAll(volatile i32 *Address)
{
	if(RtlWakeAddressAllFn != NULL)
	{
		RtlWakeAddressAllFn((void *)Address);
	}
}
