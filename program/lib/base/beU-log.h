// Nguyen Hai Duong
// Oct 29 2016

#ifndef BEU_LOG_H
#define BEU_LOG_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <inttypes.h>	// int64_t
#include <stdlib.h>		// exit()
#include <errno.h>
#include <signal.h> // signal


namespace BeU {

extern bool PRINT_MONITOR;  // print infor on terminal
extern bool PRINT_FILE;     // print infor to file (log file)
extern bool PRINT_FILE_MONTH;

// There are 3 level of debug level  from 1->3
// Higher level will debug more information
// Set  DEBUG_LEVEL=0 means : show nothing.
//    DEBUG_LEVEL=1 means : show WARNING, ERROR but no LOG.
//    DEBUG_LEVEL=2 means : show WARNING, ERROR and LOG.
//    DEBUG_LEVEL=3 means : show WARNING, ERROR and LOG and VERBOSE.
extern int32_t DEBUG_LEVEL;
extern std::string LOG_FILE;

// #define LOGFILE "../log/root.log"  // define a log-file for system. its may be redefined to log in specific log-file in each prog.
// #define ABSLOGFILE "../log/root.log"

extern const char *g_program_name;

struct LogMessageEnvelope {
  enum Severity {
    Error = -2,
    Warning = -1,
    Info = 0,
    Verbose = 1
  };
  // An 'enum Severity' value, or a positive number indicating verbosity level.
  int severity;
  const char *func;
  const char *file;
  int logLevel;
  int32_t line;
};

class MessageLogger {

public:

	MessageLogger(LogMessageEnvelope::Severity severity, const char *func,
	              const char *file, int32_t line);
	~MessageLogger();
	inline std::ostream &stream() { return ss_; }

private:
	LogMessageEnvelope envelope_;
	std::ostringstream ss_;

};

#define LOG_ERR \
	MessageLogger(LogMessageEnvelope::Error, \
                         __FUNCTION__, __FILE__, __LINE__).stream()
#define LOG_WARN \
	MessageLogger(LogMessageEnvelope::Warning, \
                         __FUNCTION__, __FILE__, __LINE__).stream()
#define LOG \
	MessageLogger(LogMessageEnvelope::Info, \
                         __FUNCTION__, __FILE__, __LINE__).stream()
#define LOG_VERB \
	MessageLogger(LogMessageEnvelope::Verbose, \
                         __FUNCTION__, __FILE__, __LINE__).stream()

} // end of namespace TREE

#endif
