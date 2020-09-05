#include "vm.h"

unsigned long ilog2(unsigned long x) { 
    unsigned long c = 0, y = x - 1;
    while ((y = y>>1)>0) c ++ ; 
    return c; 
}

Hash * Hash_init(unsigned long size) {
    Hash * h = (Hash * )malloc(sizeof(Hash));
    h ->initval = 0;  
    h ->size = (unsigned long)2<<ilog2(size);
    h ->hashTable = (Data * )calloc(h -> size, sizeof(Data));
    h ->entries = 0;
    return h; 
}

void Hash_resize(Hash * h, unsigned long newSize) {
    printf("Hash_resize: %ld -> %ld [%ld]\n", h ->size, newSize, h ->entries);
    Data *oldTable = h ->hashTable;
    unsigned long oldSize = h ->size;
    unsigned long initval = h ->initval; 
    Data * newTable = (Data * )calloc(newSize, sizeof(Data));
    h ->hashTable = newTable;
    h ->size = newSize; 
    h ->initval = initval; 
  
    long n;
    for (n = 0; n < oldSize; n++) {
        if (oldTable[n].key != NULL)
            Hash_put(h, oldTable[n].key, oldTable[n].val);
    }
    free(oldTable); 
}
typedef unsigned long ub4;
#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

unsigned long hash( register unsigned char *k, register unsigned long length, register unsigned long initval) {
    register unsigned long a,b,c,len;
    len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */
    while (len >= 12) {
        a += (k[0] +((ub4)k[1]<<8) +((ub4)k[2]<<16) +((ub4)k[3]<<24));
        b += (k[4] +((ub4)k[5]<<8) +((ub4)k[6]<<16) +((ub4)k[7]<<24));
        c += (k[8] +((ub4)k[9]<<8) +((ub4)k[10]<<16)+((ub4)k[11]<<24));
        mix(a,b,c);
        k += 12; len -= 12;
    }
    c += length;
    switch(len) {       /* all the case statements fall through */
        case 11: c+=((ub4)k[10]<<24);
        case 10: c+=((ub4)k[9]<<16);
        case 9 : c+=((ub4)k[8]<<8);
        case 8 : b+=((ub4)k[7]<<24);
        case 7 : b+=((ub4)k[6]<<16);
        case 6 : b+=((ub4)k[5]<<8);
        case 5 : b+=k[4];
        case 4 : a+=((ub4)k[3]<<24);
        case 3 : a+=((ub4)k[2]<<16);
        case 2 : a+=((ub4)k[1]<<8);
        case 1 : a+=k[0];
    }
   mix(a,b,c); return c;
}

unsigned long Hash_put(Hash * hashT, Symbol *key, void *val) {
    unsigned long n, h = hash(key ->_table, key ->_size, hashT ->initval) & (hashT ->size-1);
    //hashT->initval = h;
    //printf("%s %ld\n",key->_table,h);
    //printf("hash size:%ld\n",key->_size);
    for (n = 0; n < hashT ->size; n++) {
        unsigned long ix = (h + n) & (hashT->size - 1);
        if (hashT ->hashTable[ix].key == NULL) {
            hashT ->hashTable[ix].key = key;        
            hashT ->hashTable[ix].val = val;        
            hashT ->entries++;
            if (hashT ->entries * 4 > hashT ->size * 3)
                Hash_resize(hashT, hashT ->size * 2);
            return ix;                // 新規登録
        } else if (memcmp((hashT ->hashTable[ix].key) ->_table, key ->_table, key ->_size) == 0) {
            hashT ->hashTable[ix].val = val;        
            return ix;                // 登録済み
        }
    }
    printf("Error: Hash Table Full");
    return 0;
}

void  **Hash_get(Hash * hashT, Symbol *key) {
    unsigned long n, h = hash(key ->_table, key ->_size, hashT ->initval) & (hashT->size -1);
    //hashT->initval = h;
    for (n = 0; n < hashT ->size; n++) {
        unsigned long ix = (h + n) & (hashT->size  - 1);
        if (hashT ->hashTable[ix].key == NULL) {
            //printf("ID:%ld KEY:%s is not exeist!\n",ix,key->_table);
            return NULL;                // 登録なし
        } else if (memcmp((hashT ->hashTable[ix].key)->_table, key ->_table, key ->_size) == 0) {
            return &(hashT ->hashTable[ix].val);   // 登録あり
        }
        printf("!!!!\n");
    }
    return NULL;
}

void print_hashTable(Hash * h) {
    unsigned long i; 
    for(i = 0;  i < (h ->size); i ++ ) {
        if (h ->hashTable[i].key != NULL) 
            printf("i:%ld key:%s val:%ld\n",i , (h ->hashTable[i].key) ->_table, (long)(h ->hashTable[i].val));  
    }
}
/*
Symbol * new_symbol(char * str, int size) {
    char * s = (char * )malloc(size * sizeof(char) + 1); 
    strcpy(s, str); // printf("%s\n", s);
     // if (strcmp(s, "LDC") == 0) printf("!LDC!\n");  
    Symbol * sym = (Symbol * )malloc(sizeof(Symbol));
    sym -> _size = size; 
    sym -> _table = s;  
    return sym;  
}

int main(int argc, char *  argv[]) {
    char strbuff[2048]; 
    Symbol * key; 
    Hash * h = Hash_init(4096); 
    long i=0;
    while (TRUE) {
        fgets(strbuff,2047,stdin);
        key = new_symbol(strbuff, strlen(strbuff));     
        Hash_put(h, key,(void*)i); 
        print_hashTable(h);
        i++; 
    }
}
*/