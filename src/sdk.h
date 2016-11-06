#ifndef SDK_H
#define SDK_H

extern "C" {
#define ANSIPROTO
#define BAL_UNICODE
#include "balconfig.h"
#include "balisapi.h"
#include "parseapi.h"

#ifdef WIN32
extern EventSenderDesc __declspec(dllimport) XML_EventGenerator;
extern EventSenderDesc __declspec(dllimport) RTF_EventGenerator;
extern EventSenderDesc __declspec(dllimport) BaliExt_SpXmlEventGenerator;
extern EventSenderDesc __declspec(dllimport) BaliExt_SgmlEventGenerator;
extern EventSenderDesc __declspec(dllimport) BaliExt_SpEventGenerator;
#else
extern EventSenderDesc XML_EventGenerator;
extern EventSenderDesc RTF_EventGenerator;
extern EventSenderDesc BaliExt_SpXmlEventGenerator;
extern EventSenderDesc BaliExt_SgmlEventGenerator;
extern EventSenderDesc BaliExt_SpEventGenerator;
#endif
}

#endif
