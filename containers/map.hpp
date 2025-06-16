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
		T *Insert(Size_t Key, T Value)
		{
			Entry_t Entry{Key, Value};
			Entry_t *EntryPTR;

			if(Table.Count() == 0)
			{
				EntryPTR = Table.PushBack(Entry);
			}
			else
			{
				Size_t InsertIndex = GetIndexForKey(Key);
				EntryPTR = Table.Insert(InsertIndex, Entry);
			}

			return EntryPTR ? &EntryPTR->Value : NULL;
		}

		void Resize(Size_t Size)
		{
			Table.Resize(Size);
		}

		void Remove(Size_t Key)
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
				if(Table.Get(RemoveIndex)->Key == Key)
				{
					Table.DeleteElement(RemoveIndex);
				}
			}
		}

		Size_t Count()
		{
			return Table.Count();
		}

		void Destroy(bool FreeMemory = TRUE)
		{
			Table.Destroy(FreeMemory);
		}

		Entry_t *GetEntryAtIndex(Size_t Index)
		{
			Entry_t *Entry = Table.Get(Index);
			if(Entry == NULL)
			{
				return NULL;
			}

			return Entry;
		}

		T *GetAtIndex(Size_t Index)
		{
			Entry_t *Entry = GetEntryAtIndex(Index);
			if(Entry == NULL)
			{
				return NULL;
			}

			return &Entry->Value;
		}

		T *Get(Size_t Key)
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

		void Lock()
		{
			Table.Lock();
		}

		void Unlock()
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
		Size_t GetIndexForKey(Size_t Key) 
		{
			if(Table.Count() == 0)
			{
				return NULL;
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