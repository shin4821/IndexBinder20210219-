//Vocabulary.c
#include"Vocabulary.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#if 0
int main(int argc, char* argv[]) {
	Vocabulary vocabulary;
	CorrectQuiz correctQuiz;
	WordCard* index;
	WordCard* previous = NULL;
	Long count;
	WordCard wordCard;
	Long i;
	char meaning[32];

	Vocabulary_Create(&vocabulary);
	CorrectQuiz_Create(&correctQuiz);

	strcpy(wordCard.word.spelling, "apple");
	strcpy(wordCard.word.wordClass, "명사");
	strcpy(wordCard.word.meaning, "사과");
	strcpy(wordCard.word.example, "I like apple.");
	strcpy(wordCard.quiz.spelling, "apple");
	strcpy(wordCard.quiz.answer, "");
	strcpy(wordCard.quiz.ox, "");
	strcpy(wordCard.quiz.meaning, "사과");
	strcpy(wordCard.quiz.isImportant, "");
	index = Put(&vocabulary, wordCard);
	printf("%s %s %s %s %s %s %s %s %s\n", index->word.spelling, index->word.wordClass,
		index->word.meaning, index->word.example, index->quiz.spelling, index->quiz.answer,
		index->quiz.ox, index->quiz.meaning, index->quiz.isImportant);

	strcpy(wordCard.word.spelling, "hesitate");
	strcpy(wordCard.word.wordClass, "동사");
	strcpy(wordCard.word.meaning, "망설이다");
	strcpy(wordCard.word.example, "Don't hesitate");
	strcpy(wordCard.quiz.spelling, "hesitate");
	strcpy(wordCard.quiz.answer, "");
	strcpy(wordCard.quiz.ox, "");
	strcpy(wordCard.quiz.meaning, "망설이다");
	strcpy(wordCard.quiz.isImportant, "");
	index = Put(&vocabulary, wordCard);
	printf("%s %s %s %s %s %s %s %s %s\n", index->word.spelling, index->word.wordClass,
		index->word.meaning, index->word.example, index->quiz.spelling, index->quiz.answer,
		index->quiz.ox, index->quiz.meaning, index->quiz.isImportant);

	strcpy(wordCard.word.spelling, "apple");
	strcpy(wordCard.word.wordClass, "명사");
	strcpy(wordCard.word.meaning, "사과");
	strcpy(wordCard.word.example, "Do you like apple?");
	strcpy(wordCard.quiz.spelling, "apple");
	strcpy(wordCard.quiz.answer, "");
	strcpy(wordCard.quiz.ox, "");
	strcpy(wordCard.quiz.meaning, "사과");
	strcpy(wordCard.quiz.isImportant, "");
	index = Put(&vocabulary, wordCard);
	printf("%s %s %s %s %s %s %s %s %s\n", index->word.spelling, index->word.wordClass,
		index->word.meaning, index->word.example, index->quiz.spelling, index->quiz.answer,
		index->quiz.ox, index->quiz.meaning, index->quiz.isImportant);

	strcpy(wordCard.word.spelling, "winter");
	strcpy(wordCard.word.wordClass, "명사");
	strcpy(wordCard.word.meaning, "겨울");
	strcpy(wordCard.word.example, "This winter have big snow.");
	strcpy(wordCard.quiz.spelling, "winter");
	strcpy(wordCard.quiz.answer, "");
	strcpy(wordCard.quiz.ox, "");
	strcpy(wordCard.quiz.meaning, "겨울");
	strcpy(wordCard.quiz.isImportant, "");
	index = Put(&vocabulary, wordCard);
	printf("%s %s %s %s %s %s %s %s %s\n", index->word.spelling, index->word.wordClass,
		index->word.meaning, index->word.example, index->quiz.spelling, index->quiz.answer,
		index->quiz.ox, index->quiz.meaning, index->quiz.isImportant);

	index = Vocabulary_First(&vocabulary);
	printf("%s %s %s %s %s %s %s %s %s\n", index->word.spelling, index->word.wordClass,
		index->word.meaning, index->word.example, index->quiz.spelling, index->quiz.answer,
		index->quiz.ox, index->quiz.meaning, index->quiz.isImportant);

#if 0
	strcpy(meaning, 사과);
	index = Vocabulary_SpellingQuiz(&vocabulary, &meaning, index);
	printf("적은답: %s 오답유무:%s 정답:%s\n", index->quiz.answer, index->quiz.ox, index->quiz.meaning);
#endif

#if 0

	index = Question(&vocabulary);
	printf("%s %s %s %s %s %s %s %s %s\n", index->word.spelling, index->word.wordClass,
		index->word.meaning, index->word.example, index->quiz.spelling, index->quiz.answer,
		index->quiz.ox, index->quiz.meaning, index->quiz.isImportant);

	index = SpellingQuiz(&vocabulary, "참외", index);

	index = SpellingQuiz(&vocabulary, "망설이다", index);

	index = SpellingQuiz(&vocabulary, "사과", index);

	SpellingQuiz(&vocabulary, "봄", index);
#endif

	Vocabulary_Destroy(&vocabulary);
	CorrectQuiz_Destroy(&correctQuiz);

	return 0;
}
#endif

void Vocabulary_Create(Vocabulary* vocabulary) {
	LinkedList_Create(&vocabulary->wordCards);
	vocabulary->length = 0;
	vocabulary->current = NULL;
}
void CorrectQuiz_Create(CorrectQuiz* correctQuiz) {
	LinkedList_Create(&correctQuiz->wordCards);
	correctQuiz->length = 0;
	correctQuiz->current = NULL;
}

Long Load(Vocabulary* vocabulary) {
	WordCard wordCard;
	Word word;
	Quiz quiz;
	Node* node;
	FILE* fileIncorrectQuiz;
	size_t flagIncorrectQuiz;

	fileIncorrectQuiz = fopen("IncorrectQuiz.dat", "rb");
	if (fileIncorrectQuiz != NULL) {
		//2.1 틀리거나 안 푼 퀴즈의 단어카드를 읽는다.
		flagIncorrectQuiz = fread(&word, sizeof(Word), 1, fileIncorrectQuiz);
		flagIncorrectQuiz = fread(&quiz, sizeof(Quiz), 1, fileIncorrectQuiz);
		//2. IncorrectQuiz파일의 끝이 아닌동안 반복한다.
		while (!feof(fileIncorrectQuiz) && flagIncorrectQuiz != 0) {
			//2.2 단어카드를 꽂는다.
			wordCard.word = word;
			wordCard.quiz = quiz;
			node = LinkedList_AppendFromTail(&vocabulary->wordCards, &wordCard, sizeof(WordCard));
			vocabulary->length++;
			vocabulary->current = (WordCard*)(node + 1);
			//2.1 틀리거나 안 푼 퀴즈의 단어카드를 읽는다.
			flagIncorrectQuiz = fread(&word, sizeof(Word), 1, fileIncorrectQuiz);
			flagIncorrectQuiz = fread(&quiz, sizeof(Quiz), 1, fileIncorrectQuiz);
		}
		fclose(fileIncorrectQuiz);
	}
	return vocabulary->length;
}
Long CorrectQuiz_Load(CorrectQuiz* correctQuiz) {
	WordCard wordCard;
	Word word;
	Quiz quiz;
	Node* node;
	FILE* fileCorrectQuiz;
	size_t flagCorrectQuiz;

	fileCorrectQuiz = fopen("CorrectQuiz.dat", "rb");
	if (fileCorrectQuiz != NULL) {
		//1.1 맞은 퀴즈의 단어 카드를 읽는다.
		flagCorrectQuiz = fread(&word, sizeof(Word), 1, fileCorrectQuiz);
		flagCorrectQuiz = fread(&quiz, sizeof(Quiz), 1, fileCorrectQuiz);
		//1. CorrectQuiz 파일의 끝이 아닌동안 반복한다.
		while (!feof(fileCorrectQuiz) && flagCorrectQuiz != 0) {
			//1.2 단어카드를 꽂는다.
			wordCard.word = word;
			wordCard.quiz = quiz;
			node = LinkedList_AppendFromTail(&correctQuiz->wordCards, &wordCard, sizeof(WordCard));
			correctQuiz->length++;
			correctQuiz->current = (WordCard*)(node + 1);
			//1.1 맞은 퀴즈의 단어카드를 읽는다.
			flagCorrectQuiz = fread(&word, sizeof(Word), 1, fileCorrectQuiz);
			flagCorrectQuiz = fread(&quiz, sizeof(Quiz), 1, fileCorrectQuiz);
		}
		fclose(fileCorrectQuiz);
	}
	return correctQuiz->length;
}


WordCard* Vocabulary_Put(Vocabulary* vocabulary, WordCard wordCard) {
	Node* node;

	node = LinkedList_AppendFromTail(&vocabulary->wordCards, &wordCard, sizeof(WordCard));
	vocabulary->length++;
	vocabulary->current = (WordCard*)(node + 1);

	return vocabulary->current;
}
WordCard* CorrectQuiz_Put(CorrectQuiz* correctQuiz, WordCard wordCard) {
	Node* node;

	node = LinkedList_AppendFromTail(&correctQuiz->wordCards, &wordCard, sizeof(WordCard));
	correctQuiz->length++;
	correctQuiz->current = (WordCard*)(node + 1);

	return correctQuiz->current;
}

void FindBySpelling(Vocabulary* vocabulary, char(*spelling), WordCard** (*indexes), Long* count) {
	Node* (*indexes_);
	Long i = 0;

	LinkedList_LinearSearchDuplicate(&vocabulary->wordCards, spelling, &indexes_, count, CompareSpellings);
	*indexes = (WordCard * (*))calloc(vocabulary->length, sizeof(WordCard*));
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(indexes_[i] + 1);
		i++;
	}
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
}
void CorrectQuiz_FindBySpelling(CorrectQuiz* correctQuiz, char(*spelling), WordCard** (*indexes),
	Long* count) {
	Node* (*indexes_);
	Long i = 0;

	LinkedList_LinearSearchDuplicate(&correctQuiz->wordCards, spelling, &indexes_, count, CompareSpellings);
	*indexes = (WordCard * (*))calloc(correctQuiz->length, sizeof(WordCard*));
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(indexes_[i] + 1);
		i++;
	}
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
}

void FindByMeaning(Vocabulary* vocabulary, char(*meaning), WordCard** (*indexes), Long* count) {
	Node* (*indexes_);
	Long i = 0;

	LinkedList_LinearSearchDuplicate(&vocabulary->wordCards, meaning, &indexes_, count, CompareMeanings);
	*indexes = (WordCard * (*))calloc(vocabulary->length, sizeof(WordCard*));
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(indexes_[i] + 1);
		i++;
	}
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
}
void CorrectQuiz_FindByMeaning(CorrectQuiz* correctQuiz, char(*meaning), WordCard** (*indexes),
	Long* count) {
	Node* (*indexes_);
	Long i = 0;

	LinkedList_LinearSearchDuplicate(&correctQuiz->wordCards, meaning, &indexes_, count, CompareMeanings);
	*indexes = (WordCard * (*))calloc(correctQuiz->length, sizeof(WordCard*));
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(indexes_[i] + 1);
		i++;
	}
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
}

WordCard* FindSpelling(Vocabulary* vocabulary, char(*spelling)) {
	Node* node;
	WordCard* index;

	node = LinkedList_LinearSearchUnique(&vocabulary->wordCards, spelling, CompareSpellings);
	if (node != NULL) {
		index = (WordCard*)(node + 1);
	}
	else {
		index = NULL;
	}
	return index;
}
WordCard* CorrectQuiz_FindSpelling(CorrectQuiz* correctQuiz, char(*spelling)) {
	Node* node;
	WordCard* index;

	node = LinkedList_LinearSearchUnique(&correctQuiz->wordCards, spelling, CompareSpellings);
	if (node != NULL) {
		index = (WordCard*)(node + 1);
	}
	else {
		index = NULL;
	}
	return index;
}

WordCard* Vocabulary_First(Vocabulary* vocabulary) {
	Node* node;

	node = LinkedList_First(&vocabulary->wordCards);
	vocabulary->current = (WordCard*)(node + 1);

	return vocabulary->current;
}
WordCard* CorrectQuiz_First(CorrectQuiz* correctQuiz) {
	Node* node;

	node = LinkedList_First(&correctQuiz->wordCards);
	correctQuiz->current = (WordCard*)(node + 1);

	return correctQuiz->current;
}

WordCard* Vocabulary_Next(Vocabulary* vocabulary) {
	Node* node;

	node = LinkedList_Next(&vocabulary->wordCards);
	vocabulary->current = (WordCard*)(node + 1);

	return vocabulary->current;
}
WordCard* CorrectQuiz_Next(CorrectQuiz* correctQuiz) {
	Node* node;

	node = LinkedList_Next(&correctQuiz->wordCards);
	correctQuiz->current = (WordCard*)(node + 1);

	return correctQuiz->current;
}

WordCard* Vocabulary_Previous(Vocabulary* vocabulary) {
	Node* node;

	node = LinkedList_Previous(&vocabulary->wordCards);
	vocabulary->current = (WordCard*)(node + 1);

	return vocabulary->current;
}
WordCard* CorrectQuiz_Previous(CorrectQuiz* correctQuiz) {
	Node* node;

	node = LinkedList_Previous(&correctQuiz->wordCards);
	correctQuiz->current = (WordCard*)(node + 1);

	return correctQuiz->current;
}

WordCard* Vocabulary_Last(Vocabulary* vocabulary) {
	Node* node;

	node = LinkedList_Last(&vocabulary->wordCards);
	vocabulary->current = (WordCard*)(node + 1);

	return vocabulary->current;
}
WordCard* CorrectQuiz_Last(CorrectQuiz* correctQuiz) {
	Node* node;

	node = LinkedList_Last(&correctQuiz->wordCards);
	correctQuiz->current = (WordCard*)(node + 1);

	return correctQuiz->current;
}

WordCard* Move(Vocabulary* vocabulary, WordCard* index) {
	Node* node;

	node = LinkedList_LinearSearchUnique(&vocabulary->wordCards, index, Vocabulary_CompareWordCards);
	vocabulary->wordCards.current = node;
	vocabulary->current = (WordCard*)(node + 1);

	return vocabulary->current;
}
WordCard* CorrectQuiz_Move(CorrectQuiz* correctQuiz, WordCard* index) {
	Node* node;

	node = LinkedList_LinearSearchUnique(&correctQuiz->wordCards, index, Vocabulary_CompareWordCards);
	correctQuiz->wordCards.current = node;
	correctQuiz->current = (WordCard*)(node + 1);

	return correctQuiz->current;
}

WordCard Vocabulary_PutOut(Vocabulary* vocabulary, WordCard* index) {
	Node* node;
	WordCard wordCard;

	node = LinkedList_LinearSearchUnique(&vocabulary->wordCards, index, Vocabulary_CompareWordCards);
	LinkedList_GetAt(&vocabulary->wordCards, node, &wordCard, sizeof(WordCard));
	LinkedList_Delete(&vocabulary->wordCards, node);
	vocabulary->length--;
	if (vocabulary->wordCards.current != NULL) {
		vocabulary->current = (WordCard*)(vocabulary->wordCards.current + 1);
	}
	else {
		vocabulary->current = NULL;
	}
	return wordCard;
}
WordCard CorrectQuiz_PutOut(CorrectQuiz* correctQuiz, WordCard* index) {
	Node* node;
	WordCard wordCard;

	node = LinkedList_LinearSearchUnique(&correctQuiz->wordCards, index, Vocabulary_CompareWordCards);
	LinkedList_GetAt(&correctQuiz->wordCards, node, &wordCard, sizeof(WordCard));
	LinkedList_Delete(&correctQuiz->wordCards, node);
	correctQuiz->length--;
	if (correctQuiz->wordCards.current != NULL) {
		correctQuiz->current = (WordCard*)(correctQuiz->wordCards.current + 1);
	}
	else {
		correctQuiz->current = NULL;
	}
	return wordCard;
}

WordCard* Vocabulary_SpellingQuiz(Vocabulary* vocabulary , char(*meaning), WordCard* index) {
	Node* node;

	//1. 의미, 위치를 입력받는다.
	//2. 의미를 위치의 퀴즈 답에 적는다.
	strcpy(index->quiz.answer, meaning);
	//3. 의미가 맞으면 퀴즈 정답 유무에 O를 적는다.
	if (strcmp(index->quiz.answer, index->quiz.meaning) == 0) {
		strcpy(index->quiz.ox, "O");
	}
	else {
		strcpy(index->quiz.ox, "X");
	}

	return index;
}
WordCard* NextQuiz(Vocabulary* vocabulary) {
	Node* node;
	Node* current;
	WordCard* index = NULL;

	//1. 현재 위치를 찾는다.
	current = vocabulary->wordCards.current;

	//2. 다음 문제를 찾는다.
	node = LinkedList_Next(&vocabulary->wordCards);

	if (node != current) {
		index = (WordCard*)(node + 1);
		vocabulary->current = (WordCard*)(node + 1);
	}
	return index;
}

#if 0
void CorrectQuiz_FindSolvedQuiz(CorrectQuiz* correctQuiz, WordCard** (*indexes), Long* count) {
	char key[3] = "O";
	Node* (*indexes_);
	Long i = 0;
	Long j;
	Long k = 0;

	LinkedList_LinearSearchDuplicate(&correctQuiz->wordCards, key, &indexes_, count, CompareOx);
	*indexes = (WordCard * (*))calloc(correctQuiz->length, sizeof(WordCard*));
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(indexes_[i] + 1);
		i++;
	}
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
	j = *count; //2개
	strcpy(key, "X");
	LinkedList_LinearSearchDuplicate(&correctQuiz->wordCards, key, &indexes_, count, CompareOx);
	while (k < *count) {
		(*indexes)[i] = (WordCard*)(indexes_[k] + 1);
		i++;
		k++;
	}
	*count = *count + j;//4개
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
}

void CorrectQuiz_FindIncorrectQuiz(CorrectQuiz* correctQuiz, WordCard** (*indexes), Long* count) {
	char key[3] = "X";
	Node* (*indexes_);
	Long i = 0;

	LinkedList_LinearSearchDuplicate(&correctQuiz->wordCards, key, &indexes_, count, CompareOx);
	*indexes = (WordCard * (*))calloc(correctQuiz->length, sizeof(WordCard*));
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(indexes_[i] + 1);
		i++;
	}
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
}
#endif

void CorrectQuiz_Reset(CorrectQuiz* correctQuiz) {
	Node* node;
	Node* previous = NULL;
	WordCard* index;

	node = LinkedList_First(&correctQuiz->wordCards);
	while (node != previous) {
		index = (WordCard*)(node + 1);
		//1. 퀴즈 답을 공백 처리한다.
		strcpy(index->quiz.answer, "");
		//2. 퀴즈 정답유무를 공백 처리한다.
		strcpy(index->quiz.ox, "");
		previous = node;
		node = LinkedList_Next(&correctQuiz->wordCards);
	}
}

WordCard* PutImportantWord(Vocabulary* vocabulary, WordCard* index) {
	strcpy(index->quiz.isImportant, "중요");
	return index;
}

#if 0
WordCard* CorrectQuiz_PutImportantWord(CorrectQuiz* correctQuiz, WordCard* index) {
	strcpy(index->quiz.isImportant, "중요");
	return index;
}
#endif

WordCard* PutOutImportantWord(Vocabulary* vocabulary, WordCard* index) {
	strcpy(index->quiz.isImportant, "");
	return index;
}

#if 0
WordCard* CorrectQuiz_PutOutImportantWord(CorrectQuiz* correctQuiz, WordCard* index) {
	strcpy(index->quiz.isImportant, "");
	return index;
}
#endif

void FindImportantWords(Vocabulary* vocabulary, WordCard** (*indexes), Long* count) {
	char key[5] = "중요";
	Node* (*indexes_);
	Long i = 0;

	LinkedList_LinearSearchDuplicate(&vocabulary->wordCards, key, &indexes_, count, CompareIsImportant);
	*indexes = (WordCard * (*))calloc(vocabulary->length, sizeof(WordCard*));
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(indexes_[i] + 1);
		i++;
	}
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
}

#if 0
void CorrectQuiz_FindImportantWords(CorrectQuiz* correctQuiz, WordCard** (*indexes), Long* count) {
	char key[5] = "중요";
	Node* (*indexes_);
	Long i = 0;

	LinkedList_LinearSearchDuplicate(&correctQuiz->wordCards, key, &indexes_, count, CompareIsImportant);
	*indexes = (WordCard * (*))calloc(correctQuiz->length, sizeof(WordCard*));
	while (i < *count) {
		(*indexes)[i] = (WordCard*)(indexes_[i] + 1);
		i++;
	}
	if (indexes_ != NULL) {
		free(indexes_);
		indexes_ = NULL;
	}
}
#endif

WordCard Vocabulary_GetAt(Vocabulary* vocabulary, WordCard* index) {
	WordCard wordCard;
	Node* node;

	node = LinkedList_LinearSearchUnique(&vocabulary->wordCards, index, Vocabulary_CompareWordCards);
	LinkedList_GetAt(&vocabulary->wordCards, node, &wordCard, sizeof(WordCard));

	return wordCard;
}
WordCard CorrectQuiz_GetAt(CorrectQuiz* correctQuiz, WordCard* index) {
	WordCard wordCard;
	Node* node;

	node = LinkedList_LinearSearchUnique(&correctQuiz->wordCards, index, Vocabulary_CompareWordCards);
	LinkedList_GetAt(&correctQuiz->wordCards, node, &wordCard, sizeof(WordCard));

	return wordCard;
}

Long Save(Vocabulary* vocabulary) {
	Node* node;
	WordCard wordCard;
	FILE* fileIncorrectQuiz;
	Node* previous = NULL;

	fileIncorrectQuiz = fopen("IncorrectQuiz.dat", "wb");
	if (fileIncorrectQuiz != NULL) {
		//1. 단어카드의 처음부터 끝까지 반복한다.
		node = LinkedList_First(&vocabulary->wordCards);
		while (node != previous) {
			LinkedList_GetAt(&vocabulary->wordCards, node, &wordCard, sizeof(WordCard));
			//1.1 맞은 퀴즈는 CorrectQuiz파일에 적는다.
			fwrite(&wordCard.word, sizeof(Word), 1, fileIncorrectQuiz);
			fwrite(&wordCard.quiz, sizeof(Quiz), 1, fileIncorrectQuiz);
			previous = node;
			node = LinkedList_Next(&vocabulary->wordCards);
		}
		fclose(fileIncorrectQuiz);
	}
	//2. 개수를 출력한다.
	return vocabulary->length;
	//3. 끝내다.
}
Long CorrectQuiz_Save(CorrectQuiz* correctQuiz) {
	Node* node;
	WordCard wordCard;
	FILE* fileCorrectQuiz;
	Node* previous = NULL;

	fileCorrectQuiz = fopen("CorrectQuiz.dat", "wb");
	if (fileCorrectQuiz != NULL) {
		//1. 단어카드의 처음부터 끝까지 반복한다.
		node = LinkedList_First(&correctQuiz->wordCards);
		while (node != previous) {
			LinkedList_GetAt(&correctQuiz->wordCards, node, &wordCard, sizeof(WordCard));
			//1.1 맞은 퀴즈는 CorrectQuiz파일에 적는다.
			fwrite(&wordCard.word, sizeof(Word), 1, fileCorrectQuiz);
			fwrite(&wordCard.quiz, sizeof(Quiz), 1, fileCorrectQuiz);
			previous = node;
			node = LinkedList_Next(&correctQuiz->wordCards);
		}
		fclose(fileCorrectQuiz);
	}
	//2. 개수를 출력한다.
	return correctQuiz->length;
	//3. 끝내다.
}

void Vocabulary_Destroy(Vocabulary* vocabulary) {
	LinkedList_Destroy(&vocabulary->wordCards);
}
void CorrectQuiz_Destroy(CorrectQuiz* correctQuiz) {
	LinkedList_Destroy(&correctQuiz->wordCards);
}

int CompareSpellings(void* one, void* other) {
	WordCard* one_ = (WordCard*)one;
	char(*other_) = (char(*))other;
	int ret;

	ret = strcmp(one_->word.spelling, other_);
	return ret;
}

int CompareMeanings(void* one, void* other) {
	WordCard* one_ = (WordCard*)one;
	char(*other_) = (char(*))other;
	int ret;

	ret = strcmp(one_->word.meaning, other_);
	return ret;
}

int CompareOx(void* one, void* other) {
	WordCard* one_ = (WordCard*)one;
	char(*other_) = (char(*))other;
	int ret;

	ret = strcmp(one_->quiz.ox, other_);
	return ret;
}

int Vocabulary_CompareWordCards(void* one, void* other) {
	WordCard* one_ = (WordCard*)one;
	char(*other_) = (char(*))other;
	int ret;

	if (one_ == other_) {
		ret = 0;
	}
	else {
		ret = -1;
	}
	return ret;
}

int CompareIsImportant(void* one, void* other) {
	WordCard* one_ = (WordCard*)one;
	char(*other_) = (char(*))other;
	int ret;

	ret = strcmp(one_->quiz.isImportant, other_);

	return ret;
}





