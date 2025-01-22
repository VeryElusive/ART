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
			CNode(T Value) : Next(NULL)
			{
				ART::Memcpy(&Data, &Value, sizeof(T));
			}

			CNode *&GetNext() { return Next; };
			T &GetData() { return Data; };

		private:
			T			Data;
			CNode *Next;
		};

		LinkedList() { Size = 0; Head = Tail = NULL; }

		bool Append(T Value)
		{
			CNode *NewNode = (CNode *)ART::Alloc(sizeof(CNode));
			if(NewNode == NULL)
			{
				return FALSE;
			}

			ART::Memcpy(&NewNode->GetData(), &Value, sizeof(T));
			NewNode->GetNext() = NULL;

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

		bool Remove(CNode *Node)
		{
			if(Head == NULL || Node == NULL)
			{
				return FALSE;
			}

			if(Node == Head)
			{
				Head = Head->GetNext();
				if(Head == NULL)
				{
					Tail = NULL;
				}
			}
			else
			{
				CNode *Prev = Head;
				while(Prev->GetNext() != NULL && Prev->GetNext() != Node)
				{
					Prev = Prev->GetNext();
				}

				if(Prev->GetNext() == NULL)
				{
					return FALSE;
				}

				Prev->GetNext() = Node->GetNext();
				if(Node == Tail)
				{
					Tail = Prev;
				}
			}

			ART::Free(Node);
			--Size;
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

		CNode *&GetHeadNode()
		{
			return Head;
		}

		CNode *&GetTailNode()
		{
			return Tail;
		}

		CNode *&Iterate(CNode *Node)
		{
			if(Node == NULL)
			{
				return Head;
			}

			return Node->GetNext();
		}

	private:
		CNode *Head;
		CNode *Tail;

		Size_t			Size;
	};
}