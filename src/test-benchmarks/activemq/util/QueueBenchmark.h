#ifndef _ACTIVEMQ_QUEUE_QUEUEBENCHMARK_H_
#define _ACTIVEMQ_QUEUE_QUEUEBENCHMARK_H_

#include <benchmark/BenchmarkBase.h>
#include <activemq/util/Queue.h>

namespace activemq{
namespace util{

    class QueueBenchmark :
        public benchmark::BenchmarkBase<
            activemq::util::QueueBenchmark, Queue<int> >
    {
    private:

        Queue<std::string> stringQ;
        Queue<int> intQ;

    public:

        QueueBenchmark();
        virtual ~QueueBenchmark() {}

        virtual void run();
    };

}}

#endif /*_ACTIVEMQ_QUEUE_QUEUEBENCHMARK_H_*/
