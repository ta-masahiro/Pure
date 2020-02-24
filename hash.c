/* 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

typedef struct Data { 
    char *key; 
    void *val;
} Data;

typedef struct Hash {
    Data *hashTable;
    // int   type;
    int   size, entries;        // サイズ、登録数
} Hash;

Hash * Hash_init(int size);
void Hash_resize(Hash * h, int newSize);     // 内部関数
void Hash_free(Hash *pHash);
int  hash(char *key, int size);                 // 内部関数SH
int  Hash_put(Hash * h, char *key, void *val);
void  ** Hash_get(Hash * h, char *key);
*/
#include "vm.h"
Hash * Hash_init(int size) {
    Hash * h = (Hash * )malloc(sizeof(Hash)); 
    h ->size = size;
    h ->hashTable = (Data * )calloc(size, sizeof(Data));
    h ->entries = 0;
    return h; 
}

void Hash_resize(Hash * h, int newSize) {
    printf("Hash_resize: %d -> %d [%d]\n", h ->size, newSize, h ->entries);
    Data *oldTable = h ->hashTable;
    int oldSize = h ->size;
    Data * newTable = (Data * )calloc(newSize, sizeof(Data));
    h ->hashTable = newTable;
    h ->size = newSize; 
  
    int n;
    for (n = 0; n < oldSize; n++) {
        if (oldTable[n].key != NULL)
            Hash_put(h, oldTable[n].key, oldTable[n].val);
    }
    free(oldTable); 
}

inline int hash(char *key, int size) {
    int n, h = 0;
    for (n = 0; key[n] != '\0'; n++) {
        h = (h * 137 + (key[n]&0xff)) % size;
    }
    // printf("%d\n", h); 
    return h;
}

int Hash_put(Hash * hashT, char *key, void *val) {
    int n, h = hash(key, hashT ->size);
    for (n = 0; n < hashT ->size; n++) {
        int ix = (h + n) % hashT ->size;
        if (hashT ->hashTable[ix].key == NULL) {
            hashT ->hashTable[ix].key = key;        
            hashT ->hashTable[ix].val = val;        
            hashT ->entries++;
            if (hashT ->entries * 3 > hashT ->size * 2)
                Hash_resize(hashT, hashT ->size * 3 / 2);
            return ix;                // 新規登録
        } else if (strcmp(hashT ->hashTable[ix].key, key) == 0) {
            hashT ->hashTable[ix].val = val;        
            return ix;                // 登録済み
        }
    }
    printf("Error: Hash Table Full");
    return -1;
}
/* 
int Hash_isin(Hash * hashT, char * key) {
    if (hashT ->hashTable[hash(key, hashT ->size)].key == NULL) return FALSE; 
    return TRUE; 
}
*/ 
void  **Hash_get(Hash * hashT, char *key) {
    int n, h = hash(key, hashT ->size);
    for (n = 0; n < hashT ->size; n++) {
        int ix = (h + n) % hashT ->size;
        if (hashT ->hashTable[ix].key == NULL) {
            return NULL;                // 登録なし
        } else if (strcmp(hashT ->hashTable[ix].key, key) == 0) {
            return &(hashT ->hashTable[ix].val);   // 登録あり
        }
    }
    return NULL;
}

void print_hashTable(Hash * h) {
    int i; 
    for(i = 0;  i < (h ->size); i ++ ) {
        if (h ->hashTable[i].key != NULL) 
            printf("i:%d key:%s val:%ld\n",i , h ->hashTable[i].key, (long)(h ->hashTable[i].val));  
    }
}
/* 
int main() {
    Hash * h = Hash_init(2);
    Hash_put(h, "1", "one");
    Hash_put(h, "2", "two");
    Hash_put(h, "3", "three");
    Hash_put(h, "4409877654321", "three");
    Hash_put(h, "5", "five");
    Hash_put(h, "skjglkdjglkdsjgk", "123"); 
    Hash_put(h, "11", "123"); 
    printf("4->%s\n", Hash_get(h, "4"));
    printf("2->%s\n", Hash_get(h, "2"));
    printf("1->%s\n", Hash_get(h, "1"));
    printf("5->%s\n", Hash_get(h, "5"));
    printf("3->%s\n", Hash_get(h, "3"));

    print_hashTable(h); 
}
*/ 
