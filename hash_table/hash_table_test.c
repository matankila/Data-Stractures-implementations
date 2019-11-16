#include <stdio.h>      /* IO fucntion */ 
#include <string.h>     /* strcmp      */
#include <stdlib.h>     /* malloc      */

#include "hash_table.h" /* my header   */

#define RANGE 17576
#define PRINT_ERR(string, line, err) printf("%s %d\n", string, line); \
                                     ++err;
#define ALLOC_FAIL 1
#define SUCCESS 0
#define LIMIT 3

int err = 0;
int tests = 0;

typedef struct word_t word_t;
struct word_t
{
	char *str;
	size_t encounters;
};

/*****************************************************************************/
unsigned int HashIt(void *const data, void *const param);
unsigned int HashIt2(void *const data, void *const param);
int Compare(const void *internal_data, 
	                           const void *external_data, void *param);
int Compare2(const void *internal_data, 
	                           const void *external_data, void *param);
int PrintMe(void *data, void *param);
int PrintMe2(void *data, void *param);
void CasualTest();
int Histogram();
/*****************************************************************************/

int main()
{
	/*CasualTest();*/
	Histogram();

	if (err)
	{
		printf("you failed it go fix\n");
	}
	else
	{
		printf("you aced it DUCAS/IT\n");
	}

	return (SUCCESS);
}

unsigned int HashIt(void *const data, void *const param)
{
	char *str = data;
	int i = 0;
	int ascii_count = 0;

	(void)param;

	for (i = 0; i < 3 && '\0' != *str; ++i, ++str)
	{	
		ascii_count += *str;
	}

	return (ascii_count % RANGE);
}

unsigned int HashIt2(void *const data, void *const param)
{
	char *str = ((word_t *)data)->str;
	int i = 0;
	int ascii_count = 0;

	(void)param;

	for (i = 0; i < 3 && '\0' != *str; ++i, ++str)
	{	
		ascii_count += *str;
	}

	return (ascii_count % RANGE);
}

int PrintMe(void *data, void *param)
{
	(void)param;

	printf("%s\n", (char *)data);

	return SUCCESS;
}

int PrintMe2(void *data, void *param)
{
	(void)param;

	printf("enc %ld string:%s\n", ((word_t *)data)->encounters, 
		                          ((word_t *)data)->str);
	free(data);

	return SUCCESS;
}

int FreeAll(void *data, void *param)
{
	free(data);

	return SUCCESS;
}

int Compare(const void *internal_data, const void *external_data, void *param)
{
	(void)param;

	return (strcmp(internal_data, external_data));
}

int Compare2(const void *internal_data, const void *external_data, void *param)
{
	int res = 0;

	(void)param;

	res = strcmp(((word_t *)internal_data)->str,((word_t *)external_data)->str);
	if (0 == res)
	{
		((word_t *)internal_data)->encounters += 1;
	}

	return res;
}

void CasualTest()
{
	h_table_t *hash = NULL;
	int returned_status = 0;
	char *str_for_compare = "you";

	hash = HashCreate(RANGE, HashIt, Compare, NULL);
	if (NULL == hash)
	{
		PRINT_ERR("creation failed on line", __LINE__, err);
	}

	if (0 == HashIsEmpty(hash))
	{
		PRINT_ERR("isempty failed on line", __LINE__, err);
	}

	if (0 < HashSize(hash))
	{
		PRINT_ERR("size failed on line", __LINE__, err);
	}

    if (NULL != (char*)HashFind(hash, "you"))
	{
		PRINT_ERR("find failed on line", __LINE__, err);
	}

	if (1 != HashForEach(hash, PrintMe, NULL))
	{
		PRINT_ERR("foreach failed on line", __LINE__, err);
	}

	returned_status = HashInsert(hash, "you");
	if (returned_status == 1 || returned_status == 2)
	{
		PRINT_ERR("insertion failed/dup on line", __LINE__, err);
	}

	if (1 == HashIsEmpty(hash))
	{
		PRINT_ERR("isempty failed on line", __LINE__, err);
	}

	if (0 == HashSize(hash))
	{
		PRINT_ERR("size failed on line", __LINE__, err);
	}

	returned_status = HashInsert(hash, "your");
	if (returned_status == 1 || returned_status == 2)
	{
		PRINT_ERR("insertion failed/dup on line", __LINE__, err);
	}

	if (1 == HashIsEmpty(hash))
	{
		PRINT_ERR("isempty failed on line", __LINE__, err);
	}

	if (1 > HashSize(hash))
	{
		PRINT_ERR("size failed on line", __LINE__, err);
	}

	/* HashForEach(hash, PrintMe, NULL); */

	if (str_for_compare != (char*)HashFind(hash, "you"))
	{
		PRINT_ERR("find failed on line", __LINE__, err);
	}

	HashRemove(hash, "you");
	if (1 == HashIsEmpty(hash))
	{
		PRINT_ERR("isempty failed on line", __LINE__, err);
	}

	if (1 != HashSize(hash))
	{
		PRINT_ERR("size failed on line", __LINE__, err);
	}

	HashDestroy(hash);
}

int Histogram()
{
    char *buffer = NULL;
    char *buffer_copy = NULL;
    int string_size, read_size;
    FILE *handler = fopen("h.txt", "r");
    char tokens[] = " .,-/\n\t\"';:*()?!#$@^&*[]{}~`><=+|";
   	h_table_t *hash = NULL;
	int returned_status = 0;
	char *token = NULL;
	word_t **w = NULL;
	int i = 0;
	size_t counter = 0;

	hash = HashCreate(RANGE, HashIt2, Compare2, NULL);
	if (NULL == hash)
	{
		return ALLOC_FAIL;
	}

    if (handler)
    {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        rewind(handler);

        buffer = (char*) malloc(sizeof(char) * (string_size + 1));
        if (NULL == buffer)
        {
        	HashDestroy(hash);

        	return ALLOC_FAIL;
        }
        buffer_copy = (char*) malloc(sizeof(char) * (string_size + 1));
        if (NULL == buffer_copy)
        {
        	free(buffer);
        	buffer = NULL;
        	HashDestroy(hash);

        	return ALLOC_FAIL;
        }

        read_size = fread(buffer, sizeof(char), string_size, handler);
        buffer[string_size] = '\0';
        if (string_size != read_size)
        {
            free(buffer);
            free(buffer_copy);
            HashDestroy(hash);
            buffer = NULL;
        }
        memcpy(buffer_copy, buffer, (string_size + 1));
        fclose(handler);
    }

    /* calc size for array */
    token = strtok(buffer, tokens);
    while ( token != NULL ) 
    {   
        token = strtok(NULL, tokens);
        ++counter;
    }
    free(buffer);
    buffer = NULL;

    w = malloc(sizeof(word_t) * counter);
    if (NULL == w)
    {
    	free(buffer);
    	buffer = NULL;
        free(buffer_copy);
        buffer_copy = NULL;
        HashDestroy(hash);

        return ALLOC_FAIL;
    }

    token = strtok(buffer_copy, tokens);
    while (token != NULL) 
    {   
    	w[i] = malloc(sizeof(word_t));
    	if (NULL == w[i])
    	{
    		free(buffer);
    		buffer = NULL;
        	free(buffer_copy);
        	buffer_copy = NULL;
        	HashForEach(hash, FreeAll, NULL);
        	HashDestroy(hash);
        	free(w);

    		return ALLOC_FAIL;
    	}
    	
    	w[i]->str = token;
    	w[i]->encounters = 1;
    	returned_status = HashInsert(hash, w[i]);
    	if (returned_status == 2)
    	{
    		free(w[i]);
    	}

        token = strtok(NULL, tokens);
        ++i;
    }

    HashForEach(hash, PrintMe2, NULL);

    HashDestroy(hash);
    free(buffer);
   	free(buffer_copy);
    free(w);

    return SUCCESS;
}