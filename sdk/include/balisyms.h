// ##notice Xavier FRANC
/* Miscellaneous */

#define	SpecialGI_CDATA		"#CDATA"
#define	SpecialGI_TEXT		"#TEXT"		/* now deprecated */
#define	SpecialGI_SDATA		"#SDATA"
#define	SpecialGI_XDATA		"#XDATA"
#define	SpecialGI_PI		"#PI"
#define	SpecialGI_CONTENT	"#CONTENT"
#define	SpecialGI_DEFAULT	"#D"		/* internal use only */

/* Names of Global Options */

#define	BGOPT_libPath	    "libPath"	/* add a library path */
#define BGOPT_userArg	    "userArg"	/* add a user argument */
#define BGOPT_stdin	    "stdin"	/* set std input on this file name */
#define BGOPT_stdout	    "stdout"	/* set std output on this file name */
#define BGOPT_stderr	    "stderr"	/* set std error on this file name */
#define BGOPT_demoMode	    "demoMode"	/* enter demo mode */
#define BGOPT_stackSize	    "stackSize"	/* set Balise stack size */
#define BGOPT_APIverbose    "APIverbose" /* if set, API displays error messages */
#define BGOPT_dumbUI	    "dumbUI"	 /* no line edition in debugger */
#define BGOPT_windows	    "windows"	 /* reserved for future use */
#define BGOPT_defaultParser "defParser"  /* default parser for parseDocument */
#define BGOPT_plugModule    "plugModule" /* name of extension module to plug-in */
#define BGOPT_lookAhead	    "lookAhead"  /* enable event look-ahead on next processes */
#define BGOPT_DOSBug	    "DOSBug"	 /* to reload buggy old DOS compiled files */
#define BGOPT_demoMangled   "mangle"	 /* to switch to mangle demo mode */
#define BGOPT_demoLimit	    "limit"	 /* to switch to limit demo mode */
#define BGOPT_customerID    "customerID" /* for generation of compiled files */
#define BGOPT_errorLimit    "errorLimit" /* initial value */
#define BGOPT_regexpBuffer  "regexpBuffer" /* == max match length */

	/* options for trace and debugging */
#define BGOPT_verbose	    "verbose"	 /* trace verbosity */
#define BGOPT_traceEvent    "traceEvent" /* set a trace on SGML event */
#define BGOPT_traceFun	    "traceFun"	 /* set a function trace */
#define BGOPT_traceFile	    "traceFile"	 /* load a trace specification file */
#define BGOPT_breakPoint    "breakPoint" /* set a break-point */

#define BGOPT_callDepth	    "callDepth"	 /* displayed stack depth */
#define BGOPT_sgmlDepth	    "sgmlDepth"	 /* displayed SGML stack depth */
#define BGOPT_contLength    "contLength" /* displayed length of SGML char data */
#define BGOPT_stringLength  "stringLength" /* displayed length of strings */
#define BGOPT_debug	    "debug"	 /* enter debug mode */
#define BGOPT_dbSource	    "dbSource"	 /* initial debugger command file */
#define BGOPT_debugSave	    "debugSave"	 /* # of commands in debugger history */
#define BGOPT_profiler	    "profiler"	 /* profiler operation */

#define BGOPT_PPset	    "PPset"	/* set preprocessor command */
#define BGOPT_PPdefault	    "PPdefault"	/* reset preprocessor cmd to default */
#define BGOPT_PPreset	    "PPreset"	/* nullify PP command */
#define BGOPT_PPappend	    "PPappend"	/* add value to PP command */

#define BGOPT_defaultCoding	"defaultCoding"
#define BGOPT_sourceCoding	"sourceCoding"
#define BGOPT_stdinCoding	"stdinCoding"
#define BGOPT_stdoutCoding	"stdoutCoding"
#define BGOPT_stderrCoding	"stderrCoding"

#define BGOPT_maxDDoc		"maxDDoc"

/* Options of parseDocument */

#define EVGOPT_generator	"generator"
#define EVGOPT_silent		"silent"
#define EVGOPT_dtdOnly		"dtd"


/* Options of dumpSubTree */

#define BDUMP_format		"FORMAT"
#define BDUMP_indent		"INDENT"
#define BDUMP_attr		"ATTR"
#define BDUMP_tagcr		"TAGCR"
#define BDUMP_uppercase		"UPPERCASE"

#define BDUMP_sgml		"sgml"
#define BDUMP_node		"node"
#define BDUMP_line		"line"
#define BDUMP_debug		"debug"


/* Status codes */

#define	BSt_OK	0
#define	BSt_IMPROPER	-1
#define	BSt_ACCESS	-2
#define	BSt_FAIL	-3
#define	BSt_UNKNOWN	-4
#define	BSt_DUPLICATE	-5
#define	BSt_MEMORY	-6
#define	BSt_TRUNCATED	-7
#define	BSt_BadFormat	-8
#define	BSt_BadVersion	-9
     /*	node errors: */
#define	BSt_BadNode	-10
#define	BSt_BadDoc	-11
#define	BSt_ReadOnlyDoc	-12
#define	BSt_SelfPrune	-14

#define	BSt_BadSysName	-15
#define	BSt_CodingError	-16
#define BSt_Security	-17

#define BSt_TooLarge	-18
#define BSt_BadArg	-19
#define BSt_Integrity   -20
	/* new loading errors */
#define BSt_BadCID   -21

