#pragma once

#include "vector.hpp"
#include "../sorting/sort.hpp"
#include "../hash/fnv1a.hpp"

namespace ART
{
	template <typename ValueType>
	class HashMap
	{
	public:
		void Insert(u8 *Key, Size_t KeyLength, ValueType Value)
		{
			Insert(ART::FNV1A::Hash(Key, KeyLength), Value);
		}

		void Remove(u8 *Key, Size_t KeyLength)
		{
			Remove(ART::FNV1A::Hash(Key, KeyLength));
		}
	private:
		void Insert(u64 Key, ValueType Value)
		{
			Entry_t Entry;
			Entry.Key = Key;
			Entry.Value = Value;

			if(Table.Count() <= 1)
			{
				Table.PushBack(Entry);
			}
			else
			{
				Size_t InsertIndex = Partition(0, Table.Count() - 1, Key);
				Table.Insert(InsertIndex, Entry);
			}
		}

		void Remove(u64 Key)
		{
			if(Table.Count() == 0)
			{
				return
			}

			if(Table.Count() == 1)
			{
				Table.PopBack();
			}
			else
			{
				Size_t InsertIndex = Partition(0, Table.Count() - 1, Key);
				Table.DeleteElement(InsertIndex);
			}
		}

		Size_t Partition(Size_t Low, Size_t High, u64 Key)
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
					return Partition(Low, Mid - 1);
				}
			}
			else
			{
				if(Mid < High)
				{
					return Partition(Mid + 1, High);
				}
			}

			return Mid;
		}

		struct Entry_t 
		{
			u64 Key;
			ValueType Value;
		};

		ART::Vector<Entry_t> Table;
		Size_t Size;
		Size_t Capacity;
	};
}