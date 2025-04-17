/**
 * @file LinkedList_Singly.h
 * @brief 单向链表
 * @attention 
 * @version 0.2
 * @date 2023-08-19
 *
 */

#ifndef __LinkedList_Singly_H
#define __LinkedList_Singly_H

#include "main.h"

#define container_of(ptr, type, member) ((type *)((char *)(ptr) - (uint32_t)(&((type *)0)->member)))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
// #pragma pack(1)
typedef struct LinkedListS_Node {
	struct LinkedListS_Node *next;
	uint8_t type;
} LinkedListS_Node_t;
// #pragma pack()

class LinkedListS_t {
	public:
		LinkedListS_Node_t BaseNode;
	private:
		LinkedListS_Node_t *prvTail;
		uint32_t prvSize;
	public:
		LinkedListS_t() {
			BaseNode.next = NULL;
			BaseNode.type = 0;
			prvTail = &BaseNode;
			prvSize = 0;
		}
		void Insert_After(LinkedListS_Node_t *Target, LinkedListS_Node_t *InsertNode);
		void Insert_Before(LinkedListS_Node_t *Target, LinkedListS_Node_t *InsertNode);
		void Push(LinkedListS_Node_t *Node);
		void Append(LinkedListS_Node_t *Node);
		void MoveToTail(LinkedListS_Node_t *Target);
		void Remove(LinkedListS_Node_t *Target);
		void Pop(void);
		uint32_t Size(void);
};
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __LinkedList_Singly_H */

/**********************************END OF FILE*********************************/
