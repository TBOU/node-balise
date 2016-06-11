#ifndef _Included_balibase_
#define _Included_balibase_

/*
// ##notice Xavier FRANC
//--------------------------------------------------------------------------------
//
//	Balise
//
//
//
//
//
//--------------------------------------------------------------------------------
*/

#include "balconfig.h"
#include "balisyms.h"
#include <stdio.h>

/* --------------- macros ----------------- */

#ifdef KRC	/* K & R */

#define	Args_(args)	()
#define	NoArgs_		()
#define	BaliEXT		extern
#define CONST

#elif defined(ANSIPROTO)
/* --- ANSI interface --- */

#define	Args_(args)	args
#define	NoArgs_		(void)
#define	BaliEXT		extern
#define CONST

#else
/* --- C++ interface --- */

#define	Args_(args)	args
#define	NoArgs_		(void)
#define	BaliEXT		extern "C"
#define CONST 		const

#endif

#ifndef EXPORT_
#define EXPORTED_(spec_) spec_
#ifdef DLL
#  if defined(BORLAND)
#    define EXPORT_  far pascal _export
#  elif defined(WATCOM)
#    define EXPORT_ __export __stdcall
#  elif defined(MSVC)
#    define EXPORT_ __declspec(dllexport)
#    define API_ __stdcall
#  else
#    define EXPORT_   __export __far __pascal
#  endif
#else
#define EXPORT_
#endif
#endif

#ifndef API_
#define API_
#endif

#ifndef nil
#define nil     (0)
#endif

#ifndef false
#define	false	0
#endif

#ifndef true
#define	true	(!false)
#endif

#ifndef ArraySize
#define ArraySize(a)   (sizeof(a)/sizeof(a[0]))
#endif

/* -------- Memory allocation -----------------------------------------
 *
 */

#ifndef ANSIPROTO
#ifndef NEW_THROW
#define NEW_THROW
#else
#include <new>
#endif
#endif


#ifdef MSVC
BaliEXT void	EXPORT_ * b2malloc (NEW_SIZE_TYPE size);
BaliEXT void	EXPORT_ * b2realloc (void *object, NEW_SIZE_TYPE size);
#else
BaliEXT void	* EXPORT_ b2malloc (NEW_SIZE_TYPE size);
BaliEXT void	* EXPORT_ b2realloc (void *object, NEW_SIZE_TYPE size);
#endif
BaliEXT void	EXPORT_ b2free (void *object);


#ifndef ANSIPROTO

#ifdef NEW_INLINE
inline	void		operator delete (void *object)
{
	b2free(object);
}

inline	void		*operator new (NEW_SIZE_TYPE size) NEW_THROW
{
	return b2malloc(size);
}

# ifndef MSVC
inline	void		*operator new[] (NEW_SIZE_TYPE size) NEW_THROW
{
	return b2malloc(size);
}

inline	void		operator delete[] (void *object)
{
	b2free(object);
}

#endif

#else
/*------ not inline -----*/

extern	void		*operator new (NEW_SIZE_TYPE size) NEW_THROW;
# if !defined(MSVC) && !defined(SPARCWORKS)
extern	void		*operator new[] (NEW_SIZE_TYPE size) NEW_THROW;
# endif
#if !defined(SPARCWORKS)
extern	void		operator delete (void *object);
extern	void		operator delete[] (void *object);
#endif

#endif   // not inline

#endif


/* -------- Balise basic Types ---------------------------------------- */

#ifdef BaliAPI_NON_OPAQUE

/* True implementation representation of the types */
class B2Object;
class B2Stream;
class B2FileStream;
class B2Process;
class B2Document;
class EventReceiver;
class BalCodingSystem;

#else
/* opaque implementation */
typedef	struct _B2Obj	B2Object;
typedef	struct _B2Obj	B2Stream;
typedef	struct _B2Obj	B2FileStream;
typedef	struct _B2Obj	B2Document;
typedef	struct _B2Proc	B2Process;
typedef	struct _EvRcv	EventReceiver;
typedef	struct _CSystem	BalCodingSystem;
#endif

#ifdef HAS_32BINT
typedef	int		BaliInt;
#endif

#ifdef HAS_64BINT
typedef	int		BaliInt;
#endif

#ifdef HAS_16BINT
typedef	long		BaliInt;
#endif

#define	BaliVoid	void
typedef	unsigned char	BaliBool;

typedef CONST char *    SStatus;
typedef	BaliInt		BaliStatus;
typedef	double		BaliDouble;

typedef	char		BaliChar;
typedef	unsigned short	BaliUChar;	/* Unicode character */
typedef	CONST char *	BaliString;
typedef	char *	BaliMString;

typedef	B2Object       *BaliObject;
typedef	B2Stream       *BaliStream;
typedef	B2FileStream   *BaliFileStream;
typedef	B2Process      *BaliProcess;
typedef	B2Document     *BaliDocument;

/* Character data are defined by a type BaliXString that depends on
   a compilation option BAL_UNICODE : unsigned short or unsigned char */
#ifdef	BAL_UNICODE
typedef	BaliUChar	BaliXChar;
#else
typedef	unsigned char	BaliXChar;
#endif

typedef	BaliXChar	*BaliXString;

/* internal aliases */
#ifndef AIS_EXT
typedef	BaliXChar	XChar;
#endif
typedef	BaliUChar	UChar;

typedef	BaliInt		Int;

typedef signed char	Small;		// 8 bits signed
typedef unsigned char	Byte;		// 8 bits
typedef signed short int Short;		// 16 bits signed
typedef	float		Float;		// single precision



#define	EXT_BIN_FUNCTION(name) \
	BaliObject  name Args_((BaliObject *args, BaliInt argc, BaliProcess process))

#ifdef MSVC
int f();
typedef BaliObject  (*
	BaliExternFun ) Args_((BaliObject *args, BaliInt argc, BaliProcess process));
#else
typedef	EXT_BIN_FUNCTION( (EXPORTED_(*) BaliExternFun) );
#endif

#if defined(KRC)
# define BaliFUNCTION(name) \
     BaliObject  EXPORT_ name ( args, argc, process) \
				BaliObject *args; \
				BaliInt argc; \
				BaliProcess process;
#else
# define BaliFUNCTION(name) \
    BaliObject EXPORT_ name Args_((BaliObject *args, BaliInt argc, BaliProcess process))
#endif

BaliEXT FILE * BaliStdin;
BaliEXT FILE * BaliStdout;
BaliEXT FILE * BaliStderr;

/* -------- Balise extensions modules ----------------------------------- */

#define BALI_EXT_EVENTS		"BaliExt_EventGenerator"
#define BALI_EXT_FUNCTIONS	"BaliExt_Functions"

/* for future use: */
#define BALI_EXT_INIT		"BaliExt_Initialize"
#define BALI_EXT_DEBUGGER	"BaliExt_Debugger"
#define BALI_EXT_CODING		"BaliExt_CodingSystem"

 /* External function set spec. in extension dynamic library */
typedef struct {
	BaliString	name;
	BaliExternFun	fun;
	BaliInt		argc;
	BaliBool	dot;
}  BaliExternEntry;

/* External Coding System spec. in extension dynamic library */
typedef struct {
	BaliString	name;
	BalCodingSystem	* (*create) ();
}  BaliExternCS;

/* -------- Option parsing ----------------------------------------------- */

typedef enum {
	BOA_set,	/* set String variable (arg = address)	*/
	BOA_option,	/* set global option (arg = name)	*/
	BOA_function,	/* call function (arg = function ptr)	*/
	BOA_args,	/* special: get all following args	*/
	BOA_pp,		/* special: pass to preprocessor	*/
	BOA_help,	/* special: just print usage	*/
	BOA_print,	/* special: just show form when printing usage	*/
	BOA_demo	/* special: process optional demo flag */
}  BaliOptionAct;

typedef struct
{
	BaliString	form;		/* option name + arg style	*/
	BaliOptionAct	act;
	int		*argument;
	BaliString	help;		/* for usage printing	*/
}  BaliOptionEntry;

/* --------- Object types ------------------------------------------------ */

typedef enum {
	BType_Void,
	BType_Boolean,
	BType_Number,
	BType_String,
	BType_Map,
	BType_Set,
	BType_List,
	BType_Function,
	BType_StringStream,
	BType_FileStream,
	BType_LinkedString,
	BType_UserType,
	BType_RegExp,
	BType_Node,
	BType_Document,
	BType_Process,
	BType_Match,
	BType_Iterator,
  BType_CodingSystem
} BaliType;

/* --------- Exception codes --------------------------------------------- */

typedef enum {
	BERR_None = 0,
	BERR_Type = 1,
	BERR_System = 2,
	BERR_FileAccess = 3,
	BERR_Attr = 4,
	BERR_ModAttr = 5,
	BERR_Index = 6,
	BERR_OddCount = 7,
	BERR_DupKey = 8,
	BERR_NoSuchFun = 9,
	BERR_SGML = 10,
	BERR_GlobVar = 11,
	BERR_Field = 12,
	BERR_NotUserType = 13,
	BERR_User = 14,
	BERR_Tree = 15
} BaliError;


/* --------- security masks : -------------------------------------------- */

#define	SECU_SYSTEM	(1 << 0)
#define	SECU_FILE_W	(1 << 1)
#define	SECU_FILE_R	(1 << 2)
#define	SECU_DOC_W	(1 << 3)
#define	SECU_DOC_R	(1 << 4)
#define	SECU_SDK	(1 << 5)
	/* 16 first bits reserved: others can be used in SDK */
#define	SECU_LAST_USED	(1 << 15)


/* Event Handlers return status, allows control of the Event Generation process. */
typedef	enum EventReturn
{
	EventReturn_Normal = 0,
	EventReturn_Abort = -1,		/* abort parsing */
	EventReturn_Terminate = -2	/* stop parsing but send event 'terminate' */
} EventReturn;

/* --------- Custom Streams -------------------------------------------- */

/* NOTE: the methods in this structure may be null pointers: in which case they are
   considered not implemented.
 */
typedef struct CustomStreamClass
{
	/* identifier of the class: must be unique */
	BaliString	name;

	/* startup method: called before any other operation */
	BaliStatus	API_ (* startup) Args_ ((void *assocData));

	/* cleanup method: called after close (or implicitly on destruction) */
	BaliStatus	API_ (* cleanup) Args_ ((void *assocData));

	/* buffered byte IO */
	BaliInt		API_ (* write) Args_((void *assoc, char *bytes, BaliInt size));
	BaliInt		API_ (* read) Args_((void *assoc, char *bytes, BaliInt size));

	/* seek: position in bytes. If negative, count from the end of file: -1 means
	   at end. If operation not available, must return BSt_IMPROPER; */
	BaliStatus	API_ (* seek) Args_ ((void *assoc, BaliInt position));

	/* flush: called whenever written data must be actually output.
	   Called after the data write.
	   Useful only in special cases, generally needs not be implemented */
	BaliStatus	API_ (* flush) Args_ ((void *assoc));

	/* truncate at this position */
	BaliStatus	API_ (* truncate) Args_ ((void *assoc, BaliInt position));


} CustomStreamClass;

#endif
/* ~~~ DO NOT INSERT after this #endif ~~~ */
