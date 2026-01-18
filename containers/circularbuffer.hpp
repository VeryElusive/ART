#pragma once

#include "../common/memory.hpp"
#include "../common/heap.hpp"

namespace ART
{
	template <typename T, size_t Size>
	class CircularBuffer
	{
	public:
		CircularBuffer() : Head(0) {}

		inline T *Push(const T &Item)
		{
			ART::Memcpy(&Data[Head], &Item, sizeof(T));
			T *justAdded = &Data[Head];

			Head = (Head + 1) & (Size - 1);

			return justAdded;
		}

		inline T *Push(const T *Item)
		{
			if(Item)
			{
				ART::Memcpy(&Data[Head], Item, sizeof(T));

				T *justAdded = &Data[Head];
				Head = (Head + 1) & (Size - 1);
				return justAdded;
			}
			return nullptr;
		}

		inline T &operator[](size_t Index)
		{
			return Data[(Head - 1 - Index) & (Size - 1)];
		}

		inline const T &operator[](size_t Index) const
		{
			return Data[(Head - 1 - Index) & (Size - 1)];
		}

		inline T *Get(size_t Index)
		{
			if(Index >= Size)
			{
				return nullptr;
			}

			return &(*this)[Index];
		}

		struct Iterator
		{
			CircularBuffer &Buffer;
			size_t Index;

			T &operator*() { return Buffer[Index]; }
			T *operator->() { return &Buffer[Index]; }

			Iterator &operator++()
			{
				Index++;
				return *this;
			}

			bool operator==(const Iterator &Other) const { return Index == Other.Index; }
			bool operator!=(const Iterator &Other) const { return Index != Other.Index; }
		};

		Iterator begin() { return Iterator{*this, 0}; }
		Iterator end() { return Iterator{*this, Size}; }

		inline size_t Count() const { return Size; }

	private:
		T Data[Size];
		size_t Head;
	};
}