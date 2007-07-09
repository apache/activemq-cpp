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

#ifndef _BENCHMARK_PERFORMANCETIMER_H_
#define _BENCHMARK_PERFORMANCETIMER_H_

#include <activemq/util/Config.h>
#include <vector>

namespace benchmark{

    /**
     * This class is used to aid in testing the performace of code.  This class
     * maintains a running list of performance numbers for successive calls to
     * the method start and stop.  Once the desired number of tests has been run,
     * the user can call getAverageTime to find out the average time it took for
     * all start / stop cycles.
     */
    class PerformanceTimer {
    private:

        long long numberOfRuns;
        std::vector<long long> times;
        long long startTime;
        long long endTime;

    public:

        PerformanceTimer();
        virtual ~PerformanceTimer();

        /**
         * Starts the timer, time is from the call to start until a call
         * to stop is made.
         */
        void start();

        /**
         * Stops the timer and records the latest time.
         */
        void stop();

        /**
         * Resets the Timer to an unused state all data is thrown out.
         */
        void reset();

        /**
         * Gets the number of runs made so far
         * @returns unsigned int that counts the number of runs
         */
        unsigned int getNumberOfRuns() const {
            return numberOfRuns;
        }

        /**
         * Gets the overall average time that the count has recoreded
         * for all start / stop cycles.
         * @returns the average time for all the runs times / numberOfRuns
         */
        long long getAverageTime() const;

    };

}

#endif /*_PERFORMANCETIMER_H_*/
