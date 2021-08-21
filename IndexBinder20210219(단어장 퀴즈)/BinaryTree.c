//BinaryTree.c
#include"BinaryTree.h"
#include<stdlib.h>
#include<memory.h>

void BinaryTree_Create(BinaryTree* binaryTree) {
	binaryTree->root = NULL;
	binaryTree->length = 0;
	binaryTree->balance = 0;
}

BinaryNode* BinaryTree_Insert(BinaryTree* binaryTree, void* key, size_t size, int(*compare)(void*, void*)) {
	BinaryNode* index;
	BinaryNode* node;
	BinaryNode* parent = NULL;

	//1. key�� ���� �Է¹޴´�.
	//2. index�� ���� �Ҵ��Ѵ�.
	index = (BinaryNode*)malloc(sizeof(BinaryNode) + size);
	memcpy(index + 1, key, size);
	index->left = NULL;
	index->right = NULL;
	node = binaryTree->root;

	//3. �θ� ã�´�.
	while (node != NULL) {
		parent = node;
		if (compare(node + 1, key) > 0) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}

	//4. �θ�� index�� �մ´�.
	if (parent != NULL) {
		if (compare(parent + 1, key) > 0) {
			parent->left = index;
		}
		else {
			parent->right = index;
		}
	}
	else {
		binaryTree->root = index;
	}

	//5. length�� 1�� ������Ų��.
	binaryTree->length++;

	//6. �θ��� key�� key�� ���Ͽ� balance�� �����Ѵ�.
	if (compare(binaryTree->root + 1, key) > 0) {
		binaryTree->balance--;
	}
	else if(compare(binaryTree->root + 1, key) < 0){
		binaryTree->balance++;
	}

	//7. index�� ����Ѵ�.
	return index;

	//8. ������.
}

BinaryNode* Search(BinaryTree* binaryTree, void* key, int(*compare)(void*, void*)) {
	BinaryNode* it;
	BinaryNode* index = NULL;

	//1. key�� �Է¹޴´�.
	//2. it�� null�� �ƴϰ�, it�� key���� �ƴѵ��� �ݺ��Ѵ�.
	it = binaryTree->root;
	while (it != NULL && compare(it + 1, key) != 0) {
		if (compare(it + 1, key) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	index = it;
	//3. index�� ����Ѵ�.
	return index;
	//4. ������.
}


void BinaryTree_GetAt(BinaryTree* binaryTree, BinaryNode* index, void* key, size_t size) {
	//1. index�� �Է¹޴´�.
	//2. index�� key���� key�� ���´�.
	memcpy(key, index + 1, size);
	//3. key�� ����Ѵ�.
	//4.������.
}

BinaryNode* BinaryTree_Delete(BinaryTree* binaryTree, void* key, size_t size, int(*compare)(void*, void*)) {
	BinaryNode* index = NULL;
	BinaryNode* parent = NULL;
	BinaryNode* replace = NULL;
	BinaryNode* replaceKid = NULL;
	BinaryNode* replaceParent = NULL;
	BinaryNode* it = NULL;
	size_t rootKey;
	BinaryNode* right = NULL;

	//1. key���� �Է¹޴´�.
	//2. key�� ��ġ�� ã�´�.
	it = binaryTree->root;
	while (compare(it + 1, key) != 0) {
		parent = it;
		if (compare(it + 1, key) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	index = it;
	right = index->right;

	//3. replace�� ã�´�.
	it = index->right;
	while (it != NULL) {
		replace = it;
		it = it->left;
	}
	if (right == NULL) { //3.1 index�� ������ ���� ���ٸ�,
		it = index->left;
		while (it != NULL) {
			replace = it;
			it = it->right;
		}
	}
	//4. replaceParent�� ��ġ�� ã�´�.
	it = index;
	while (replace != NULL && it != replace) {
		replaceParent = it;
		if (compare(it + 1, replace + 1) > 0) {
			it = it->left;
		}
		else {
			it = it->right;
		}
	}
	//5. replaceParent�� replaceKid�� ���踦 �����Ѵ�.
	if (replace != NULL && right != NULL && replaceParent != index) {
		replaceKid = replace->right;
		replaceParent->left = replaceKid;
	}
	else if (replace != NULL && right == NULL && replaceParent != index) {
		replaceKid = replace->left;
		replaceParent->right = replaceKid;
	}
	//6. replace�� ���踦 �����Ѵ�.
	if (replace != NULL && right != NULL && replaceParent != index) {
		replace->left = index->left;
		replace->right = index->right;
	}
	else if (replace != NULL && right == NULL && replaceParent != index) {
		replace->left = index->left;
	}
	else if (replace != NULL && right != NULL && replaceParent == index) {
		replace->left = index->left;
	}
	//7. parent���� ���踦 �����Ѵ�.
	rootKey = malloc(size);
	memcpy(rootKey, binaryTree->root + 1, size);

	if (parent != NULL) {
		if (compare(parent + 1, index + 1) > 0) {
			parent->left = replace;
		}
		else {
			parent->right = replace;
		}
	}
	else {
		binaryTree->root = replace;
	}
	//8. index�� �����.
	if (index != NULL) {
		free(index);
		index = NULL;
	}
	//9. length�� 1�� ����.
	binaryTree->length--;
	//10. balance�� ����Ѵ�.
	if (compare(rootKey, key) > 0) {
		binaryTree->balance++;
	}
	else if (compare(rootKey, key) < 0) {
		binaryTree->balance--;
	}
	else if (replace != NULL) {
		if (compare(rootKey, replace + 1) > 0) {
			binaryTree->balance++;
		}
		else {
			binaryTree->balance--;
		}
	}

	//11. index�� ����Ѵ�.
	return index;
	//12. ������.
	if (rootKey != NULL) {
		free(rootKey);
		rootKey = NULL;
	}
}

void MakeBalance(BinaryTree* binaryTree, size_t size) {
	void(*keys);
	Long count;

	//1. MakeKeys�� ȣ���Ѵ�.
	MakeKeys(binaryTree, &keys, &count, size);
	//2. DeleteAllItems�� ȣ���Ѵ�.
	DeleteAllItems(binaryTree);
	//3. MakeTree�� ȣ���Ѵ�.
	MakeTree(binaryTree, keys, count, size);
	//4. ������.
}

void Node_MakeKeys(BinaryNode* node, void(*keys), Long* count, Long* i, size_t size) {
	//1. ��带 �д´�.
	//2. ��尡 null�� �ƴ� ���,
	if (node != NULL) {
		//2.1 ����� left���� 1~4�� �ݺ��Ѵ�.
		Node_MakeKeys(node->left, keys, count, i, size); // �� ���� MakeKeys�� ��ȯ�Ǵ� ���̴�.
		//2.2 keys�� ����� key�� ���´�.
		memcpy(((char(*))keys) + ((*i) * size), node + 1, size);
		//2.3 ������ ����.
		(*count)++;
		(*i)++;
		//2.4 ����� right���� 1~4�� �ݺ��Ѵ�.
		Node_MakeKeys(node->right, keys, count, i, size);
	}
	//3. keys, i, count�� ����Ѵ�.
	//4. ������.
}

void MakeKeys(BinaryTree* binaryTree, void* (*keys), Long* count, size_t size) {
	Long i = 0;
	*count = 0;

	//1. keys�� length��ŭ �迭�� �Ҵ��Ѵ�.
	*keys = (void(*))calloc(binaryTree->length, size);
	//2. Node_MakeKeys�� ȣ���Ѵ�.
	Node_MakeKeys(binaryTree->root, *keys, count, &i, size);
	//3. keys, count�� ����Ѵ�.
	//4. ������.
}

void Node_DeleteAllItems(BinaryNode* node) {
	//1. ��带 �д´�.
	//2. ��尡 null�� �ƴ� ���,
	if (node != NULL) {
		//2.1 ����� left���� 1~3������ �ݺ��Ѵ�.
		Node_DeleteAllItems(node->left);
		//2.2 ����� right���� 1~3������ �ݺ��Ѵ�.
		Node_DeleteAllItems(node->right);
		//2.3 node�� ���ش�.
		if (node != NULL) {
			free(node);
			node = NULL;
		}
	}
	//3. ������.
}

void DeleteAllItems(BinaryTree* binaryTree) {
	//1. Node_DeleteAllItems�� ȣ���Ѵ�.
	Node_DeleteAllItems(binaryTree->root);
	//2. root�� null ó���Ѵ�.
	binaryTree->root = NULL;
	//3. length�� 0���� �ʱ�ȭ�Ѵ�.
	binaryTree->length = 0;
	//4. balance�� 0���� �ʱ�ȭ�Ѵ�.
	binaryTree->balance = 0;
	//5. ������.
}

BinaryNode* Node_MakeTree(void(*keys), Long low, Long high, size_t size) {
	BinaryNode* node = NULL;
	Long middle;

	//1. keys, low, high�� �Է¹޴´�.
	//2. low�� high���� ���ų� ������,
	if (low <= high) {
		node = (BinaryNode*)malloc(sizeof(BinaryNode) + size);
		middle = (low + high) / 2;
		memcpy(node + 1, ((char(*))keys) + (middle * size), size);
		node->left = NULL;
		node->right = NULL;

		node->left = Node_MakeTree(keys, low, middle - 1, size);
		node->right = Node_MakeTree(keys, middle + 1, high, size);
	}
	//3. node�� ����Ѵ�.
	return node;
	//4. ������.
}

void MakeTree(BinaryTree* binaryTree, void(*keys), Long count, size_t size) {
	BinaryTree* node;
	Long low ;
	Long high;
	Long remainder;

	low = 0;
	high = count - 1;

	//1. Node_MakeTree�� ȣ���Ѵ�.
	node = Node_MakeTree(keys, low, high, size);
	//2. root�� node�� �մ´�.
	binaryTree->root = node;
	//3. length�� count�� ���´�.
	binaryTree->length = count;
	//4. count�� ¦������ Ȧ������ �Ǵ� ��, balance�� ���Ѵ�.
	remainder = count - (count / 2) * 2;
	if (remainder >= 1) {
		binaryTree->balance = 0;
	}
	else {
		binaryTree->balance = 1;
	}
	//5. ������.
}

void Node_Destroy(BinaryNode* node) {
	//1. ��带 �Է¹޴´�.
	//2. ��尡 null�� �ƴ� ���,
	if (node != NULL) {
		//2.1 ����� left�� ���� ��, 1~3 ������ �ݺ��Ѵ�.
		Node_Destroy(node->left);
		//2.2 ����� right�� ���� ��, 1~3 ������ �ݺ��Ѵ�.
		Node_Destroy(node->right);
		//2.3 ��带 �����.
		if (node != NULL) {
			free(node);
			node = NULL;
		}
	}
	//3.������.
}

void BinaryTree_Destroy(BinaryTree* binaryTree) {
	//1. Node_Destroy�� ȣ���Ѵ�.
	Node_Destroy(binaryTree->root);
	//2. ������.
}






