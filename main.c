#include <stdio.h>
#include <stdlib.h>

/*

 * Dominick Amaral
 * COP 3502C
 * Lab Assignment 9
 * 4/11/24

 */

// RecordType
struct RecordType{
    int    id;
    char   name;
    int    order;
};

struct ListNode{
    struct RecordType data;
    struct ListNode *next;
};

// Fill out this structure
struct HashType{
    int size;
    struct ListNode** buckets;
};

// Compute the hash function
int hash(int x, int hashSz){

    return x % hashSz;
}

struct HashType* initHashTable(int hashSz){

    struct HashType* ht = malloc(sizeof(struct HashType));
    ht->size = hashSz;
    ht->buckets = malloc(sizeof(struct ListNode*) * hashSz);
    for (int i = 0; i < hashSz; i++)
    {
        ht->buckets[i] = NULL;
    }
    return ht;
}

void insertRecord(struct HashType* ht, struct RecordType record){

    int bucketIdx = hash(record.id, ht->size);
    struct ListNode *newNode = malloc(sizeof(struct ListNode));
    newNode->data = record;
    newNode->next = ht->buckets[bucketIdx];
    ht->buckets[bucketIdx] = newNode;
}

void displayRecordsInHash(struct HashType *ht){

    printf("Hash Table Contents:\n");
    for (int i = 0; i < ht->size; ++i){
        struct ListNode *node = ht->buckets[i];

        if (node != NULL){
            printf("Bucket %d -> ", i);

            while (node != NULL){
                printf("%d %c %d -> ", node->data.id, node->data.name, node->data.order);
                node = node->next;
            }
            printf("NULL\n");
        }
    }
    printf("\n");
}

// Parsing input file and other helper functions remain unchanged
int parseData(char* inputFileName, struct RecordType** ppData){

    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile){
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL){
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i){
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}
void printRecords(struct RecordType pData[], int dataSz){

    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i){
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

void freeHashTable(struct HashType* ht){

    if (ht != NULL){

        for (int i = 0; i < ht->size; i++){
            struct ListNode *current = ht->buckets[i];

            while (current != NULL){
                struct ListNode *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(ht->buckets); // Free the array of bucket pointers
        free(ht);          // Finally, free the hash table structure itself
    }
}

void freeRecords(struct RecordType* records){

    if (records != NULL){
        free(records);
    }
}

int main(void){

    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    int hashTableSize = 10;  // For example, we use a hash table of size 10
    struct HashType* ht = initHashTable(hashTableSize);

    for (int i = 0; i < recordSz; i++){
        insertRecord(ht, pRecords[i]);
    }

    displayRecordsInHash(ht);

    freeHashTable(ht);
    freeRecords(pRecords);

    return 0;
}

