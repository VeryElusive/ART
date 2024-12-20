#include "vector.hpp"

#include "../common/heap.hpp"
#include "../common/memory.hpp"

namespace ART
{
	template<typename T>
	Vector<T>::Vector(size_t InitialCapacity)
	{
		ReservedCount	= InitialCapacity;
		ElementCount	= 0;
		if(InitialCapacity != 0)
		{
			Data = (T *)Alloc(InitialCapacity * sizeof(T));
		}
	}

	template<typename T>
	void ART::Vector<T>::Destroy()
	{
		Free(Data);
	}

	template<typename T>
	ART::Vector<T>::~Vector()
	{
		Destroy();
	}

	template<typename T>
	T *ART::Vector<T>::Get(size_t Index)
	{
		if(Index >= ElementCount)
		{
			return NULL;
		}

		return &Data[Index];
	}

	/// <summary>
	/// Resizes the data to a particular size.
	/// </summary>
	/// <param name="NewSize"></param>
	/// <returns>true if was successful, false if unsuccessful</returns>
	template<typename T>
	bool ART::Vector<T>::Resize(size_t NewSize)
	{
		T *NewData = (T *)Realloc((void *)Data, NewSize * sizeof(T));
		if(NewData != NULL)
		{
			ReservedCount	= NewSize;
			Data			= NewData;
			return TRUE;
		}

		return FALSE;
	}

	/// <summary>
	/// Allocates new memory and inserts it to the end of the data.
	/// </summary>
	/// <param name="NewElement">New element to insert</param>
	/// <returns>Pointer to the data in vector memory. Returns NULL if ran out of memory.</returns>
	template<typename T>
	T *ART::Vector<T>::PushBack(T *NewElement)
	{
		if(ElementCount >= ReservedCount)
		{
			if(Resize(ElementCount + 1))
			{
				return NULL
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

	template<typename T>
	T *ART::Vector<T>::PushBack(T NewElement)
	{
		PushBack(&NewElement);
	}

	/// <summary>
	/// Allocates new memory, inserts element to data at a given index and shifts further elements to higher indexes
	/// </summary>
	/// <param name="Index">Index to insert to</param>
	/// <param name="NewElement">New element to insert</param>
	/// <returns>Pointer to the data in vector memory. Returns NULL if ran out of memory.</returns>
	template<typename T>
	T *ART::Vector<T>::Insert(size_t Index, T *NewElement)
	{
		if(Index > ElementCount) 
		{
			return NULL; 
		}

		if(ElementCount >= ReservedCount) 
		{
			if(Resize(ElementCount + 1))
			{
				return NULL
			}
		}

		ART::Memmove
		(
			&Data[Index + 1],
			&Data[Index],
			(ElementCount - Index - 1) * sizeof(T)
		);

		ART::Memcpy
		(
			&Data[Index],
			NewElement,
			sizeof(T)
		);

		ElementCount++;

		return &Data[index];
	}

	template<typename T>
	T *ART::Vector<T>::Insert(size_t Index, T NewElement)
	{
		return Insert(Index, &NewElement);
	}

	/// <summary>
	/// Deletes the element of the vector with the given index.
	/// </summary>
	/// <param name="Index">Index of element to delete.</param>
	/// <returns> True if successful, false if unsuccessful. Will fail if ElementCount is 0, or index is above ElementCount.</returns>
	template<typename T>
	bool ART::Vector<T>::DeleteElement(size_t Index)
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
	template<typename T>
	bool ART::Vector<T>::PopBack()
	{
		return DeleteElement(ElementCount - 1);
	}	
	
	/// <summary>
	/// Deletes the frontmost element of the vector.
	/// </summary>
	/// <returns> True if successful, false if unsuccessful. Will fail if ElementCount is 0.</returns>
	template<typename T>
	bool ART::Vector<T>::PopFront()
	{
		return DeleteElement(0);
	}
}