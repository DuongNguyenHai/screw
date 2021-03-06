// Nguyen Hai Duong
// Oct 29 2016

#include "beU-log.h"

namespace BeU {

bool PRINT_MONITOR = true;      // define for print on terminal in Log function.
bool PRINT_FILE = false;      // define for print to file in Log function.
bool PRINT_FILE_MONTH = true;
int32_t DEBUG_LEVEL = 3;      // define for debug in database function.
std::string LOG_FILE ("opert");

char *DateAndTime(time_t rawtime);
char *YearMonth(time_t rawtime);

const char *g_program_name = NULL;
time_t rawtime;
// If the program name was set (g_program_name != ""), the function
// GetProgramName returns the program name (without the path) followed by a
// colon, e.g. "gmm-align:".  Otherwise it returns the empty string "".
const char *GetProgramName() {
  return g_program_name == NULL ? "" : g_program_name;
}
// Given a filename like "/a/b/c/d/e/f.cc",  GetShortFileName
// returns "e/f.cc".  Does not currently work if backslash is
// the filename separator.
const char *GetShortFileName(const char *filename) {
	const char *last_slash = strrchr(filename, '/');
	if (!last_slash) {
		return filename;
	} else {
		while (last_slash > filename && last_slash[-1] != '/')
			last_slash--;
		return last_slash;
	}
}
// Print log/error/warning/verbose on terminal an log file
static void SendToLog(	const LogMessageEnvelope &envelope,
                        const char *message) {

	// Otherwise, use Kaldi default logging.
	std::stringstream header;
	if (envelope.severity == LogMessageEnvelope::Info)
		header << "LOG(";
	else if (envelope.severity == LogMessageEnvelope::Warning)
		header << "WARNING(";
	else if (envelope.severity == LogMessageEnvelope::Error)
		header << "ERROR(";
	else
		header << "VERBOSE(";

	time (&rawtime);
	char *dateTime = DateAndTime(rawtime);
	header << GetProgramName() << envelope.func << "():"
	     << envelope.file << ':' << envelope.line << ")"
	     << "["<< dateTime << "]";

	free(dateTime);
	std::string header_str = header.str();

	if (PRINT_MONITOR)
		printf("%s %s\n", header_str.c_str(), message);

	if (PRINT_FILE) {
		std::string fileLog = LOG_FILE;
		if(PRINT_FILE_MONTH) {
      char *mon = YearMonth(rawtime);
			fileLog += mon;
      free(mon);
		}
		fileLog += ".log";
		std::ofstream logFile(fileLog.c_str(), std::ios::out | std::ios::app);
		if (logFile.is_open()) {
			logFile << header_str.c_str() << " " << message << std::endl;
			logFile.close();
		} else {
			printf("%s Can not log to \"%s\" file. The folder is not founded\n", header_str.c_str(), fileLog.c_str());
		}
	}
}

MessageLogger::MessageLogger(LogMessageEnvelope::Severity severity,
                             const char *func, const char *file, int32_t line) {
	// Obviously, we assume the strings survive the destruction of this object.
	envelope_.severity = severity;
	envelope_.func = func;
	envelope_.file = GetShortFileName(file);  // Pointer inside 'file'.
	envelope_.line = line;
	envelope_.logLevel = DEBUG_LEVEL;
}

// There are 3 level of debug level  from 1->3
// Higher level will debug more information
// Set  DEBUG_LEVEL=0 means : show nothing.
// 		DEBUG_LEVEL=1 means : show WARNING, ERROR but no LOG.
// 		DEBUG_LEVEL=2 means : show WARNING, ERROR and LOG.
// 		DEBUG_LEVEL=3 means : show WARNING, ERROR and LOG and VERBOSE.

MessageLogger::~MessageLogger() {

	std::string str = ss_.str();
	while (!str.empty() && str[str.length() - 1] == '\n')
		str.resize(str.length() - 1);

	// Print error and exit if program got error message
	if(envelope_.severity == LogMessageEnvelope::Error) {
		SendToLog(envelope_, str.c_str());
		kill(0, SIGTERM);	// Terminate system (all processes)
	}

	// Log corresponds to DEBUG_LEVEL
	switch(envelope_.logLevel) {
		case 1: {
			if( envelope_.severity != LogMessageEnvelope::Info &&
				envelope_.severity != LogMessageEnvelope::Verbose )
				SendToLog(envelope_, str.c_str());
		} break;
		case 2: {
			if(envelope_.severity != LogMessageEnvelope::Verbose)
				SendToLog(envelope_, str.c_str());
		} break;
		case 3: {
			SendToLog(envelope_, str.c_str());
		} break;
		default:break;
	}

}

char *DateAndTime(time_t rawtime) {
	struct tm infor;
	localtime_r(&rawtime, &infor);
	char buffer[32];
	strftime(buffer, sizeof(buffer),"%X-%x", &infor);

	return strdup(buffer);
}

char *YearMonth(time_t rawtime) {
	struct tm infor;
	localtime_r(&rawtime, &infor);
	char buffer[32];
	strftime(buffer, sizeof(buffer),"%Y_%m", &infor);

	return strdup(buffer);
}

} // end of namspace TREE
