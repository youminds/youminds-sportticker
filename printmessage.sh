#!/bin/bash



TXT=$(echo $1 | tr "'" . | tr "\"" !)

COMMAND="sudo ../rpi-ws2812-server/ws2812svr -i 'setup 1,256,3;init;brightness 1,16;config_2D 1,32,8,3,32,8;init_layer 1,0,1,0,1,0;print_text 1,0,8,\"$TXT\",$2,8,1,0,../rpi-ws2812-server/enhanced_led_board-7.ttf;render;exit;'"


eval $COMMAND

