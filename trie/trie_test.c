#include <stdio.h>
#include "trie.h"
#include "barr.h"

int main()
{
	trie_t *trie = TrieCreateTrie(32);
	number_t new_num = 0; 
	unsigned long number_to_insert = 0x80000000;
	unsigned long number_to_insert1 = 0x00000000;
	unsigned long n1 = 0xc0000000;
	unsigned long n2 = 0x40000000;
	int status = 0;

	BitsArrayPrintBinary(number_to_insert);

	printf("is empty? %d\n", TrieIsEmpty(trie));
	printf("size: %ld\n", TrieSize(trie));

	status = TrieInsert(trie, number_to_insert);
	printf("stat %d\n", status);

	printf("size: %ld\n", TrieSize(trie));

	status = TrieInsert(trie, number_to_insert1);
	printf("stat %d\n", status);
	
	status = TrieInsert(trie, number_to_insert);
	printf("stat %d\n", status);

	status = TrieInsert(trie, n1);
	printf("stat %d\n", status);	

	status = TrieInsert(trie, n2);
	printf("stat %d\n", status);

	TrieNextUnusedNumber(trie, n2, &new_num);
	printf("%d\n", new_num);

	printf("is empty? %d\n", TrieIsEmpty(trie));
	printf("size: %ld\n", TrieSize(trie));

	TrieRemove(trie, number_to_insert);
	printf("size: %ld\n", TrieSize(trie));

	TrieRemove(trie, number_to_insert1);
	printf("size: %ld\n", TrieSize(trie));

	TrieRemove(trie, n1);
	printf("size: %ld\n", TrieSize(trie));

	printf("yyyyyyyyyyy%d\n", TrieRemove(trie, n2));
	printf("size: %ld\n", TrieSize(trie));
	printf("xxxxxxxxxxx%ld\n", TrieCountInsertedNums(trie));

	printf("size: %ld\n", TrieSize(trie));

	TrieDestroy(trie);

	return 0;
}