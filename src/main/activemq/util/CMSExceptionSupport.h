/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_UTIL_CMSEXCEPTIONSUPPORT_H_
#define _ACTIVEMQ_UTIL_CMSEXCEPTIONSUPPORT_H_

#include <activemq/util/Config.h>

#include <cms/CMSException.h>
#include <cms/CMSSecurityException.h>
#include <cms/MessageEOFException.h>
#include <cms/MessageFormatException.h>
#include <cms/MessageNotReadableException.h>
#include <cms/MessageNotWriteableException.h>
#include <cms/InvalidClientIdException.h>
#include <cms/InvalidDestinationException.h>
#include <cms/InvalidSelectorException.h>
#include <cms/IllegalStateException.h>
#include <cms/UnsupportedOperationException.h>

#include <decaf/lang/Exception.h>

#include <string>

namespace activemq {
namespace util {

    class AMQCPP_API CMSExceptionSupport {
    private:

        CMSExceptionSupport();

    public:

        virtual ~CMSExceptionSupport();

        static cms::CMSException create( const std::string& msg, const decaf::lang::Exception& cause );

        static cms::CMSException create( const decaf::lang::Exception& cause );

        static cms::MessageEOFException createMessageEOFException( const decaf::lang::Exception& cause );

        static cms::MessageFormatException createMessageFormatException( const decaf::lang::Exception& cause );

    };

}}

/**
* Macro for catching an exception of one type and then re-throwing
* as a Basic CMSException, good for cases where the method isn't specific
* about what CMS Exceptions are thrown, bad if you need to throw an
* exception of MessageNotReadableException for instance.
*/
#define AMQ_CATCH_ALL_THROW_CMSEXCEPTION() \
    catch( cms::CMSSecurityException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::IllegalStateException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::InvalidClientIdException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::InvalidDestinationException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::InvalidSelectorException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::MessageEOFException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::MessageFormatException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::MessageNotReadableException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::MessageNotWriteableException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::UnsupportedOperationException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( cms::CMSException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex; \
    } catch( activemq::exceptions::ActiveMQException& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        throw ex.convertToCMSException(); \
    } catch( decaf::lang::Exception& ex ){ \
        ex.setMark( __FILE__, __LINE__ ); \
        activemq::exceptions::ActiveMQException amqEx( ex ); \
        throw amqEx.convertToCMSException(); \
    } catch( std::exception& ex ){ \
        throw cms::CMSException( ex.what(), NULL ); \
    } catch(...) { \
        throw cms::CMSException( "Caught Unknown Exception", NULL ); \
    }

#endif /* _ACTIVEMQ_UTIL_CMSEXCEPTIONSUPPORT_H_ */
