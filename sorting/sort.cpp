#include "sort.hpp"

template<typename T = Size_t>
void Swap(T &a, T &b) 
{
	T TMP = a;
	a = b;
	b = TMP;
}

template<typename T = Size_t>
Size_t Partition(T Arr[], Size_t Size)
{
	int pivot = Arr[Size - 1];
	int i = -1; 

	for(int j = 0; j < Size - 1; j++) 
	{
		if(Arr[j] <= pivot)
		{
			i++;
			Swap(Arr[i], Arr[j]);
		}
	}
	Swap(Arr[i + 1], Arr[Size - 1]);
	return (i + 1);
}

template<typename T = Size_t>
void ART::QuickSort(T Arr[], Size_t Size)
{
	if(Size <= 1) 
	{
		return;
	}

	Size_t pi = Partition(Arr, Size);

	QuickSort(Arr, pi);
	QuickSort(Arr + pi + 1, Size - pi - 1);
}