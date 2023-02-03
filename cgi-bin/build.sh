#!/bin/bash

echo "Building WebApp for YouMinds Sportticker"

echo "Prepare Conf directory"
sudo chmod 777 ../conf

sudo chmod 666 ../conf/*.cfg

echo "Copy HTML Files"
sudo mkdir /var/www/html/icons/
sudo cp -r ../html/* /var/www/html/




sudo cp *.html /usr/lib/cgi-bin/
sudo cp *.inc /usr/lib/cgi-bin/



echo "Compile CGI"
sudo gcc template.cpp -lstdc++ -o "/usr/lib/cgi-bin/template.cgi"


echo "Done"
