#pragma once

#include "../common/decl.hpp"

namespace ART
{
	namespace Threading
	{
		class Mutex
		{
		public:
			constexpr Mutex()
				: State(0)
			{
			}
			Mutex(const Mutex &) = delete;
			Mutex &operator=(const Mutex &) = delete;

			void Lock();
			bool TryLock();
			void Unlock();

		private:
			volatile i32 State;
		};

		class SharedMutex
		{
		public:
			constexpr SharedMutex()
				: State(0), WaitingWriters(0)
			{
			}
			SharedMutex(const SharedMutex &) = delete;
			SharedMutex &operator=(const SharedMutex &) = delete;

			void Lock();
			bool TryLock();
			void Unlock();
			void LockShared();
			bool TryLockShared();
			void UnlockShared();

		private:
			volatile i32 State;
			volatile i32 WaitingWriters;
		};

		template<typename MutexType>
		class LockGuard
		{
		public:
			explicit LockGuard(MutexType &MutexObject)
				: MutexObject(MutexObject)
			{
				MutexObject.Lock();
			}

			~LockGuard()
			{
				MutexObject.Unlock();
			}

			LockGuard(const LockGuard &) = delete;
			LockGuard &operator=(const LockGuard &) = delete;

		private:
			MutexType &MutexObject;
		};

		template<typename MutexType>
		class UniqueLock
		{
		public:
			UniqueLock()
				: MutexObject(NULL), OwnsMutex(false)
			{
			}

			explicit UniqueLock(MutexType &MutexObject)
				: MutexObject(&MutexObject), OwnsMutex(false)
			{
				Lock();
			}

			~UniqueLock()
			{
				if(OwnsMutex)
				{
					MutexObject->Unlock();
				}
			}

			UniqueLock(const UniqueLock &) = delete;
			UniqueLock &operator=(const UniqueLock &) = delete;

			UniqueLock(UniqueLock &&Other)
				: MutexObject(Other.MutexObject), OwnsMutex(Other.OwnsMutex)
			{
				Other.MutexObject = NULL;
				Other.OwnsMutex = false;
			}

			UniqueLock &operator=(UniqueLock &&Other)
			{
				if(this == &Other)
				{
					return *this;
				}
				if(OwnsMutex)
				{
					MutexObject->Unlock();
				}
				MutexObject = Other.MutexObject;
				OwnsMutex = Other.OwnsMutex;
				Other.MutexObject = NULL;
				Other.OwnsMutex = false;
				return *this;
			}

			void Lock()
			{
				if(MutexObject != NULL && OwnsMutex == false)
				{
					MutexObject->Lock();
					OwnsMutex = true;
				}
			}

			bool TryLock()
			{
				if(MutexObject == NULL || OwnsMutex)
				{
					return false;
				}
				OwnsMutex = MutexObject->TryLock();
				return OwnsMutex;
			}

			void Unlock()
			{
				if(OwnsMutex)
				{
					MutexObject->Unlock();
					OwnsMutex = false;
				}
			}

			bool OwnsLock() const
			{
				return OwnsMutex;
			}

			explicit operator bool() const
			{
				return OwnsMutex;
			}

			MutexType *Release()
			{
				auto Result = MutexObject;
				MutexObject = NULL;
				OwnsMutex = false;
				return Result;
			}

		private:
			MutexType *MutexObject;
			bool OwnsMutex;
		};

		template<typename MutexType>
		class SharedLock
		{
		public:
			SharedLock()
				: MutexObject(NULL), OwnsMutex(false)
			{
			}

			explicit SharedLock(MutexType &MutexObject)
				: MutexObject(&MutexObject), OwnsMutex(false)
			{
				Lock();
			}

			~SharedLock()
			{
				if(OwnsMutex)
				{
					MutexObject->UnlockShared();
				}
			}

			SharedLock(const SharedLock &) = delete;
			SharedLock &operator=(const SharedLock &) = delete;

			SharedLock(SharedLock &&Other)
				: MutexObject(Other.MutexObject), OwnsMutex(Other.OwnsMutex)
			{
				Other.MutexObject = NULL;
				Other.OwnsMutex = false;
			}

			SharedLock &operator=(SharedLock &&Other)
			{
				if(this == &Other)
				{
					return *this;
				}
				if(OwnsMutex)
				{
					MutexObject->UnlockShared();
				}
				MutexObject = Other.MutexObject;
				OwnsMutex = Other.OwnsMutex;
				Other.MutexObject = NULL;
				Other.OwnsMutex = false;
				return *this;
			}

			void Lock()
			{
				if(MutexObject != NULL && OwnsMutex == false)
				{
					MutexObject->LockShared();
					OwnsMutex = true;
				}
			}

			bool TryLock()
			{
				if(MutexObject == NULL || OwnsMutex)
				{
					return false;
				}
				OwnsMutex = MutexObject->TryLockShared();
				return OwnsMutex;
			}

			void Unlock()
			{
				if(OwnsMutex)
				{
					MutexObject->UnlockShared();
					OwnsMutex = false;
				}
			}

			bool OwnsLock() const
			{
				return OwnsMutex;
			}

			explicit operator bool() const
			{
				return OwnsMutex;
			}

			MutexType *Release()
			{
				auto Result = MutexObject;
				MutexObject = NULL;
				OwnsMutex = false;
				return Result;
			}

		private:
			MutexType *MutexObject;
			bool OwnsMutex;
		};
	}
}
