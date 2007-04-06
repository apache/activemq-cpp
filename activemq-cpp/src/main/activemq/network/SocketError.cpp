#include "SocketError.h"
#include <activemq/util/Config.h>

#if defined(HAVE_WINSOCK2_H)
    #include <Winsock2.h>
#else
    #include <errno.h>
#endif

using namespace activemq;
using namespace activemq::network;

////////////////////////////////////////////////////////////////////////////////
std::string SocketError::getErrorString() {
    
    std::string returnValue;
    
    #if defined(HAVE_WINSOCK2_H)
    
        // If the socket was temporarily unavailable - just try again.
        int errorCode = ::WSAGetLastError();
  
        // Create the error string.
        static const int errorStringSize = 512;
        char errorString[errorStringSize];
        memset( errorString, 0, errorStringSize );
        ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
           0,
           errorCode,
           0,
           errorString,
           errorStringSize - 1,
           NULL);
           
        returnValue = errorString;
        
    #else
         
        // Create the error string.
        returnValue = ::strerror(errno);
        
    #endif
    
    return returnValue;
}

