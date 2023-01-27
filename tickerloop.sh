#!/bin/bash

cd /home/pi/youminds-sportticker



echo "Intro"

# Hostname for Webserver
HN=$(hostname  -s)
# IP Adress for Webserver
IPA=$(hostname  -I | cut -f1 -d' ')

echo $HN
echo $IPA

# Intro einmal scrollen
bash scrollmessage.sh "Welcome to YouMinds  Sportticker, at -> http://$HN or http://$IPA" 1 999999

while
{
	bash printmessage.sh "-->>--" 999999

	echo "Receive Ticker data";date

	BLTXT=$(bash ticker.sh BL1 Bundesliga)

	CLTXT=$(bash ticker.sh CL "Champions League")

	PLTXT=$(bash ticker.sh PL "Premier League")

	PDTXT=$(bash ticker.sh PD "La Liga")

	FLTXT=$(bash ticker.sh FL1 "Ligue 1")

	SATXT=$(bash ticker.sh SA "Serie A")




	echo "Show Ticker";date

	# Ticker mehrmals mal scrollen

	for ((chrono=0; chrono < 2; chrono++))
	do
		bash splitscrollmessage.sh "$BLTXT" AA0000
		bash splitscrollmessage.sh "$CLTXT" 0033FF
		bash splitscrollmessage.sh "$PLTXT" CC3366
		bash splitscrollmessage.sh "$PDTXT" AAAA00
		bash splitscrollmessage.sh "$FLTXT" 00AA00
		bash splitscrollmessage.sh "$SATXT" 00AAAA
	done


} || {

	# IP Adress
	IPA=$(hostname  -I | cut -f1 -d' ')

	FTXT="An error occured, please check your WLAN connection ($IPA)...try again in 3 minutes..."

	echo $FTXT

	bash scrollmessage.sh "$FTXT" 1 FFFF00

	sleep 180

	# IP Adress
	IPA=$(hostname  -I | cut -f1 -d' ')

	bash scrollmessage.sh "$IPA" 1 999999
}
do true; done

