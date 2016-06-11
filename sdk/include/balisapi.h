#ifndef _Included_balisapi_
#define _Included_balisapi_

/*
// ##notice Xavier FRANC
//--------------------------------------------------------------------------------
//
//	Balise
//
//
//	API version 3
//
//
//--------------------------------------------------------------------------------
*/

#ifndef	_Included_balibase_
#include "balibase.h"
#endif

/* -------- User data ----------------------------------------------- */

#ifdef MSVC
BaliEXT BaliVoid	 EXPORT_	*Bali_getUserData Args_((BaliProcess process));
#else
BaliEXT BaliVoid	* EXPORT_	 Bali_getUserData Args_((BaliProcess process));
#endif

BaliEXT	BaliVoid	EXPORT_ Bali_setUserData Args_((BaliProcess, BaliVoid *data));


/* -------- Balise Master API --------------------------------------- */

/*	Called before any other action to setup Balise state.
	Defines the Balise root directory, where data files are found.
 */
BaliEXT	BaliVoid	EXPORT_ Bali_initSession  Args_((BaliString homeDir));

typedef BaliVoid (*BuiltinRegisterer) (void);

BaliEXT	BaliVoid	EXPORT_ Bali_addBuiltin  Args_((BuiltinRegisterer registerer));

/*	Returns the path of a home directory :
	Use the program name to try to find an environment variable that contains
	the path of the Balise home directory (By default "B2_HOME").
	For example, if the executable is 'dtbalise', try to getenv("DTBALISE_HOME").
 */
BaliEXT	BaliString	EXPORT_ Bali_stdHome  Args_((BaliString programName));

/*	Parsing command-line options: uses an option description table.
	returns unrecognized options in argv, modifying argc.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_arguments Args_((BaliInt *argc,
						      BaliString argv[],
						      BaliString usage,
						      BaliOptionEntry options[]));

/*	Sends a usage message by printing out the option table.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_usage Args_((BaliString programName,
						  BaliString message,
						  BaliOptionEntry options[]));

/*	Cleanup Balise stuff
 */
BaliEXT	BaliVoid	EXPORT_ Bali_cleanup();

/*	Terminate Balise : call exit() by default, unless Bali_exitHandler is used
	*** DEPRECATED: use Bali_cleanup and exit()
 */
BaliEXT	BaliVoid	EXPORT_ Bali_exit  Args_((BaliInt status));

/*	Define a function called instead of exit() when Balise terminates
	TODO: should be zapped, have Bali_cleanup()
 */
BaliEXT	BaliVoid	EXPORT_ Bali_exitHandler  Args_((void (*)(BaliInt status)));

/*	Print software version on stderr
 */
BaliEXT	BaliVoid	EXPORT_ Bali_showVersion  Args_((BaliString progName));

/*	Send a message : like in printf, '%s' sequences in 'format' are
	substituted by values of a1 and a2.
	type = 'I', 'W', 'E', 'F' for resp. Info, Warning, Error, Fatal
	Note: using 'F' (fatal) implies termination (invokes Bali_exit)
 */
BaliEXT	BaliVoid	EXPORT_ Bali_userMessage  Args_((BaliInt type, BaliString format,
							 BaliString a1, BaliString a2));

/*	Set a global option
 */
BaliEXT	BaliStatus	EXPORT_ Bali_globalOption  Args_((BaliString name,
							  BaliString value));

/*	Try to load an auto-startup module, which name is the "basename" of programName
	First try a compiled one (with .bba extension), then if not existing or
	out-of-date, a source module (.bal extension).
	These modules are searched in 'lib' sub-directory of the directory
	defined by Bali_initSession.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_autoStart  Args_((BaliString programName,
						       BaliProcess *process));

/*	Loads an extension module, and auto-plug it. The name can be an absolute
	path or a symbolic name (module found in $BaliseHome/lib)
 */
BaliEXT BaliStatus      EXPORT_ Bali_loadExtension Args_((BaliString name));

/*	Register an external function.
	(can be called implicitely by Bali_loadExtension, if the plug-in protocol
	is followed.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_registerFunction Args_(
						(BaliString name, BaliInt argCount,
						 BaliBool dot, BaliExternFun code));

/*	Register an external function with a security mask.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_registerFunctionS Args_(
					(BaliString name, BaliInt argCount,
					 BaliBool dot, BaliExternFun code,
					 BaliInt securityMask));

/*	Pushes a security level by adding the inhibition mask to the current
	mask. Returns a unique authentification key to be passed to popSecurity.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_pushSecurity Args_(
						(BaliInt mask, BaliInt *authKey));

/*	Removes one or more security levels
	Returns BSt_Security if key is not valid.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_popSecurity Args_((BaliInt authKey));

/* -------- Process and documents ------------------------------------- */

/* Set execution flag for "main" clause */

BaliEXT	BaliVoid	EXPORT_ Bali_setExecuteMainClause Args_((BaliBool status));

/*	Parse a source file and return a new process
 */
BaliEXT	BaliStatus	EXPORT_ Bali_loadSrcProcess Args_(
						(BaliString fileId, BaliStream diag,
						 BaliProcess *process));

/*	Parse from a source stream and return a new process.
	No preprocessing applied.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_loadStreamSrc Args_(
						(BaliObject input, BaliStream diag,
						 BaliProcess *process));

/*	Load a compiled module and return a new process.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_loadBinProcess Args_(
						(BaliString binFile, BaliStream diag,
						 BaliProcess *process));

/*	Save a process into a compiled module.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_saveBinProcess Args_(
						(BaliProcess, BaliString fileName));

/*	Parse a source file (containing only global variable and function
	definitions), and if successful add these definitions to a process.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_loadSrcLibrary
			Args_((BaliProcess process, BaliString sysId, BaliStream diag));

/*	Parse from source stream or string (containing only global variable and
	function definitions), and if successful add these definitions to a process.
	No preprocessing applied.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_loadStreamSrcLib
			Args_((BaliProcess process, BaliObject input, BaliStream diag));

/*	Parse a compiled module (containing only global variable and function
	definitions), and add these definitions to a process, if successful.
 */
BaliEXT	BaliStatus	EXPORT_ Bali_loadBinLibrary Args_((BaliProcess process,
						     BaliString file, BaliStream diag));

/*	Runs the 'main' and/or 'before' clause of a process
 */
BaliEXT	BaliStatus	EXPORT_ Bali_runProlog	  Args_((BaliProcess process));

/*	return a constant string for diagnostic */
BaliEXT	BaliString	EXPORT_ Bali_loadError Args_((BaliStatus code));

/*	Parse a SGML document made of one or several entities.
	Returns the parsed document (whole tree if null process given).
 */
BaliEXT	BaliStatus	EXPORT_ Bali_parseDocument Args_(
					(BaliString parserName, BaliProcess process,
					 BaliString entIds[], BaliInt entIdCount,
					 BaliObject optionMap, BaliDocument *doc));

/* -------- Balise services ------------------------------------------- */

/* --- Predefined objects --- */

BaliEXT BaliObject      EXPORT_ Bali_Nothing  Args_ (());
BaliEXT BaliFileStream  EXPORT_ Bali_Cout  Args_ (());
BaliEXT BaliFileStream  EXPORT_ Bali_Cerr  Args_ (());
BaliEXT BaliFileStream  EXPORT_ Bali_Cin   Args_ (());
BaliEXT BaliProcess     EXPORT_ Bali_RootProcess  Args_ (());

/* --- Memory management ---- */

/*	called at each new reference to an object			*/
BaliEXT	BaliObject	EXPORT_ Bali_use Args_((BaliObject obj));

/*	suppress a object reference. WARNING: can destroy the object	*/
BaliEXT	BaliVoid	EXPORT_ Bali_unuse Args_((BaliObject obj));

/*	facility: use(new value), unuse(old value), store new in loc	*/
BaliEXT	BaliVoid	EXPORT_ Bali_assign Args_((BaliObject *loc, BaliObject newVal));

/*	Return the type of an object
 */
BaliEXT	BaliType	EXPORT_ Bali_objectType	Args_((BaliObject obj));

/*	Type testing
 */
BaliEXT	BaliBool	EXPORT_ Bali_isaBoolean  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaNumber  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaString  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaMap  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaSet  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaList  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaFunction  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaStringStream  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaFileStream  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaLinkedString  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaUserType  Args_((BaliObject, BaliString name));
BaliEXT	BaliBool	EXPORT_ Bali_isaRegExp  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaNode  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaDocument  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaProcess  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaMatch  Args_((BaliObject));
BaliEXT	BaliBool	EXPORT_ Bali_isaCodingSystem  Args_((BaliObject));

/*	Returns the length of an object, like the 'length' function
 */
BaliEXT	BaliInt		EXPORT_ Bali_length Args_((BaliObject));

/*	Returns an object of type Number, with the given numeric value
 */
BaliEXT	BaliStatus	EXPORT_ Bali_makeNumber Args_((BaliDouble value,
						       BaliObject *result));
/*	Returns the numeric value of an object of type Number
 */
BaliEXT	BaliStatus	EXPORT_ Bali_getNumber Args_((BaliObject object,
						      BaliDouble *value));

/*	Create a String object from a null-terminated string */
BaliEXT	BaliStatus	EXPORT_ Bali_makeString Args_((BaliString data,
						       BaliObject *result));
/*	Create a String object from a string of given length */
BaliEXT	BaliStatus	EXPORT_ Bali_makeLString Args_((BaliXString data, BaliInt length,
							BaliObject *result));
/*	Returns a null-terminated string which is the value of a String object
	CAUTION: this string must not be altered nor freed
 */
BaliEXT	BaliStatus	EXPORT_ Bali_getString Args_((BaliObject data, BaliXString *));

/*	Returns the 'true' or 'false' objects, according to the boolean value
 */
BaliEXT	BaliStatus	EXPORT_ Bali_makeBoolean Args_((BaliBool, BaliObject *result));

/*	Returns a C-like boolean from a Boolean object
*/
BaliEXT	BaliBool	EXPORT_ Bali_getBoolean Args_((BaliObject));


/*	BkThread management (for PL/Balise only)
*/
BaliEXT	BaliStatus	EXPORT_ Bali_makeFirstBkThreadCurrent Args_(());
BaliEXT	BaliStatus	EXPORT_ Bali_lockSdk Args_(());
BaliEXT	BaliStatus	EXPORT_ Bali_unlockSdk Args_(());


/*	StringStream manipulation
*/
BaliEXT	BaliObject	EXPORT_ Bali_makeStringStream Args_(());

BaliEXT	BaliStatus	EXPORT_ Bali_streamReadAll Args_((BaliObject stream,
							BaliObject *value));


/*	List manipulations :
 */
BaliEXT	BaliObject	EXPORT_ Bali_makeList Args_(());

BaliEXT	BaliStatus	EXPORT_ Bali_listGet Args_((BaliObject list, BaliInt pos,
						    BaliObject *value));

BaliEXT	BaliStatus	EXPORT_ Bali_listPut Args_((BaliObject list, BaliInt pos,
						    BaliObject value));

BaliEXT	BaliStatus	EXPORT_ Bali_listInsert Args_((BaliObject list, BaliInt pos,
						       BaliObject value));

BaliEXT	BaliStatus	EXPORT_ Bali_listRemove Args_((BaliObject list, BaliInt pos,
						       BaliInt count));

/*	Map manipulations :
 */
BaliEXT	BaliObject	EXPORT_ Bali_makeMap Args_(());

BaliEXT	BaliBool	EXPORT_ Bali_mapTest Args_((BaliObject map, BaliObject key));

BaliEXT	BaliStatus	EXPORT_ Bali_mapGet Args_((BaliObject map, BaliObject key,
						   BaliObject *value));

BaliEXT	BaliStatus	EXPORT_ Bali_mapPut Args_((BaliObject map, BaliObject key,
						   BaliObject value));

BaliEXT	BaliStatus	EXPORT_ Bali_mapRemove Args_((BaliObject set, BaliObject key));

/*	Set manipulations :
 */
BaliEXT	BaliObject	EXPORT_ Bali_makeSet Args_(());

BaliEXT	BaliBool	EXPORT_ Bali_setTest Args_((BaliObject set, BaliObject value));

BaliEXT	BaliStatus	EXPORT_ Bali_setPut Args_((BaliObject set, BaliObject value));

BaliEXT	BaliStatus	EXPORT_ Bali_setRemove Args_((BaliObject set, BaliObject value));

/*	User-defined types manipulations:
 */
BaliEXT	BaliStatus	EXPORT_ Bali_makeUType Args_(
				       (BaliProcess, BaliString name, BaliObject *res));

BaliEXT	BaliStatus	EXPORT_ Bali_utypeGet Args_((BaliObject utype, BaliInt pos,
						     BaliObject *value));

BaliEXT	BaliStatus	EXPORT_ Bali_utypePut Args_((BaliObject utype, BaliInt pos,
						     BaliObject value));

/*	Iterators on collections (List, Map, or Set)
 */
BaliEXT	BaliStatus	EXPORT_ Bali_makeIter Args_((BaliObject collection,
						     BaliObject *iter));

BaliEXT	BaliStatus	EXPORT_ Bali_iterNext Args_((BaliObject iter,
						     BaliObject *item,
						     BaliObject *assocItem));

BaliEXT	BaliStatus	EXPORT_ Bali_iterReset Args_((BaliObject iter));

/* --- Documents and nodes --- */

BaliEXT	BaliBool	EXPORT_ Bali_isaElemNode  Args_((BaliObject node));
BaliEXT	BaliBool	EXPORT_ Bali_isaTextNode  Args_((BaliObject node));
BaliEXT	BaliBool	EXPORT_ Bali_isaEntityNode  Args_((BaliObject node));
BaliEXT	BaliBool	EXPORT_ Bali_isaPINode  Args_((BaliObject node));

BaliEXT	BaliBool	EXPORT_ Bali_empty  Args_((BaliObject node));

BaliEXT	BaliStatus	EXPORT_ Bali_gi  Args_((BaliObject node, BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_document  Args_((BaliObject node, BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_parent  Args_((BaliObject node, BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_content  Args_((BaliObject node, BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_makeAttrIter  Args_((BaliObject node,
							  BaliBool sgml, BaliBool appl,
							  BaliObject *iter));


BaliEXT	BaliStatus	EXPORT_ Bali_arity Args_((BaliObject node, BaliInt *));

BaliEXT	BaliStatus	EXPORT_ Bali_rank Args_((BaliObject node, BaliInt *));

BaliEXT	BaliStatus	EXPORT_ Bali_echo Args_((BaliObject node, BaliBool start,
						BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_root Args_((BaliObject doc, BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_currentNode Args_(
						 (BaliProcess process, BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_child Args_((BaliObject node,
						  BaliXString gi, BaliInt pos,
						  BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_enclosing Args_((BaliObject node,
						      BaliXString gi, BaliInt levels,
						      BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_rightSibling Args_((BaliObject node,
							 BaliXString gi, BaliInt levels,
							 BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_leftSibling Args_ ((BaliObject node,
							 BaliXString gi, BaliInt levels,
							 BaliObject *));

BaliEXT	BaliStatus	EXPORT_ Bali_children Args_((BaliObject node,
					   BaliXString gi, BaliXString an, BaliObject av,
						     BaliObject *));

/* --- Character encoding and decoding --- */

/* build a  transcoder usable with Bali_doEncoding and Bali_doDecoding
 */
BaliEXT	BaliStatus  EXPORT_ Bali_transcoder Args_((BaliProcess process,
					     BaliString codingSystem, BaliString options,
					     BaliObject *transcoder));

/*	encode N characters into bytes : return the count of bytes
	returns the # of transcoding errors into *errCount (if this ptr isn't null)
	The output buffer is assumed to be wide enough to receive encoded bytes
 */
BaliEXT	BaliInt	 EXPORT_ Bali_doEncoding Args_((BaliObject transcoder,
						BaliXString chars, BaliInt N,
						char *bytes,
						BaliInt *errCount));

/*	decode N bytes into characters : return the count of decoded characters
	returns the # of transcoding errors into *errCount (if this ptr isn't null)
	The output buffer is assumed to be wide enough to receive decoded chars
 */
BaliEXT	BaliInt	 EXPORT_ Bali_doDecoding Args_((BaliObject transcoder,
						BaliString bytes, BaliInt N,
						BaliXString chars,
						BaliInt *errCount));

/* --- Character encoding and decoding OLD FASHION --- */

/*	sets the Coding System and the options used in the decode function */
BaliEXT	BaliStatus	EXPORT_ Bali_setDecoder Args_((BaliXString name,
						       BaliXString options));

/*	decode N bytes into characters : return the count of decoded characters
	returns the # of transcoding errors into *errCount (if this ptr isn't null)
	The output buffer is assumed to be wide enough to receive decoded chars
 */
BaliEXT	BaliInt		EXPORT_ Bali_decodeBytes Args_((BaliString bytes, BaliInt N,
							BaliXString chars,
							BaliInt *errCount ));

/*	sets the Coding System and the options used in the encode function */
BaliEXT	BaliStatus	EXPORT_ Bali_setEncoder Args_((BaliXString name,
						       BaliXString options));

/*	encode N characters into bytes : return the count of bytes
	returns the # of transcoding errors into *errCount (if this ptr isn't null)
	The output buffer is assumed to be wide enough to receive encoded bytes
 */
BaliEXT	BaliInt		EXPORT_ Bali_encode Args_((BaliXString chars, BaliInt N,
						   char *bytes, BaliInt *errCount ));


/* --- Function invocation --- */

/*	Gets the descriptor of a function defined in the given process */
BaliEXT	BaliStatus	EXPORT_ Bali_getFunction Args_((BaliProcess, BaliString name,
							BaliObject *result));

/*	Executes a function: 'fun' is a descriptor returned by Bali_getFunction
	NOTE: the 'called' process argument is obsolete from v3.0.57
 */
BaliEXT	BaliStatus	EXPORT_ Bali_perform Args_((BaliProcess called,
						    BaliObject fun,
						    BaliInt argCount,
						    BaliObject args[],
						    BaliObject *result,
						    BaliProcess caller));

BaliEXT	BaliStatus	EXPORT_ Bali_perform0 Args_((BaliProcess, BaliObject fun,
						     BaliObject *, BaliProcess caller));

BaliEXT	BaliStatus	EXPORT_ Bali_perform1 Args_((BaliProcess, BaliObject fun,
						     BaliObject,
						     BaliObject *, BaliProcess caller));

BaliEXT	BaliStatus	EXPORT_ Bali_perform2 Args_((BaliProcess, BaliObject fun,
						     BaliObject,
						     BaliObject,
						     BaliObject *, BaliProcess caller));

BaliEXT	BaliStatus	EXPORT_ Bali_perform3 Args_((BaliProcess, BaliObject fun,
						     BaliObject,
						     BaliObject,
						     BaliObject,
						     BaliObject *, BaliProcess caller));

BaliEXT	BaliStatus	EXPORT_ Bali_perform4 Args_((BaliProcess, BaliObject fun,
						     BaliObject,
						     BaliObject,
						     BaliObject,
						     BaliObject,
						     BaliObject *, BaliProcess caller));

/*	Evaluates an expression in source form :
 */
BaliEXT	BaliStatus	EXPORT_ Bali_evalExpression Args_((BaliProcess process,
							   BaliXString expr,
							   BaliStream diag,
							   BaliObject *result));

/* --- Global variables --- */

BaliEXT	BaliStatus	EXPORT_ Bali_getGlobal Args_((BaliProcess, BaliString name,
						      BaliObject *value));

BaliEXT	BaliStatus	EXPORT_ Bali_setGlobal Args_((BaliProcess, BaliString name,
						      BaliObject value));

BaliEXT	BaliStatus	EXPORT_ Bali_testGlobal Args_((BaliProcess, BaliString name));

/* --- Raising errors --- */

BaliEXT	BaliObject	EXPORT_ Bali_raiseError Args_((BaliProcess,
						    BaliError kind, BaliString errArg));

BaliEXT	BaliObject	EXPORT_ Bali_typeError Args_((BaliProcess,
						      BaliInt argPos,
						      BaliInt expectedType));

/* --- Custom Streams --- */

BaliEXT	BaliStatus  EXPORT_ Bali_registerCustomStream Args_((CustomStreamClass *));

/*	build a new Custom Stream of a registered class
 */
BaliEXT	BaliStatus  EXPORT_ Bali_CustomStream Args_((BaliString streamClass,
						     void *assocData, char mode,
						     BaliString codingSystem,
						     BaliObject *newStream));

/* -------- Event Handling API ------------------------------------------- */

/*	Install process as Balise context
 */
BaliEXT	BaliStatus	EXPORT_ Bali_installContext Args_((BaliProcess,
							   BaliBool prune));

/*	Deinstall Balise context; get a full CoreDocument if prune was true
 */
BaliEXT	BaliStatus	EXPORT_ Bali_removeContext Args_((BaliProcess,
							  BaliDocument *result));

/* -------- Services, Threads and Messages ------------------------------- */

/*:
	Creates a new Balise Service loading the specified script
	which can be a source program or a compiled program.
	The arguments are put to the global variable Arguments in the root process.
	The resulting service is unlocked.
 */
BaliEXT	BaliStatus  EXPORT_ Bali_createService Args_((BaliString program,
						      BaliInt argc, BaliXString argv[],
						      Int *threadId));

/*:
	Deletes a local Service that must be unattached.
 */
BaliEXT	BaliStatus  EXPORT_ Bali_deleteService Args_((Int threadId));

/*:
	Creates a new Balise Thread that starts up with the specified program
	(first calls Bali_createService)
	which can be a source program or a compiled program.
	The arguments will be given to the starting process in its Arguments global.
 */
BaliEXT	BaliStatus  EXPORT_ Bali_createThread Args_((BaliString program,
						     BaliInt argc, BaliXString argv[],
						     Int *threadId));

/*:
	bind to message service, local or remote.
	returns an object of type Service.
 */
BaliEXT	BaliStatus  EXPORT_ Bali_msgBind Args_((BaliString id, BaliObject *service));

/*:
	Send a message to a local or remote Service (object returned by Bali_msgBind).
	If the pointer 'reply' is not null, the function will wait for a reply and
	return the value of the reply message in 'reply'. The 'timeOut' specifies how
	long at most to wait for the reply. If negative, wait as long as necessary.
 */
BaliEXT	BaliStatus EXPORT_ Bali_msgSend Args_((BaliObject service, BaliString request,
					       BaliObject arg, BaliInt priority,
					       BaliObject *reply, BaliInt timeOut));


/*:
	Returns a String describing a Service, so that it can be transmitted
	to a third party and be used for connecting to this service.
	On return 'ref' contains a string that describes the service (this
	string must be freed by the caller).
	Interpretation of argument 'port': (useful only if the service is local)
	- if -1 : describe the service as local: ex "local:1"
	- if 0 : describe as BRM service with (unique) port used: ex "brm:<host>:8080"
	- if > 0 : describe as BRM service with port specified. "brm:<host>:9000"
 */
BaliEXT	BaliStatus EXPORT_ Bali_msgRef Args_((BaliObject service, Int port,
					      char **ref));

/*:
	Enables BRM connections.
	Automatic port allocation:
	if the the pointer 'allocatedPort' is not null and 'port' is null, then
	on return allocatedPort will point to the allocated value of the port.
 */
BaliEXT	BaliStatus EXPORT_ Bali_brmListen Args_((BaliString domain,
						 BaliInt port, BaliInt *allocatedPort));

/*:
	returns the service Id attached to current physical thread id (null if none)
 */
BaliEXT BaliStatus EXPORT_ Bali_currentService Args_(( BaliInt *servId ));

/*:
	returns the (local) service Id of a Service object. Fails if not local
 */
BaliEXT BaliStatus EXPORT_ Bali_serviceId Args_((BaliObject service, BaliInt *servId));

/*:
	returns a Service object from a local service Id.
 */
BaliEXT BaliStatus EXPORT_ Bali_serviceFromId Args_((BaliInt servId,
						     BaliObject *service));
/*:
	sets the name of a local Service
 */
BaliEXT BaliStatus EXPORT_ Bali_setServiceName Args_((BaliInt servId, BaliString name));

/*:
	returns the physical thread id (probably to be phased out)
 */
BaliEXT BaliStatus EXPORT_ Bali_getServiceSysId ( BaliInt servId, BaliInt *sysId );


/* -------- Macros for testing arguments ------------------------------------------- */

#define Bali_BadArg(r_)		return Bali_typeError(process, r_, BType_Void)
#define Bali_BadArgCount()	return Bali_typeError(process, -1, BType_Void)

#define	Bali_CheckBoolean(n, bool_var) \
   { if(!Bali_isaBoolean(args[n])) return Bali_typeError(process, n, BType_Boolean); \
	bool_var = Bali_getBoolean(args[n]); };

#define	Bali_CheckNumber(n, num_var) \
   { if(!Bali_isaNumber(args[n])) return Bali_typeError(process, n, BType_Number); \
	Bali_getNumber(args[n], &num_var); };

#define	Bali_CheckString(n, var) \
   { var = args[n]; \
     if(!Bali_isaString(var)) return Bali_typeError(process, n, BType_String); };

#define	Bali_StringArg(n, var) \
   { if(!Bali_isaString(args[n]) || Bali_getString(args[n], &var) != BSt_OK) \
		return Bali_typeError(process, n, BType_String); };

#define	Bali_CheckMap(n, var) \
   { var = args[n]; \
     if(!Bali_isaMap(var)) return Bali_typeError(process, n, BType_Map); };

#define	Bali_CheckSet(n, var) \
   { var = args[n]; \
     if(!Bali_isaSet(var)) return Bali_typeError(process, n, BType_Set); };

#define	Bali_CheckList(n, var) \
   { var = args[n]; \
     if(!Bali_isaList(var)) return Bali_typeError(process, n, BType_List); };

#define	Bali_CheckRegExp(n, var) \
   { var = args[n]; \
     if(!Bali_isaRegExp(var)) return Bali_typeError(process, n, BType_RegExp); };

#define	Bali_CheckNode(n, var) \
   { var = args[n]; \
     if(!Bali_isaNode(var)) return Bali_typeError(process, n, BType_Node); };

#define	Bali_CheckDocument(n, var) \
   { var = args[n]; \
     if(!Bali_isaDocument(var)) return Bali_typeError(process, n, BType_Document); };

#define	Bali_CheckFunction(n, var) \
   { var = args[n]; \
     if(!Bali_isaFunction(var)) return Bali_typeError(process, n, BType_Function); };

#define	Bali_CheckLinkedString(n, var) \
   { var = args[n]; \
     if(!Bali_isaLinkedString(var)) return Bali_typeError(process, n, BType_LinkedString); };

#define	Bali_CheckStringStream(n, var) \
   { var = args[n]; \
     if(!Bali_isaStringStream(var)) return Bali_typeError(process, n, BType_StringStream); };

#define	Bali_CheckFileStream(n, var) \
   { var = args[n]; \
     if(!Bali_isaFileStream(var)) return Bali_typeError(process, n, BType_FileStream); };

#define	Bali_CheckProcess(n, var) \
   { var = args[n]; \
     if(!Bali_isaProcess(var)) return Bali_typeError(process, n, BType_Process); };

#define	Bali_CheckUserType(n, var) \
   { var = args[n]; \
     if(!Bali_isaUserType(var, nil)) return Bali_typeError(process, n, BType_UserType); };

#define	Bali_UserTypeArg(n, type, var) \
   { var = args[n]; \
     if(!Bali_isaUserType(var, type)) return Bali_typeError(process, n, BType_UserType); };

#define	Bali_CheckMatch(n, var) \
   { var = args[n]; \
     if(!Bali_isaMatch(var)) return Bali_typeError(process, n, BType_Match); };

#define Bali_CheckCodingSystem(n, var) \
   { var = args[n]; \
     if(!Bali_isaCodingSystem(var)) return Bali_typeError(process, n, BType_CodingSystem); };

#endif
/* ~~~ DO NOT INSERT after this #endif ~~~ */
