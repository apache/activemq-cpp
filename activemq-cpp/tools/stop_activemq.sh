#!/bin/bash
# Stop java and tail (if any)

/home/tabish/dev/apache-activemq-5.1.0/bin/activemq-admin stop --all
rm -rf /home/tabish/dev/apache-activemq-5.1.0/data/*
