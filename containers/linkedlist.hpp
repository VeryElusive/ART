#pragma once

#include "../common/memory.hpp"
#include "../common/heap.hpp"

namespace ART
{
	template <typename T>
	class LinkedList
	{
	public:
		struct Node_t 
		{
			T			Data;
			Node_t		*Next;

			Node_t(T Value) : Data(Value), Next(NULL) {}
		};

		bool Append(T Value) 
		{
			Node_t *NewNode = ART::Alloc(sizeof(Node_t));
			if(NewNode == NULL)
			{
				return FALSE;
			}

			*NewNode = Node_t(Value);
			if(Head == NULL) 
			{
				Head = Tail = NewNode;
			}
			else 
			{
				Tail->Next = NewNode;
				Tail = NewNode;
			}

			++Size;

			return TRUE;
		}

		void Clear()
		{
			Node_t *Current = Head;

			while(Current) 
			{
				Node_t *ToDelete = Current;
				Current = Current->Next;
				ART::Free(ToDelete);
			}

			Head = Tail = NULL;
			Size = 0;
		}

		T *GetHead()
		{
			if(Head == NULL)
			{
				return NULL;
			}

			return Head->Data;
		}

		T *GetTail()
		{
			if(Tail == NULL)
			{
				return NULL;
			}

			return Tail->Data;
		}

	private:
		Node_t			*Head;
		Node_t			*Tail;

		Size_t			Size;
	}
}