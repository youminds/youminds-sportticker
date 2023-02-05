#!/bin/bash



SPEED=35


if [[ $4 != "" ]] ; then
	SPEED=$4
fi


TXT=$(echo $1 | tr "'" . | tr "\"" !)

COMMAND="sudo ../rpi-ws2812-server/ws2812svr -i 'setup 1,256,3;init;brightness 1,16;config_2D 1,32,8,3,32,8;init_layer 1,0,1,0,1,0;message_board 1,0,8,32,8,0,$3,000000,$SPEED,$2,\"$TXT\",8,,0,../rpi-ws2812-server/enhanced_led_board-7.ttf;reset;exit;'"



echo "SCROLL [$3][$SPEED]: "$1


eval $COMMAND

