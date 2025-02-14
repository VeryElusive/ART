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
				Size_t InsertIndex = Partition(0, Table.Count() - 1, Key);
				EntryPTR = Table.Insert(InsertIndex, Entry);
			}

			if(EntryPTR != NULL)
			{
				return &EntryPTR->Value;
			}

			return NULL;
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
				Size_t RemoveIndex = Partition(0, Table.Count() - 1, Key);
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

		T *GetAtIndex(Size_t Index)
		{
			Entry_t *Entry = Table.Get(Index);
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

			Size_t Index = Partition(0, Table.Count() - 1, Key);

			Entry_t *Entry = Table.Get(Index);
			if(Entry->Key == Key)
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
		Size_t Partition(Size_t Low, Size_t High, Size_t Key)
		{
			Size_t Mid = Low + (High - Low) / 2;

			if(Table.Get(Mid)->Key == Key)
			{
				return Mid;
			}
			else if(Table.Get(Mid)->Key > Key)
			{
				if(Low < Mid)
				{
					return Partition(Low, Mid - 1, Key);
				}
			}
			else
			{
				if(Mid < High)
				{
					return Partition(Mid + 1, High, Key);
				}
			}

			return Mid;
		}

		ART::Vector<Entry_t> Table;
	};
}