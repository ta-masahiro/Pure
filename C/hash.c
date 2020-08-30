#include "vm.h"

int ilog2(int x) { 
    int c = 0, y = x - 1;
    while ((y = y>>1)>0) c ++ ; 
    return c; 
}

Hash * Hash_init(int size) {
    Hash * h = (Hash * )malloc(sizeof(Hash)); 
    h ->size = 2<<ilog2(size);
    h ->hashTable = (Data * )calloc(h -> size, sizeof(Data));
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
        h = (h * 137 + (key[n]&0xff)) & (size - 1);
    }
    // printf("%d\n", h); 
    return h;
}

int Hash_put(Hash * hashT, char *key, void *val) {
    int n, h = hash(key, hashT ->size);
    for (n = 0; n < hashT ->size; n++) {
        int ix = (h + n) & (hashT->size - 1);
         if (hashT ->hashTable[ix].key == NULL) {
         // if (hashT ->hashTable[ix] == NULL) {
            hashT ->hashTable[ix].key = key;        
            hashT ->hashTable[ix].val = val;        
            hashT ->entries++;
            if (hashT ->entries * 4 > hashT ->size * 3)
                Hash_resize(hashT, hashT ->size * 2);
            return ix;                // 新規登録
        } else if (strcmp(hashT ->hashTable[ix].key, key) == 0) {
            hashT ->hashTable[ix].val = val;        
            return ix;                // 登録済み
        }
    }
    printf("Error: Hash Table Full");
    return -1;
}

void  **Hash_get(Hash * hashT, char *key) {
    int n, h = hash(key, hashT ->size);
    for (n = 0; n < hashT ->size; n++) {
        int ix = (h + n) & (hashT ->size - 1);
         if (hashT ->hashTable[ix].key == NULL) {
         // if (hashT ->hashTable[ix] == NULL) {
            return NULL;                // 登録なし
        } else if (strcmp(hashT ->hashTable[ix].key, key) == 0) {
            return &(hashT ->hashTable[ix].val);   // 登録あり
        }
    }
    return NULL;
}
 /* 
void Hash_delete(Hash * hashT, char *key) {
    int n, h = hash(key, hashT ->size);
    for (n = 0; n < hashT ->size; n++) {
        int ix = (h + n) & (hashT ->size - 1);
        if (hashT ->hashTable[ix].key == NULL) {
         // if (hashT ->hashTable[ix] == NULL) {
            printf("key not exisit!\n"); 
            return ;                // 登録なし
        } else if (strcmp(hashT ->hashTable[ix].key, key) == 0) {
            hashT ->hashTable[ix].key = NULL;   // 登録あり
            return; 
        }
    }
    printf("key not exisit!\n"); 
    return ;
}
 */ 
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
