#ifndef SDK_H
#define SDK_H

extern "C" {
#define ANSIPROTO
#define BAL_UNICODE
#include "balconfig.h"
#include "balisapi.h"
#include "parseapi.h"

#ifdef WIN32
extern EventSenderDesc __declspec(dllimport) BaliExt_SgmlEventGenerator;
extern EventSenderDesc __declspec(dllimport) BaliExt_SpEventGenerator;
extern EventSenderDesc __declspec(dllimport) BaliExt_SpXmlEventGenerator;
#else
extern EventSenderDesc BaliExt_SgmlEventGenerator;
extern EventSenderDesc BaliExt_SpEventGenerator;
extern EventSenderDesc BaliExt_SpXmlEventGenerator;
#endif
}

#endif
