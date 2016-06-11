
/* Platform family (optional) */

/* OS spec. */
#define _x86_64
#define DARWIN

/* OS Family */
#define UNIX

/* Compiler spec. */
#define GNU

/* Protection flag */
#define BALISE_FREE

/* Number sizes */
/* #define HAS_16BINT */
#define HAS_64BINT

/* Function / include specificities */
/* #define HAS_NINT */
/* #define HAS_INDEX */
#define HAS_MEMORY_H
/* #define HAS_MALLOC_T */
/* #define HAS_REGEXP */
#define REGEXP_TYPE char *
#define NEW_SIZE_TYPE size_t
/* #define HAS_SIG_PF */
#define HAS_STRDUP
/* #define NOPIPE */
/* #define NOSIGNALS */
#define HAVE_UNISTD_H

/* Misc */
/* #define USE_OLD_ENCRYPT  */
#define CPP_COMMAND "cpp"


/** Devenu inutile, car kernel/makedefs spécifie -DSTATIC_PARSER **/
#define DEFAULT_PARSER_DLL	"defparser"

#define	SYS_PROF_TICK 10000

#define ANSI_CLASS_INST

#define POSIX_THREADS

#define NEW_INLINE
#define NEW_THROW throw(std::bad_alloc)

#define EXIV2_0_21
