#!/bin/bash

# Prepare Conf directory
sudo chmod 777 ../conf

sudo chmod 666 ../conf/*.cfg

# Copy HTML Files
sudo cp ../html/*.html /var/www/html/
sudo cp *.html /usr/lib/cgi-bin/
sudo cp *.inc /usr/lib/cgi-bin/



# Compile CGI
sudo gcc template.cpp -lstdc++ -o "/usr/lib/cgi-bin/template.cgi"



