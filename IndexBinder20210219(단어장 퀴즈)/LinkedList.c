//LinkedList.c
#include"LinkedList.h"
#include<memory.h>
#include<stdlib.h>

void LinkedList_Create(LinkedList* linkedList) {
	//1. head�� null ó���Ѵ�.
	linkedList->head = NULL;
	//2. tail�� null ó���Ѵ�.
	linkedList->tail = NULL;
	//3. length�� 0���� �ʱ�ȭ�Ѵ�.
	linkedList->length = 0;
	//4. current�� null ó���Ѵ�.
	linkedList->current = NULL;
	//5. ������.
}

Node* LinkedList_AppendFromHead(LinkedList* linkedList, void* object, size_t size) {
	//1. ���� �Է¹޴´�.
	//2. node�� �� �տ� �ȴ´�.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->previous = linkedList->current;
	if (linkedList->head != NULL) {
		linkedList->current->next = linkedList->head;
		linkedList->head->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}
	linkedList->head = linkedList->current;
	//3. ������ ����.
	linkedList->length++;
	//4. current�� ����Ѵ�.
	return linkedList->current;
	//5. ������.
}

Node* LinkedList_InsertBefore(LinkedList* linkedList, Node* index, void* object, size_t size) {
	//1. ��ġ, ���� �Է¹޴´�.
	//2. ��ġ �տ� node�� �����.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->next = index;
	if (linkedList->head != index) {
		linkedList->current->previous = index->previous;
		index->previous->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}
	index->previous = linkedList->current;
	//3. ������ ����.
	linkedList->length++;
	//4. current�� ����Ѵ�.
	return linkedList->current;
	//5. ������.
}

Node* LinkedList_InsertAfter(LinkedList* linkedList, Node* index, void* object, size_t size) {
	//1. ��ġ, ���� �Է¹޴´�.
	//2. ��ġ �ڿ� node�� �����.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->previous = index;
	if (linkedList->tail != index) {
		linkedList->current->next = index->next;
		index->next->previous = linkedList->current;
	}
	else {
		linkedList->current->next = linkedList->current;
		linkedList->tail = linkedList->current;
	}
	index->next = linkedList->current;
	//3. ������ ����.
	linkedList->length++;
	//4. current�� ����Ѵ�.
	return linkedList->current;
	//5. ������.
}


Node* LinkedList_AppendFromTail(LinkedList* linkedList, void* object, size_t size) {
	//1. ���� �Է¹޴´�.
	//2. Node�� �� �ڿ� �ȴ´�.
	linkedList->current = (Node*)malloc(sizeof(Node) + size);
	memcpy(linkedList->current + 1, object, size);
	linkedList->current->next = linkedList->current;
	if (linkedList->tail != NULL) {
		linkedList->current->previous = linkedList->tail;
		linkedList->tail->next = linkedList->current;
	}
	else {
		linkedList->current->previous = linkedList->current;
		linkedList->head = linkedList->current;
	}
	linkedList->tail = linkedList->current;
	//3. ������ ����.
	linkedList->length++;
	//4. current�� ����Ѵ�.
	return linkedList->current;
	//5. ������.
}

Node* LinkedList_Delete(LinkedList* linkedList, Node* index) {
	//1. ��ġ�� �Է¹޴´�.
	//2. ��ġ�� head�� tail ��� �ƴ� ���, ���� ���� ī�带 �հ� current�� ã�´�.
	if (linkedList->head != index && linkedList->tail != index) {
		index->next->previous = index->previous;
		index->previous->next = index->next;
		linkedList->current = index->next;
	}
	//2.1 ��ġ�� head�� ���, ���� ���� ī�带 �հ� current�� ã�´�.
	else if (linkedList->head == index && linkedList->tail != index) {
		index->next->previous = index->next;
		linkedList->head = index->next;
		linkedList->current = index->next;
	}
	//2.2 ��ġ�� tail�� ���, ���� ���� ī�带 �հ� current�� ã�´�.
	else if (linkedList->tail == index && linkedList->head != index) {
		index->previous->next = index->previous;
		linkedList->tail = index->previous;
		linkedList->current = index->previous;
	}
	//2.3 ��ġ�� head�� tail ����� ���, ��� null ó���Ѵ�.
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}
	//3. ��ġ node�� �����.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	//4. ������ 1�� ����.
	linkedList->length--;
	//5. ��ġ�� ����Ѵ�.
	return index;
	//6. ������.
}

Node* LinkedList_DeleteFromHead(LinkedList* linkedList) {
	Node* index;

	//1. index�� head�� �ű��.
	index = linkedList->head;
	//2. current�� ��ġ�� �����Ѵ�.
	if (linkedList->tail != index) {
		index->next->previous = index->next;
		linkedList->head = index->next;
		linkedList->current = index->next;
	}
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}
	//3. index�� �����.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	//4. ������ 1�� ����.
	linkedList->length--;
	//5. index�� ����Ѵ�.
	return index;
	//6. ������.
}

Node* LinkedList_DeleteFromTail(LinkedList* linkedList) {
	Node* index;

	//1. index�� tail�� �ű��.
	index = linkedList->tail;
	//2. current�� ��ġ�� �����Ѵ�.
	if (linkedList->head != index) {
		index->previous->next = index->previous;
		linkedList->tail = index->previous;
		linkedList->current = index->previous;
	}
	else {
		linkedList->head = NULL;
		linkedList->tail = NULL;
		linkedList->current = NULL;
	}
	//3. index�� �����.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	//4. ������ 1�� ����.
	linkedList->length--;
	//5. current�� ����Ѵ�.
	return index;
	//6. ������.
}

void LinkedList_DeleteAllItems(LinkedList* linkedList) {
	Node* it;
	Node* previous = NULL;

	//1. ����� ó������ ������ �ݺ��Ѵ�.
	it = linkedList->head;
	while (it != previous) {
		linkedList->current = it->next;
		previous = it;

		//1.1  ��带 �����.
		if (it != NULL) {
			free(it);
			it = NULL;
		}
		it = linkedList->current;
	}
	//2. head�� null ó���Ѵ�.
	linkedList->head = NULL;
	//3. tail�� null ó���Ѵ�.
	linkedList->tail = NULL;
	//4. current�� null ó���Ѵ�.
	linkedList->current = NULL;
	//5. length�� 0���� �ʱ�ȭ�Ѵ�.
	linkedList->length = 0;
	//6. ������.
}

Node* LinkedList_LinearSearchUnique(LinkedList* linkedList, void* key, int(*compare)(void*, void*)) {
	Node* index = NULL;
	Node* it;
	Node* previous = NULL;

	//1. key�� �Է¹޴´�.
	//2. key�� ���� �ƴϰ�, ����� ���� �ƴ� ���� �ݺ��Ѵ�.
	it = linkedList->head;
	while (it != previous && compare(it + 1, key) != 0) {
		previous = it;
		it = it->next;
	}
	if (it != previous) {
		index = it;
	}
	//3. index�� ����Ѵ�.
	return index;
	//4. ������.
}

void LinkedList_LinearSearchDuplicate(LinkedList* linkedList, void* key, Node** (*indexes), Long* count,
	int(*compare)(void*, void*)) {
	Node* it;
	Node* previous = NULL;
	Long i = 0;
	*count = 0;

	//1. key�� �Է¹޴´�.
	//2. ����� ���� �ƴѵ��� �ݺ��Ѵ�.
	*indexes = (Node * (*))calloc(linkedList->length, sizeof(Node*));
	it = linkedList->head;
	while (it != previous) {
		//2.1 object�� key���� ���,
		if (compare(it + 1, key) == 0) {
			//2.1.1. indexes�� ���´�.
			(*indexes)[i] = it;
			//2.1.2. ������ ����.
			(*count)++;
			i++;
		}
		previous = it;
		it = it->next;
	}
	//2. indexes, count�� ����Ѵ�.
	//3. ������.
}

Node* LinkedList_First(LinkedList* linkedList) {
	//1. current�� ���� ���� �ű��.
	linkedList->current = linkedList->head;
	//2. current�� ����Ѵ�.
	return linkedList->current;
	//3. ������.
}

Node* LinkedList_Previous(LinkedList* linkedList) {
	//1. current�� ���� ���� �ű��.
	linkedList->current = linkedList->current->previous;
	//2. current�� ����Ѵ�.
	return linkedList->current;
	//3. ������.
}

Node* LinkedList_Next(LinkedList* linkedList) {
	//1. current�� ���� ���� �ű��.
	linkedList->current = linkedList->current->next;
	//2. current�� ����Ѵ�.
	return linkedList->current;
	//3. ������.
}

Node* LinkedList_Last(LinkedList* linkedList) {
	//1. current�� tail�� �ű��.
	linkedList->current = linkedList->tail;
	//2. current�� ����Ѵ�.
	return linkedList->current;
	//3. ������.
}

void LinkedList_GetAt(LinkedList* linkedList, Node* index, void* object, size_t size) {
	//1. ��ġ�� �Է¹޴´�.
	//2. ��ġ�� ���� object�� ���´�.
	memcpy(object, index + 1, size);
	//3. object�� ����Ѵ�.
	//4. ������.
}

void LinkedList_Destroy(LinkedList* linkedList) {
	Node* it;
	Node* previous = NULL;

	//1. ����� ���� �ƴѵ��� �ݺ��Ѵ�.
	it = linkedList->head;
	while (it != previous) {
		linkedList->current = it->next;
		previous = it;
		//1.1 ��带 �����.
		if (it != NULL) {
			free(it);
			it = NULL;
		}
		it = linkedList->current;
	}
	//2. ������.
}



