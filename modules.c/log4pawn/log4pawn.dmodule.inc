/*
 * LogLevels:
 *
 * @more info at http://www.allapplabs.com/log4j/log4j_levels.htm
 *
 *   5 DEBUG - The DEBUG Level designates fine-grained informational events that are most useful to debug an application.
 *   4 INFO - The INFO level designates informational messages that highlight the progress of the application at coarse-grained level.
 *   3 WARN - The WARN level designates potentially harmful situations.
 *   2 ERROR - The ERROR level designates error events that might still allow the application to continue running.
 *   1 FATAL - The FATAL level designates very severe error events that will presumably lead the application to abort.
 *   0 - no log messages at all!
 */


/**
 * This function does nothing. And it's pretty good at it.
 */
stock LOG4PAWN_nop() { }

#if LOGLEVEL>4
	#define LOG4PAWN_DEBUG true
	#define logdebug(%1) printf(%1)
	#define debug(%1) printf(%1)
#else
	#define logdebug(%1) LOG4PAWN_nop()
	#define debug(%1) LOG4PAWN_nop()
#endif

#if LOGLEVEL>3
	#define LOG4PAWN_INFO true
	#define loginfo(%1) printf(%1)
#else
	#define loginfo(%1) LOG4PAWN_nop()
#endif

#if LOGLEVEL>2
	#define LOG4PAWN_WARN true
	#define logwarn(%1) printf(%1)
#else
	#define logwarn(%1) LOG4PAWN_nop()
#endif

#if LOGLEVEL>1
	#define LOG4PAWN_ERROR true
	#define logerror(%1) printf(%1)
#else
	#define logerror(%1) LOG4PAWN_nop()
#endif

#if LOGLEVEL>0
	#define LOG4PAWN_FATAL true
	#define logfatal(%1) printf(%1)
#else
	#define logfatal(%1) LOG4PAWN_nop()
#endif
