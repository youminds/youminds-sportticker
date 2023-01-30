#!/bin/bash

# Prepare Conf directory
sudo chmod 777 ../conf

sudo chmod 666 ../conf/*.cfg

# Compile CGI
sudo gcc template.cpp -lstdc++ -o "/usr/lib/cgi-bin/template.cgi"



