/**
 * @file LinkedList_Singly.cpp
 * @brief 单向链表
 * @attention
 * @version 0.1
 * @date 2023-08-19
 *
 */

#include "LinkedList_Singly.h"

void LinkedListS_t::Insert_After(LinkedListS_Node_t *Target, LinkedListS_Node_t *InsertNode) {
	if (Target == NULL || InsertNode == NULL) return;
	if (Target == prvTail) prvTail = InsertNode;
	InsertNode->next = Target->next;
	Target->next = InsertNode;
	prvSize++;
}

void LinkedListS_t::Insert_Before(LinkedListS_Node_t *Target, LinkedListS_Node_t *InsertNode) {
	LinkedListS_Node_t *Node = &BaseNode;
	if (Target == NULL || InsertNode == NULL) return;
	while (Node->next != Target) {
		if (Node->next == Target) {
			Insert_After(Node, InsertNode);
			return;
		}
		Node = Node->next;
	}
}
void LinkedListS_t::Push(LinkedListS_Node_t *Node) {
	if (Node == NULL) return;
	Insert_After(&BaseNode, Node);
}

void LinkedListS_t::Append(LinkedListS_Node_t *Node) {
	if (Node == NULL) return;
	Insert_After(prvTail, Node);
}

void LinkedListS_t::MoveToTail(LinkedListS_Node_t *Target) {
	LinkedListS_Node_t *Node = &BaseNode;
	if (Target == NULL) return;
	if (Target == prvTail) return;
	if (!Target->next) {
		Insert_After(prvTail, Target);
		return;
	}
	while (Node->next != Target) {
		if (Node->next == NULL) return;
		if (Node->next == Target) {
			Node->next = Target->next;
			Insert_After(prvTail, Target);
			return;
		}
		Node = Node->next;
	}
}

void LinkedListS_t::Remove(LinkedListS_Node_t *Target) {
	LinkedListS_Node_t *Node = &BaseNode;
	if (Target == NULL) return;
	
	while (Node->next != Target) {
		if (Node->next == NULL) return;
		Node = Node->next;
	}
	if (Target == prvTail) {
		prvTail = Node;
		prvTail->next = NULL;
	} else {
		Node->next = Target->next;
		Target->next = NULL;
	}
	prvSize--;
}

void LinkedListS_t::Pop(void) {
	if (BaseNode.next == NULL) return;
	if (BaseNode.next->next == NULL) prvTail = &BaseNode;
	LinkedListS_Node_t *Node = BaseNode.next;
	BaseNode.next = BaseNode.next->next;
	Node->next = NULL;
	prvSize--;
}

uint32_t LinkedListS_t::Size(void) {
	return prvSize;
}

/**********************************END OF FILE*********************************/
