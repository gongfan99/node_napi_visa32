#include <memory>
#include <string>
#include "visa.h"

typedef struct {
  ViSession vi;
  ViStatus status;
  ViChar ErrorMsg[256];
} arg_viOpenDefaultRM;

typedef struct {
  ViSession vi;
  ViStatus status;
  ViChar ErrorMsg[256];
} arg_viClose;

typedef struct {
  ViSession sesn;
  char name[128];
  ViAccessMode mode;
  ViUInt32 timeout;
  ViSession vi;
  ViStatus status;
  ViChar ErrorMsg[256];
} arg_viOpen;

typedef struct {
  char writeFmt[128];
  ViSession vi;
  ViStatus status;
  ViChar ErrorMsg[256];
} arg_viPrintf;

typedef struct {
  char readFmt[128];
  std::string output;
  ViSession vi;
  ViStatus status;
  ViChar ErrorMsg[256];
} arg_viScanf;