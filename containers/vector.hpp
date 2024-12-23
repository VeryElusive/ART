#pragma once

#include "../common/memory.hpp"

namespace ART
{
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
		explicit Vector(size_t InitialCapacity);

		~Vector();

		void Destroy();

		size_t Count() { return ElementCount; }
		T *Get(size_t Index);

		bool Resize(size_t NewSize);

		bool DeleteElement(size_t Index);
		bool PopBack();
		bool PopFront();

		T *PushBack(T *NewElement);
		T *PushBack(T NewElement);

		T *Insert(size_t Index, T NewElement);
		T *Insert(size_t Index, T *NewElement);

	private:
		T		*Data;

		size_t	ReservedCount;
		size_t	ElementCount;
	};
}