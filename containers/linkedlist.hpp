#pragma once

#include "../common/memory.hpp"
#include "../common/heap.hpp"

namespace ART
{
	template <typename T>
	class LinkedList
	{
	public:
		class CNode
		{
		public:
			CNode(T Value) : Data(Value), Next(NULL) {}

			CNode *&GetNext() { return Next; };
			T &GetData() { return Data; };

		private:
			T			Data;
			CNode		*Next;
		};

		bool Append(T Value)
		{
			CNode *NewNode = (CNode *)ART::Alloc(sizeof(CNode));
			if(NewNode == NULL)
			{
				return FALSE;
			}

			*NewNode = CNode(Value);
			if(Head == NULL)
			{
				Head = Tail = NewNode;
			}
			else
			{
				Tail->GetNext() = NewNode;
				Tail = NewNode;
			}

			++Size;

			return TRUE;
		}

		Size_t Count()
		{
			return Size;
		}

		void Clear()
		{
			CNode *Current = Head;

			while(Current)
			{
				CNode *ToDelete = Current;
				Current = Current->GetNext();
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

			return Head->GetData();
		}

		T *GetTail()
		{
			if(Tail == NULL)
			{
				return NULL;
			}

			return Tail->GetData();
		}

		CNode *GetHeadNode()
		{
			return Head;
		}

		CNode *GetTailNode()
		{
			return Tail;
		}

		CNode *Iterate(CNode *Node)
		{
			if(Node == NULL)
			{
				return Head;
			}

			return Node->GetNext();
		}

	private:
		CNode			*Head;
		CNode			*Tail;

		Size_t			Size;
	};
}