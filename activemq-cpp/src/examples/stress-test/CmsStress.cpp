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

#include <activemq/library/ActiveMQCPP.h>

#include "TestSenderAndReceiver.h"
#include "ConnectionFactoryMgr.h"
#include "BrokerMonitor.h"
#include "MessagingTask.h"

#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/atomic/AtomicInteger.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace cms;
using namespace cms::stress;
using namespace activemq::library;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util::concurrent;
using namespace decaf::util::concurrent::atomic;

bool VERBOSE = false;
static bool bPause = false;

decaf::util::concurrent::CountDownLatch* quit;

TESTINFO TestResults;
int InitialPrivateMemorySize;

////////////////////////////////////////////////////////////////////////////////
void DisplayResults() {
    long long mills = TestResults.endTime - TestResults.startTime;
    int secs = (int) (mills / 1000);
    int mins = secs / 60;
    int hrs = mins / 60;
    int days = hrs / 24;

    printf("\nT E S T   S U M M A R Y\n");

    if (days > 0) {
        printf("Elapsed time = %d:%02.2d:%02.2d:%02.2d.%03.3d\n", days, hrs % 24, mins % 60, secs % 60, mills % 1000);
    } else {
        printf("Elapsed time = %02.2d:%02.2d:%02.2d.%03.3d\n", hrs % 24, mins % 60, secs % 60, mills % 1000);
    }

    printf("Threads used = %d\n", TestResults.threadCount);

    printf("Messages sent = %d\n", TestResults.sent.get());
    printf("Messages received = %d\n", TestResults.received.get());
    if (TestResults.invalidMessages.get()) {
        printf("Invalid Messages = %d\n", TestResults.invalidMessages.get());
    }
    if (TestResults.badSequenceMessages.get()) {
        printf("Sequence Errors = %d\n", TestResults.badSequenceMessages.get());
    }
    if (TestResults.sendErrors.get()) {
        printf("Send Errors = %d\n", TestResults.sendErrors.get());
    }

    if (TestResults.sent.get() > 0) {
        printf("Reliability = %0.3f%%\n", (double) (((double) (TestResults.received.get()) * 100) / (double) TestResults.sent.get()));
        printf("Sequenced = %0.3f%%\n", (double) (((double) (TestResults.received.get() - TestResults.badSequenceMessages.get()) * 100) / (double) TestResults.sent.get()));
        printf("Output performance = %0.3f ms/msg\n", (double) ((double) mills / (double) TestResults.sent.get()));
        if ((TestResults.received.get() + TestResults.invalidMessages.get()) > 0) {
            printf("Input performance = %0.3f ms/msg\n", (double) ((double) mills / (double) (TestResults.received.get() + TestResults.invalidMessages.get())));
        }
    }

    if (bPause) {
        printf("Press any key to continue...\n");
        std::cin.get();
        printf("\n");
    }
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {

    BrokerMonitor* monitor = NULL;
    std::string url = "tcp://";
    int cnt = 25;
    int done = 3600;
    int monint = 5;
    int inter = -1;
    int seed = 0;
    int thrdmax = 100;
    int thrdmin = 3;
    char *ptr;
    bool bOK = true;
    bool bReport = false;
    bool bPool = true;
    const char* pName = NULL;
    const char* pHeader = NULL;

    printf("\n");

    for (int i = 1; bOK && (i < argc); i++) {
        if (argv[i][0] != '-') {
            bOK = false;
        } else {
            switch (argv[i][1]) {
            case 'a':
                bPool = false;
                break;
            case 'b':
                monint = Integer::parseInt(argv[i + 1]);
                i++;
                break;
            case 'f':
                thrdmin = Integer::parseInt(argv[i + 1]);
                i++;
                break;
            case 'g':
                thrdmax = Integer::parseInt(argv[i + 1]);
                i++;
                break;
            case 'i':
                inter = Integer::parseInt(argv[i + 1]);
                i++;
                break;
            case 'n':
                pName = argv[i + 1];
                i++;
                break;
            case 'p':
                bPause = true;
                break;
            case 'r':
                bReport = true;
                break;
            case 's':
                done = Integer::parseInt(argv[i + 1]);
                i++;
                break;
            case 't':
                cnt = Integer::parseInt(argv[i + 1]);
                i++;
                break;
            case 'u':
                pHeader = argv[i + 1];
                i++;
                break;
            case 'v':
                VERBOSE = true;
                break;
            case 'x':
                seed = Integer::parseInt(argv[i + 1]);
                i++;
                break;
            default:
                bOK = false;
                break;
            }
        }
    }

    if (!bOK) {
        printf("Usage: %s [-b #] [-i #] [-m] [-n name] [-p] [-r] [-s #] [-t #] [-u header] [-v] [-x # ]\n", argv[0]);
        printf(" -a   : Do not use thread pools when dispatching incoming messages (default is to use pools)\n");
        printf(" -b # : Number of seconds between heartbeat messages (0 disables) (default is 5)\n");
        printf(" -f # : Minimum number of threads to have available when use thread pools (default is 3)\n");
        printf(" -g # : Maximum number of threads that can be created in each thread pool (default is 100)\n");
        printf(" -i # : Number of milliseconds to wait between message sends (-1 is random delay between 0 and 1000ms) (default is -1)\n");
        printf(" -n s : Specify the name of the topic to use (default is 'topic')\n");
        printf(" -p   : Pause for keyboard input (default is false)\n");
        printf(" -r   : Display test summary (default is false)\n");
        printf(" -s # : Number of seconds to run test for (0 is until CTRL+C is entered) (default is 3600 - 1hr)\n");
        printf(" -t # : Number of threads to use (default is 25)\n");
        printf(" -u s : Use the specified header name for ID and utilize selectors\n");
        printf(" -v   : Display verbose progress where the following symbols have the specified meaning:\n");
        printf("        (%c) = Good send of a message\n", SYM_GOOD_SEND);
        printf("        (%c) = Error encountered sending a message\n", SYM_BAD_SEND);
        printf("        (%c) = Good sequenced message received\n", SYM_GOOD_SEQ);
        printf("        (%c) = Invalid message received\n", SYM_BAD_MSG);
        printf("        (%c) = Received message sequence is off by 10 or more\n", SYM_BIG_DIFF);
        printf("        (#) = Number indicating how far off received sequence number is\n");
        printf("        (%c) = Good heartbeat sent and received\n", SYM_MON_GOOD);
        printf("        (%c) = Failed to send and receive heartbeat\n", SYM_MON_BAD);
        printf("        (%c) = CMS exception received while send/receiving heartbeat\n", SYM_MON_CMS);
        printf("        (%c) = Exception received while send/receiving heartbeat\n", SYM_MON_EXC);
        printf(" -x # : Seed the random number generator with the specified seed (0 uses time) (default is 0)\n");
        printf("\n");
        return -1;
    }

    ptr = getenv("MessageBrokerIP");
    if (ptr != NULL) {
        url += ptr;
    } else {
        url += "127.0.0.1";
    }
    url += ":";
    ptr = getenv("MessageBrokerPort");
    if (ptr != NULL) {
        url += ptr;
    } else {
        url += "61616";
    }
    url += "?connection.sendTimeout=1000";

    ActiveMQCPP::initializeLibrary();
    ConnectionFactoryMgr::initialize();
    MessagingTask::initializeThreads(thrdmin, thrdmax);
    quit = new CountDownLatch(1);

    TestSenderAndReceiver** sar = new TestSenderAndReceiver*[cnt];
    TestResults.threadCount = cnt;
    TestResults.lastSequence = new AtomicInteger[cnt];

    if (pName == NULL) {
        pName = "topic";
    }
    if (pHeader == NULL) {
        pHeader = "";
    }

#ifdef _UNICODE
    wstring wstr(pName);
    string nstr(wstr.begin(), wstr.end());
    wstring whdr(pHeader);
    string nhdr(whdr.begin(), whdr.end());
#else
    string nstr(pName);
    string nhdr(pHeader);
#endif

    TestResults.startTime = System::currentTimeMillis();

    if (monint > 0) {
        monitor = new BrokerMonitor(url, monint * 1000, quit);
        monitor->start();
    }

    for (int i = 0; i < cnt; i++) {
        string topic(nstr);

        // If not using selectors send to different topic names
        if (nhdr == "") {
            stringstream str;
            str << i;
            topic += str.str();
        }
        sar[i] = new TestSenderAndReceiver(url, topic.c_str(), nhdr.c_str(), true, false, monitor, quit, 50, 1000, i, bPool, inter, seed);
        sar[i]->init();
    }

    if (done != 0) {
        quit->await(done * 1000);
    } else {
        quit->await();
    }

    quit->countDown();

    for (int i = 0; i < cnt; i++) {
        sar[i]->close();
        delete sar[i];
    }
    delete sar;

    if (monitor != NULL) {
        monitor->close();
        delete monitor;
    }

    TestResults.endTime = System::currentTimeMillis();
    printf("\n");

    delete[] TestResults.lastSequence;
    MessagingTask::terminateThreads();
    ConnectionFactoryMgr::unInitialize();
    delete quit;
    ActiveMQCPP::shutdownLibrary();

    if (bReport) {
        DisplayResults();
    }

    printf("\nTest Completed!\n");
}
