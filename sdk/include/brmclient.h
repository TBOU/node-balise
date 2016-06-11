#ifndef _Included_brmclient_h
#define _Included_brmclient_h

// ##notice Xavier FRANC
//================================================================================
//
//	Balise
//
//
//
//--------------------------------------------------------------------------------
//	
//	Module: C++ client interface to BRM
//	
//	

typedef	char		*BrmString;
#ifdef BAL_UNICODE
typedef	unsigned short	*BrmXString;
#else
typedef	unsigned char	*BrmXString;
#endif
typedef	double		 BrmNumber;
typedef	int		 BrmInt;
enum BrmSpecial {
	SpNothing, SpTrue, SpFalse
};

// status : null if OK, else a string describing the error
typedef	const char	*BrmStatus;
#define BrmOK ((BrmStatus) 0)

class	BrmMessage;	// below

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//	class BrmClient :
//

class	BrmClient
{
	friend class BrmClientImpl;

// .......... public methods ..................................
public:
	BrmClient();
	~BrmClient();

	// bind to Brm address: <host> [ ':' <port> ] [ '/' <thread> ]
	BrmStatus	bind (const char *address);

	// send a message
	BrmStatus	send (BrmMessage *);

	// send a signal of this name: no value and no reply
	BrmStatus	signal (BrmString );

	// wait for reply to one message
	// timeout in milliseconds: if negative wait indefinitely
	// if OK, on return the message has its value changed.
	BrmStatus	waitForReply (BrmMessage *, BrmInt timeout);

	// wait for reply to a list of messages
	// returns by reference the first replied message, with a changed value
	BrmStatus	waitForReply (BrmMessage *list[], BrmInt count, BrmInt timeout,
				      BrmMessage * &replied);

// .......... instance variables ..............................
protected:
class	BrmClientImpl	*impl;
}; // end of class BrmClient


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//	class BrmMessage :
//		a message has a name and a value: the value can be a single
//		basic value or an array of basic values.
//		The count of values is set at construction: by default the
//		message has a single value;
//
class	BrmMessage
{
	friend class BrmClient;
	friend class BrmClientImpl;

// .......... public methods ..................................
public:
	BrmMessage (BrmString name, int itemCount = -1, int priority = 1);
	~BrmMessage();

	// put values into the message before send: must have as many values
	// as specified by 'itemCount' in the constructor (1 by default)
	BrmStatus	putItem (BrmSpecial value, BrmInt rank = 0);
	BrmStatus	putItem (BrmInt value, BrmInt rank = 0);
	BrmStatus	putItem (BrmNumber value, BrmInt rank = 0);
	// strings are NOT copied to the message, they are managed by the
	// application and must remain valid until the send.
	BrmStatus	putItem (BrmString value, BrmInt length = -1, BrmInt rank = 0);
	BrmStatus	putItem (BrmXString value, BrmInt length = -1, BrmInt rank = 0);

	// get values of an existing message (normally after reply)

	BrmString	name();
	BrmInt		getItemCount();

	BrmStatus	getItem (BrmSpecial &value, BrmInt rank = 0);
	BrmStatus	getItem (BrmInt & valueBrm, BrmInt rank = 0);
	BrmStatus	getItem (BrmNumber & value, BrmInt rank = 0);
	// strings obtained from the reply are managed by the message: they
	// last as long as the message is not destroyed.
	BrmStatus	getItem (BrmString & value, BrmInt & length, BrmInt rank = 0);
	BrmStatus	getItem (BrmXString & value, BrmInt & length, BrmInt rank = 0);

	// dynamic typing:
	enum Type {
		TVoid, TBoolean, TInteger, TNumber, TString, TXString
	};
	BrmStatus	getItemType (Type &type, BrmInt rank = 0);

// .......... instance variables ..............................
protected:
class	BrmMessageImpl	*impl;
}; // end of class BrmMessage

#endif
// DO NOT INSERT after this #endif 
