#ifndef SIMPLEAMQPCLIENT_UTIL_H
#define SIMPLEAMQPCLIENT_UTIL_H


#ifdef WIN32
#ifdef SimpleAmqpClient_EXPORTS
#define SIMPLEAMQPCLIENT_EXPORT __declspec(dllexport)
#else
#define SIMPLEAMQPCLIENT_EXPORT __declspec(dllimport)
#endif
#else
#define SIMPLEAMQPCLIENT_EXPORT
#endif

#if defined(__GNUC__) || defined(__clang__)
#define SAC_DEPRECATED(msg) __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define SAC_DEPRECATED(msg) __declspec(deprecated(msg))
#else
#define SAC_DEPRECATED(msg)
#endif

#endif  
