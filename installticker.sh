#!/bin/bash

sudo chmod +x tickerloop.sh


systemctl stop ticker
cp ticker.service /etc/systemd/system
systemctl daemon-reload
systemctl enable ticker
systemctl start ticker


