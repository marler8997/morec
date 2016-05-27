#ifndef LOG_MACROS_C
#define LOG_MACROS_C

#ifdef DEBUG

#define logDebug(printArgs...) fprintf(stderr, "(DEBUG) " printArgs)

#else 

#define logDebug(printArgs...) 

#endif


//#define logAttack(printArgs...) fprintf(stderr, "(Attack) " printArgs)
#define logSqlInjection(inputName, value) fprintf(stderr, "(SqlInjection %s) the input " inputName " contained sql injection characters '%s'", getenv("REMOTE_ADDR"), value);

#define logError(printArgs...) fprintf(stderr, "(Error) " printArgs)
#define logWarning(printArgs...) fprintf(stderr, "(Warning) " printArgs)


#define logBadPostData(printArgs...) logError("(BadPostData) " printArgs)
#define logMissingEnv(env) logError("(MissingEnvironmentVariable %s) ", env)
#define logOutOfMemory(codeLocation) logError("(OutOfMemory) " codeLocation)

// TODO: Make this include the MYSQL structure, and automatically add errno and error string
#define logMysqlError(mysql, function) logError("(Mysql " function " %u) %s", mysql_errno(mysql), mysql_error(mysql))



#define logArgumentError(argName, printArgs...) logError("ArgumentError: '" argName "': " printArgs)

#define logBadCookie(cookieName, printArgs...) logError("(BadCookie " cookieName ") " printArgs)



#endif
