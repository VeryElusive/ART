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

	// Bounded stack usage and O(n log n), including already sorted input.
	template<typename T>
	void HeapSort(T Arr[], Size_t Size)
	{
		if(Size < 2) return;
		Size_t Start = Size / 2;
		Size_t End = Size;
		while(End > 1)
		{
			if(Start != 0)
			{
				Start--;
			}
			else
			{
				Detail::Swap(Arr[0], Arr[--End]);
			}
			Size_t Root = Start;
			while(Root < End / 2)
			{
				Size_t Child = Root * 2 + 1;
				if(Child + 1 < End && Arr[Child] < Arr[Child + 1]) Child++;
				if(!(Arr[Root] < Arr[Child])) break;
				Detail::Swap(Arr[Root], Arr[Child]);
				Root = Child;
			}
		}
	}
}
