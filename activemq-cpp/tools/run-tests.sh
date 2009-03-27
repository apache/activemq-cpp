#!/bin/bash
ulimit -s unlimited

for ((i=0;i<100;i+=1)); do

  ./start_activemq.sh
  sleep 12
  echo "Running Iteration =" $i
  ../build/src/test-integration/activemq-test-integration
  killall /opt/local/java/bin/java
  #./stop_activemq.sh
  sleep 4
done

