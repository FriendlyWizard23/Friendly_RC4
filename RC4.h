#include <stdio.h>
#include <time.h>
#include <string.h>
#define K_SIZE 256
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
static void KSA(unsigned char[],unsigned char[]);
static void swap(unsigned char*,unsigned char*);
static void PRNG(unsigned char[],unsigned char[],unsigned char[]);
unsigned char* RC4Encrypt(unsigned char[],unsigned char[]);
unsigned char* RC4Decrypt(unsigned char[],unsigned char[]);
unsigned char* genKey();

static int generateRandom();
static int isIllegalValue(int);
int unsetMem(unsigned char*);

void printHex(unsigned char[]);
void printChar(unsigned char[]);
void printPlain(unsigned char[]);
//////////////////////////////////////////////////////////////////////////////
///////////////////// MAIN ALGOTIHM BLOCK ////////////////////////////////////
static void KSA(unsigned char s[],unsigned char k[]){
    int keylen=strlen(k);
    for(int i=0;i<K_SIZE;i++){
        s[i]=i;
    }
    int j=0;
    int i=0;
    for (i=0;i<K_SIZE;i++){
        j=((j+s[i]+k[i%keylen])%K_SIZE);
        swap(&s[i],&s[j]);
    }
}

static void swap(unsigned char *first,unsigned char *second){
    unsigned char sup=*first;
    *first=*second;
    *second=sup;
}

static void PRNG(unsigned char s[],unsigned char plaintext[],unsigned char ciphertext[]){
    unsigned char i=0;
    unsigned char j=0;
    unsigned char t=0;
    int count=0;
    unsigned char k=0;
    for(count=0;count<strlen(plaintext);count++){
        i=(i+1)%K_SIZE;
        j=(j+s[i])%K_SIZE;
        swap(&s[i],&s[j]);
        t=(s[i]+s[j])%K_SIZE;
        k=s[t];
        ciphertext[count]=plaintext[count]^k;
    }
}

unsigned char* RC4Encrypt(unsigned char key[],unsigned char plaintext[]){
    unsigned char *s=malloc(K_SIZE*sizeof(unsigned char));
    unsigned char *ciphertext=malloc(K_SIZE*sizeof(unsigned char));
    KSA(s,key);
    PRNG(s,plaintext,ciphertext);
    return ciphertext;

}

unsigned char* RC4Decrypt(unsigned char key[],unsigned char ciphertext[]){
    unsigned char *s=malloc(K_SIZE*sizeof(unsigned char));
    unsigned char *plaintext=malloc(K_SIZE*sizeof(unsigned char));
    KSA(s,key);
    PRNG(s,ciphertext,plaintext);
    return plaintext;
}

///////////////////// END OF MAIN ALGOTIHM BLOCK ///////////////////////////////
///////////////////// MAIN SUPPORT FUNCTIONS BLOCK /////////////////////////////
static int generateRandom(){
    int UPPER_BOUND=254;
    int LOWER_BOUND=33;
    int flag=1;
    int genRan=0;
    while(1){
        genRan=(rand()%(UPPER_BOUND-LOWER_BOUND+1))+LOWER_BOUND;
        if (isIllegalValue(genRan)==0){
            return genRan;
        }
    }
}
static int isIllegalValue(int genRan){
    int exclude[]={39,127,171,172,254}; //ADD or REMOVE values if you want to include/exclude byte values in the random generator
    int arraySize=sizeof(exclude)/sizeof(exclude[0]);
    for(int j=0;j<arraySize;j++){
            if (genRan==exclude[j]){
                return 1;
            }
    }
    return 0;
}
unsigned char* genKey(){
    srand ((unsigned int)time(NULL));
    unsigned char *key = malloc(K_SIZE*(sizeof(char)));
    int i;
    int genRan;
    for (i = 0; i < K_SIZE; i++){
        genRan=generateRandom();
        key[i] =(unsigned char)genRan;
    }
    return key;
}
int unsetMem(unsigned char* tofree){
    if (tofree!=NULL){
        free(tofree);
        return 1;
    }
    return 0;
}
///////////////////// END OF MAIN ALGOTIHM BLOCK ///////////////////////////////
///////////////////// DEBUG FUNCTIONS //////////////////////////////////////////
void printHex(unsigned char text[]){
    for (int i=0;i<K_SIZE;i++){
        printf("%x",text[i]);

    }
    printf("%s","\n\n");
}
void printPlain(unsigned char plain[]){
    for (int i=0;i<strlen(plain);i++){
        printf("%x ",plain[i]);
    }
    printf("%s","\n\n");
}
void printChar(unsigned char text[]){
    for (int i=0;i<K_SIZE;i++){
        printf("%c",text[i]);
    }
    printf("%s","\n\n");
}
/////////////////// END DEBUG FUNCTIONS //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
