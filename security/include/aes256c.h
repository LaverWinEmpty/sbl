#ifndef SBL_AES_256_C_HEADER__
#define SBL_AES_256_C_HEADER__

#ifndef IN
#    define IN
#endif
#ifndef OUT
#    define OUT
#endif
#ifndef OPT
#    define OPT
#endif

// Must call free procedure

char* AES256_Encrypt(IN const void* data, IN int size, IN const char* key);
char* AES256_Decrypt(IN const void* data, IN int size, IN const char* key, OUT OPT int* padding);

#endif