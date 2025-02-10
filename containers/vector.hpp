#pragma once

#include "../common/memory.hpp"
#include "../common/heap.hpp"
#include "../concurrency/spinlock.hpp"

namespace ART
{
	// TODO: decide if we should spinlock for every operation.
	template <typename T>
	class Vector
	{
	public:
		Vector()
		{
			ReservedCount = 0;
			ElementCount = 0;
			Data = NULL;
		};
		Size_t Count() { return ElementCount; }

		void Lock() { Spinlock.Lock(); };
		void Unlock() { Spinlock.Unlock(); };

		Vector(Size_t InitialCapacity)
		{
			ReservedCount = InitialCapacity;
			ElementCount = 0;
			if(InitialCapacity != 0)
			{
				Data = (T *)ART::Alloc(InitialCapacity * sizeof(T));
			}
		}

		void Destroy(bool FreeMemory = TRUE)
		{
			ElementCount = 0;

			if(FreeMemory == TRUE)
			{
				ReservedCount = 0;
				if(Data)
				{
					ART::Free((void *)Data);
					Data = NULL;
				}
			}
		}

		T *Get(Size_t Index)
		{
			if(Index >= ElementCount)
			{
				return NULL;
			}

			return &Data[Index];
		}

		// just for quick iteration.
		T *begin() 
		{
			return Data;
		}

		T *end() 
		{
			return (T *)((Ptr_t)Data + (Ptr_t)(ElementCount * sizeof(T)));
		}

		/// <summary>
		/// Resizes the data to a particular size.
		/// </summary>
		/// <param name="NewSize"></param>
		/// <returns>true if was successful, false if unsuccessful</returns>

		bool Resize(Size_t NewSize)
		{
			T *NewData = (T *)ART::Realloc((void *)Data, NewSize * sizeof(T));
			if(NewData != NULL)
			{
				ReservedCount = NewSize;
				Data = NewData;
				return TRUE;
			}

			return FALSE;
		}

		/// <summary>
		/// Allocates new memory and inserts it to the end of the data.
		/// </summary>
		/// <param name="NewElement">New element to insert</param>
		/// <returns>Pointer to the data in vector memory. Returns NULL if ran out of memory.</returns>
		T *PushBack(T *NewElement)
		{
			if(ElementCount >= ReservedCount)
			{
				if(Resize(ElementCount + 1) == FALSE)
				{
					return NULL;
				}
			}

			ART::Memcpy
			(
				&Data[ElementCount],
				NewElement,
				sizeof(T)
			);

			ElementCount++;

			return &Data[ElementCount - 1];
		}


		T *PushBack(T NewElement)
		{
			return PushBack(&NewElement);
		}

		/// <summary>
		/// Allocates new memory, inserts element to data at a given index and shifts further elements to higher indexes
		/// </summary>
		/// <param name="Index">Index to insert to</param>
		/// <param name="NewElement">New element to insert</param>
		/// <returns>Pointer to the data in vector memory. Returns NULL if ran out of memory.</returns>
		T *Insert(Size_t Index, T *NewElement)
		{
			if(Index > ElementCount)
			{
				return NULL;
			}

			if(ElementCount >= ReservedCount)
			{
				if(Resize(ElementCount + 1) == FALSE)
				{
					return NULL;
				}
			}

			if(Index == ElementCount)
			{
				return PushBack(NewElement);
			}

			ART::Memmove
			(
				&Data[Index + 1],
				&Data[Index],
				(ElementCount - Index) * sizeof(T)
			);

			ART::Memcpy
			(
				&Data[Index],
				NewElement,
				sizeof(T)
			);

			ElementCount++;

			return &Data[Index];
		}


		T *Insert(Size_t Index, T NewElement)
		{
			return Insert(Index, &NewElement);
		}

		/// <summary>
		/// Deletes the element of the vector with the given index.
		/// </summary>
		/// <param name="Index">Index of element to delete.</param>
		/// <returns> True if successful, false if unsuccessful. Will fail if ElementCount is 0, or index is above ElementCount.</returns>
		bool DeleteElement(Size_t Index)
		{
			if(ElementCount == 0 || Index >= ElementCount || Data == NULL)
			{
				return FALSE;
			}

			if(ElementCount != 1 && Index != ElementCount - 1)
			{
				ART::Memmove
				(
					&Data[Index],
					&Data[Index + 1],
					(ElementCount - Index - 1) * sizeof(T)
				);
			}

			ElementCount--;

			return TRUE;
		}

		/// <summary>
		/// Deletes the backmost element of the vector.
		/// </summary>
		/// <returns> True if successful, false if unsuccessful. Will fail if ElementCount is 0.</returns>
		bool PopBack()
		{
			return DeleteElement(ElementCount - 1);
		}

		/// <summary>
		/// Deletes the frontmost element of the vector.
		/// </summary>
		/// <returns> True if successful, false if unsuccessful. Will fail if ElementCount is 0.</returns>
		bool PopFront()
		{
			return DeleteElement(0);
		}

		T *GetData()
		{
			return Data;
		}

	private:
		T *Data;

		Size_t	ReservedCount;
		Size_t	ElementCount;

		ART::Spinlock Spinlock;
	};
}
