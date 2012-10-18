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

#ifndef _DECAF_UTIL_HASHCODE_H_
#define _DECAF_UTIL_HASHCODE_H_

#include <decaf/util/Config.h>

#include <decaf/lang/Pointer.h>
#include <decaf/lang/Float.h>
#include <decaf/lang/Double.h>

#include <string>
#include <functional>

namespace decaf {
namespace util {

    /**
     * Base HashCode template, specializations are created from this to account for
     * the various native types.
     *
     * @since 1.0
     */
    template<typename T>
    struct HashCode : public std::unary_function<T, int> {
    public:
        int operator()(T arg) const;
    };

    template<typename T>
    struct HashCode<T*> : public std::unary_function<T*, int> {
        int operator()(T* arg)  const {
            return reinterpret_cast<int>(arg);
        }
    };

    template<typename T>
    struct HashCode<const T&> : public std::unary_function<const T&, int> {
        int operator()(const T& arg) const {
            return HashCode<const T*>(&arg);
        }
    };

    template<>
    struct HashCode<bool> : public std::unary_function<bool, int> {
        int operator()(bool arg) const {
            return arg ? 1231 : 1237;
        }
    };

    #if defined(HAVE_WCHAR_T)
    template<>
    struct HashCode<unsigned char> : public std::unary_function<unsigned char, int> {
        int operator()(unsigned char arg) const {
            return (int) arg;
        }
    };
    #endif

    template<>
    struct HashCode<char> : public std::unary_function<char, int> {
        int operator()(char arg) const {
            return (int) arg;
        }
    };

    template<>
    struct HashCode<wchar_t> : public std::unary_function<wchar_t, int> {
        int operator()(wchar_t arg) const {
            return (int) arg;
        }
    };

    template<>
    struct HashCode<unsigned short> : public std::unary_function<unsigned short, int> {
        int operator()(unsigned short arg) const {
            return (int) arg;
        }
    };

    template<>
    struct HashCode<short> : public std::unary_function<short, int> {
        int operator()(short arg) const {
            return (int) arg;
        }
    };

    template<>
    struct HashCode<unsigned int> : public std::unary_function<unsigned int, int> {
        int operator()(unsigned int arg) const {
            return (int) arg;
        }
    };

    template<>
    struct HashCode<int> : public std::unary_function<int, int> {
        int operator()(int arg) const {
            return arg;
        }
    };

    template<>
    struct HashCode<unsigned long long> : public std::unary_function<unsigned long long, int> {
        int operator()(unsigned long long arg) const {
            return (int) (arg ^ (arg >> 32));
        }
    };

    template<>
    struct HashCode<long long> : public std::unary_function<long long, int> {
        int operator()(long long arg) const {
            return (int) (arg ^ ((unsigned long long) arg >> 32));
        }
    };

    template<>
    struct HashCode<float> : public std::unary_function<float, int> {
        int operator()(float arg) const {
            return decaf::lang::Float::floatToIntBits(arg);
        }
    };

    template<>
    struct HashCode<double> : public std::unary_function<double, int> {
        int operator()(double arg) const {
            long long value = decaf::lang::Double::doubleToLongBits(arg);
            return (int) (value ^ ((unsigned long long) value >> 32));
        }
    };

    template<>
    struct HashCode<std::string> : public std::unary_function<const std::string&, int> {
        int operator()(const std::string& arg) const {
            int h = 0;
            if (h == 0 && arg.length() > 0) {
                std::string::const_iterator iter = arg.begin();
                for (; iter != arg.end(); ++iter) {
                    h = 31 * h + (*iter);
                }
            }
            return h;
        }
    };

    template<typename T>
    struct HashCode< decaf::lang::Pointer<T> > : public std::unary_function<decaf::lang::Pointer<T>, int> {
        int operator()(decaf::lang::Pointer<T> arg) const {
            if (arg != NULL) {
                return HashCode<T>()(*arg);
            }
            return 0;
        }
    };

}}

#endif /* _DECAF_UTIL_HASHCODE_H_ */
