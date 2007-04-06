#ifndef ACTIVEMQ_NETWORK_SOCKETERROR_H_
#define ACTIVEMQ_NETWORK_SOCKETERROR_H_

#include <string>

namespace activemq{
namespace network{
    
    class SocketError {  
    public:
    
        static std::string getErrorString();
    };
}}

#endif /*ACTIVEMQ_NETWORK_SOCKETERROR_H_*/
