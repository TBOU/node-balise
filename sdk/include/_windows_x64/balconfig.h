/* Configuration: hardware _ OS */
#define	BalARCH	"x64_Windows"

/* Platform family (optional) */
/* #define SUN4 */
/* #define RISC */
#define PC

/* OS spec. */
/* #define SUNOS */
/* #define SOLARIS */
/* #define BOSF */
/* #define AIX */
#define _AMD64_
#undef _X86_
#define WINDOWS
#define WINNT
#define MSDOS
#ifndef DLL
#define DLL
#endif

/* OS Family */
/* #define UNIX */
#define DOS

/* Compiler spec. */
/* #define ATT */
/* #define GNU */
/* #define BORLAND */
#define MSVC

#pragma warning ( disable : 4996 ) // "strdup" disapproved
#pragma warning ( disable : 4355 ) //'this' used un base member initializer list

#ifndef EXPORT_
#define EXPORT_ __declspec(dllexport)
#define EXPORTED_(spec_) __declspec(dllexport) spec_
#endif

/* Protection flag  (invert if free)*/
#define PROTECT
#undef BALISE_FREE
#define PROTECT_WITH_MAC_ADDRESS

/* Number sizes */
#define HAS_64BINT

/* Function / include specificities */
/* #define HAS_NINT */
/* #define HAS_INDEX */
/* #define HAS_MEMORY_H */
/* #define HAS_MALLOC_T */
/* #define HAS_REGEXP */
#define REGEXP_TYPE char *
#define NEW_SIZE_TYPE size_t
/* #define HAS_SIG_PF */
#define HAS_STRDUP
#define NOPIPE
#define NOSIGNALS
#define NO_RAND48
#define NO_DIRENT
#define CONSOLE
#define NEW_INLINE

/* Misc */
/* #define USE_OLD_ENCRYPT  */
//#define CPP_COMMAND "cpp"
#define CPP_COMMAND "b2ppnt"

/** Devenu inutile (sauf pour SDK), car kernel/makedefs spécifie -DSTATIC_PARSER **/
#ifdef BAL_UNICODE
#define	DEFAULT_PARSER_DLL "dlibusp"
#else
#define	DEFAULT_PARSER_DLL "dlibsp"
#endif


#define WIN_THREADS

