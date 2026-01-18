#pragma once

#include "vector.hpp"

namespace ART
{
	template <typename T>
	class OrderedMap
	{
	private:
		struct Entry_t
		{
			u64 Key;
			T Value;
		};

		class CValueIterator
		{
		private:
			Entry_t *Current;
		public:
			CValueIterator(Entry_t *entry) : Current(entry) {}

			T &operator*()
			{
				return Current->Value;
			}

			CValueIterator &operator++()
			{
				++Current;
				return *this;
			}

			bool operator!=(const CValueIterator &other) const {
				return Current != other.Current;
			}
		};

	public:
		inline T *Insert(Size_t Key, T Value)
		{
			Size_t InsertIndex = 0;

			if(Table.Count() > 0)
			{
				InsertIndex = GetIndexForKey(Key);

				Entry_t *Existing = Table.Get(InsertIndex);
				if(Existing && Existing->Key == Key)
				{
					ART::Memcpy(&Existing->Value, &Value, sizeof(T));
					return &Existing->Value;
				}
			}

			Entry_t *EntryPTR = Table.Create(InsertIndex);

			if(EntryPTR)
			{
				EntryPTR->Key = Key;
				ART::Memcpy(&EntryPTR->Value, &Value, sizeof(T));
				return &EntryPTR->Value;
			}

			return NULL;
		}

		inline T *Insert(Size_t Key, T *Value)
		{
			Size_t InsertIndex = 0;

			if(Table.Count() > 0)
			{
				InsertIndex = GetIndexForKey(Key);
				Entry_t *Existing = Table.Get(InsertIndex);
				if(Existing && Existing->Key == Key)
				{
					ART::Memcpy(&Existing->Value, Value, sizeof(T));
					return &Existing->Value;
				}
			}

			Entry_t *EntryPTR = Table.Create(InsertIndex);

			if(EntryPTR)
			{
				EntryPTR->Key = Key;
				if(Value) 
				{
					ART::Memcpy(&EntryPTR->Value, Value, sizeof(T));
				}
				return &EntryPTR->Value;
			}

			return NULL;
		}

		inline T *Insert(Size_t Key)
		{
			Size_t InsertIndex = 0;

			if(Table.Count() > 0)
			{
				InsertIndex = GetIndexForKey(Key);

				Entry_t *Existing = Table.Get(InsertIndex);
				if(Existing && Existing->Key == Key)
				{
					return &Existing->Value;
				}
			}

			Entry_t *EntryPTR = Table.Create(InsertIndex);

			if(EntryPTR)
			{
				EntryPTR->Key = Key;
				return &EntryPTR->Value;
			}

			return NULL;
		}

		inline void Resize(Size_t Size)
		{
			Table.Resize(Size);
		}

		inline void Remove(Size_t Key)
		{
			if(Table.Count() == 0)
			{
				return;
			}

			if(Table.Count() == 1)
			{
				Table.PopBack();
			}
			else
			{
				Size_t RemoveIndex = GetIndexForKey(Key);
				auto Data = Table.Get(RemoveIndex);
				if(Data && Data->Key == Key)
				{
					Table.DeleteElement(RemoveIndex);
				}
			}
		}

		inline Size_t Count()
		{
			return Table.Count();
		}

		inline void Destroy(bool FreeMemory = TRUE)
		{
			Table.Destroy(FreeMemory);
		}

		inline Entry_t *GetEntryAtIndex(Size_t Index)
		{
			Entry_t *Entry = Table.Get(Index);
			if(Entry == NULL)
			{
				return NULL;
			}

			return Entry;
		}

		inline T *GetAtIndex(Size_t Index)
		{
			Entry_t *Entry = GetEntryAtIndex(Index);
			if(Entry == NULL)
			{
				return NULL;
			}

			return &Entry->Value;
		}

		inline T *Get(Size_t Key)
		{
			if(Table.Count() == 0)
			{
				return NULL;
			}

			Size_t Index = GetIndexForKey(Key);

			Entry_t *Entry = Table.Get(Index);
			if(Entry && Entry->Key == Key)
			{
				return &Entry->Value;
			}

			return NULL;
		}

		inline void Lock()
		{
			Table.Lock();
		}

		inline void Unlock()
		{
			Table.Unlock();
		}

		CValueIterator begin()
		{
			return CValueIterator(Table.begin());
		}

		CValueIterator end()
		{
			return CValueIterator(Table.end());
		}

	private:
		inline Size_t GetIndexForKey(Size_t Key)
		{
			if(Table.Count() == 0)
			{
				return UNSIGNED_MAX;
			}

			Size_t Low = 0;
			Size_t High = Table.Count();

			while(Low < High)
			{
				Size_t Mid = Low + (High - Low) / 2;
				Entry_t *currentEntry = Table.Get(Mid);

				if(currentEntry->Key < Key)
				{
					Low = Mid + 1;
				}
				else
				{
					High = Mid;
				}
			}

			return Low;
		}

		ART::Vector<Entry_t> Table;
	};
}
