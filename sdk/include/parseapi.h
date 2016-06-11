
/*
// ##notice Xavier FRANC
//--------------------------------------------------------------------------------
//
//	Balise
//
//
//	SGML Events API
//
//
//--------------------------------------------------------------------------------
*/

#ifndef _Included_balisapi_
#include "balisapi.h"
#endif

/* ----------------- Event Receiver Interface ---------------------------- */

typedef struct _receiverDesc
{
	/* identifier */
	BaliString	id;

	/* start of document */
	EventReturn	(*API_ prologue) Args_ ((EventReceiver*));

	/* start-tag */
	EventReturn	(*API_ startTag) Args_ ((EventReceiver*,
						    CONST BaliXString name));

	/* start attribute: implied is true if attribute not present */
	EventReturn	(*API_ startAttr) Args_ ((EventReceiver*,
					      CONST BaliXString name, BaliInt implied));

	/* Data token in Attribute */
	EventReturn	(*API_ attrToken) Args_ ((EventReceiver*,
					        CONST BaliXString data, BaliInt length));

	/* SDATA entity reference in Attribute */
	EventReturn	(*API_ sdataToken) Args_ ((EventReceiver*,
					        CONST BaliXString name, BaliInt length));

	/* end of start-tag: empty true if EMPTY content or CONREF attribute */
	EventReturn	(*API_ startElement) Args_ ((EventReceiver*, BaliInt empty));

	/* end-tag */
	EventReturn	(*API_ endTag) Args_ ((EventReceiver*,
						  CONST BaliXString name));

	/* text (character data) */
	EventReturn	(*API_ text) Args_ ((EventReceiver*,
						CONST BaliXString data, BaliInt length));

	/* general SDATA reference in content (declaration precedes) */
	EventReturn	(*API_ sdataRef) Args_ ((EventReceiver*,
						    CONST BaliXString name));

	/* external entity reference in content (declaration precedes) */
	EventReturn	(*API_ xdataRef) Args_ ((EventReceiver*,
						    CONST BaliXString name));

	/* processing-instruction */
	EventReturn	(*API_ pi) Args_ ((EventReceiver*,
					      CONST BaliXString data, BaliInt length));

	/* comment */
	EventReturn	(*API_ comment) Args_ ((EventReceiver*,
					      CONST BaliXString data, BaliInt length));

	/* end of document */
	EventReturn	(*API_ epilogue) Args_ ((EventReceiver*));

	/* SGML error : see below error category and severity codes */
	EventReturn	(*API_ sgmlError) Args_ ((EventReceiver*,
						     BaliInt categ, BaliInt severity,
							 CONST BaliXString message));

	/* SGML location */
	EventReturn	(*API_ location) Args_ ((EventReceiver*,
					     BaliInt lineNo, CONST BaliXString fileId));

	/* entity declaration : see types below */
	EventReturn	(*API_ entityDecl) Args_((EventReceiver*,
						     CONST BaliXString name,
						     BaliInt type, BaliInt isPublic,
						     CONST BaliXString id,
						     CONST BaliXString notation));
	/* notation declaration */
	EventReturn	(*API_ notationDecl) Args_ ((EventReceiver*,
						       CONST BaliXString name,
						       CONST BaliXString id));

	/* APPLINFO declaration */
	EventReturn	(*API_ applInfo) Args_ ((EventReceiver*,
					     CONST BaliXString data, BaliInt length));

	/* ---- extensions: ---- */

	/* start attribute: implied is true if attribute not present */
	EventReturn	(*API_ startApplAttr) Args_ ((EventReceiver*,
							 CONST BaliXString name));

	/* start of marked section : the data is the control key */
	EventReturn	(*API_ msStart) Args_ ((EventReceiver*,
						   int msType,
						   CONST BaliXString data,
						   BaliInt length));

	/* end of marked section */
	EventReturn	(*API_ msEnd) Args_ ((EventReceiver*,
						 int msType));

	/* contents of IGNORE marked section */
	EventReturn	(*API_ msIgnoredChars) Args_ ((EventReceiver*,
					  CONST BaliXString data, BaliInt length));

	/* start-bound of entity : the data is entity name */
	EventReturn	(*API_ entityStart) Args_ ((EventReceiver*,
						       int internalEntity,
						       CONST BaliXString data,
						       BaliInt length));

	/* end-bound of entity : the data is entity name */
	EventReturn	(*API_ entityEnd) Args_ ((EventReceiver*,
						     int internalEntity,
						     CONST BaliXString data,
						     BaliInt length));

	/* name-case flags (called just after prolog) */
	EventReturn	(*API_ nameCase) Args_ ((EventReceiver*,
						    int general, int entity));

}	EventReceiverDesc;

BaliEXT	BaliStatus	API_ Bali_setEventCallback Args_ ((BaliProcess process,
							      void (*)(BaliInt event)));

/* ----- entity types (other types are substituted by parsers) ------ */

#define	BaliEntityType_SDATA	1
#define	BaliEntityType_XSDATA	2
#define	BaliEntityType_XCDATA	3
#define	BaliEntityType_XNDATA	4
#define	BaliEntityType_XSUBDOC	5

/* ----- SGML error types ----- */

#define SGMLErr_syntax		1
#define SGMLErr_resource	2
#define SGMLErr_declaration	3
#define SGMLErr_capacity	4
#define SGMLErr_idref		5
#define SGMLErr_other		6

#define SGMLSeverity_warning	1
#define SGMLSeverity_error	2
#define SGMLSeverity_critical	3

/* ----- Marked Section types ----- */
#define MSType_IGNORE	0
#define MSType_INCLUDE	1
#define MSType_CDATA	2
#define MSType_RCDATA	3

/* --------------------------------------------------------------------------------
 *	Event Generator Control Interface
 */

/* opaque: managed internally by parser modules */
typedef	int	EventSender;	/* no more EventGenerator (conflict with SP) */

/* Description of an Event Sender */
typedef struct _evgendesc
{
	/* generic identifier (eg "sgmls", "sp") */
	BaliString	id;

	/* create an instance of the Event Generator: */
	EventSender	*(*API_ create) NoArgs_;

	/* delete an instance of the Event Generator: */
	void		(*API_ destroy) Args_((EventSender *));

	/* specify parser options */
	BaliInt		(*API_ setOption) Args_((EventSender *,
						    BaliString name, BaliString value));

	/* Specifies the Event Receiver handlers (this is not a OO interface): */
	BaliInt		(*API_ setReceiver) Args_((EventSender *,
						      EventReceiverDesc *));

	/* run on a series of document entities: returns the number of errors:
	   If negative, means that a critical error happened (invalid document)
	 */
	BaliInt		(*API_ run) Args_((EventSender *, EventReceiver *,
					      BaliString entIds[], BaliInt entIdCount));


	/* initiate a step-by-step parsing : returns before sending any event
	   same arguments as 'run'
	 */
	BaliInt		(*API_ startStepRun) Args_((EventSender *, EventReceiver *,
					      BaliString entIds[], BaliInt entIdCount));

	/* step-by-step parsing : generate up to 'eventCount' SGML events (to
	   the end if eventCount == 0). Must be called after 'startRun'
	   returns 0 when the parsing is finished
	 */
	BaliInt		(*API_ getEvents) Args_((EventSender *, BaliInt eventCount));

	/* terminate a step-by-step parsing : generate epilogue event
	   returns same status as 'run'
	 */
	BaliInt		(*API_ endStepRun) Args_((EventSender *));

	/* link to next (for registration) */
	struct _evgendesc *next;

}	EventSenderDesc;


/*	Register an Event Sender  */
BaliEXT	BaliStatus	EXPORT_ Bali_registerEventGenerator Args_(
							 (EventSenderDesc *gen));

/*	Find a registered Event Sender and get its handler vector  */
BaliEXT	BaliStatus EXPORT_ Bali_findEventGenerator Args_(
					(BaliString name, EventSenderDesc **gen));

/*	Get the vector of Balise event handlers		*/
//BaliEXT	 EventReceiverDesc * EXPORT_  Bali_eventHandlers NoArgs_ ;
BaliEXT	 EventReceiverDesc EXPORT_ * Bali_eventHandlers NoArgs_ ; // fcha
