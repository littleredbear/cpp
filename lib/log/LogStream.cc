#include "log/LogStream.h"


#ifdef LRBLOG

lrb::log::LogLevel g_logLevel = lrb::log::ALL;

lrb::log::LogStream g_logStream("lrbLog");

#else

lrb::log::LogLevel g_logLevel = lrb::log::NONE;


#endif




