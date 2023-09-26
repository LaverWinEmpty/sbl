#ifndef SBL_SHA_256_C_HEADER__
#define SBL_SHA_256_C_HEADER__

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

char* SHA256_Hasing(IN const void* message, IN int size);
char* SHA256_Encrypt(IN const void* message, IN int msgSize, IN const void* salt, IN int saltSize, IN int stretching);

#endif