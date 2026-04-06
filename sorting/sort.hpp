// sort.hpp
#pragma once
#include "../common/decl.hpp"

namespace ART
{
	namespace Detail
	{
		template<typename T>
		void Swap(T &a, T &b)
		{
			T tmp = a;
			a = b;
			b = tmp;
		}

		template<typename T>
		Size_t Partition(T Arr[], Size_t Size)
		{
			T pivot = Arr[Size - 1];
			int i = -1;
			for(int j = 0; j < Size - 1; j++)
			{
				if(Arr[j] <= pivot)
				{
					i++;
					Detail::Swap(Arr[i], Arr[j]);
				}
			}
			Detail::Swap(Arr[i + 1], Arr[Size - 1]);
			return (i + 1);
		}
	}

	template<typename T>
	void QuickSort(T Arr[], Size_t Size)
	{
		if(Size <= 1) return;
		Size_t pi = Detail::Partition(Arr, Size);
		QuickSort(Arr, pi);
		QuickSort(Arr + pi + 1, Size - pi - 1);
	}
}