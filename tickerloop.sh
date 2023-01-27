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

	#Iterate over the league configurations files

	declare -a LEAGUETEXTARRAY



	for n in ./conf/*.cfg
	do
		#printf '%s\n' "$(basename ${n%.*})"

		CFG=$(cat ${n})

		# Parse Parameter from configuration file 
		PARAS=(${CFG//;/ })

		# Read parameters
		LEAGUE=$(basename ${n%.*})
		ON=${PARAS[0]}
		COLOR=${PARAS[1]}
		SPEED=${PARAS[2]}
		MODE=${PARAS[3]}
		NAME=${PARAS[4]}

		# Receive Ticker Text for this league

		if [[ $ON == "on" ]] ; then

			LEAGUETEXT=$(bash ticker.sh $LEAGUE "$NAME")

			echo $LEAGUETEXT

			LEAGUETEXTARRAY+=("$LEAGUETEXT")

		fi
	done


	#BLTXT=$(bash ticker.sh BL1 Bundesliga)

	#CLTXT=$(bash ticker.sh CL "Champions League")

	#PLTXT=$(bash ticker.sh PL "Premier League")

	#PDTXT=$(bash ticker.sh PD "La Liga")

	#FLTXT=$(bash ticker.sh FL1 "Ligue 1")

	#SATXT=$(bash ticker.sh SA "Serie A")




	echo "Show Ticker";date

	# Scroll Ticker several times

	for ((chrono=0; chrono < 2; chrono++))
	do
	        for TEXT in "${LEAGUETEXTARRAY[@]}"
        	do
			echo $TEXT

			bash splitscrollmessage.sh "$TEXT" AA0000
        	done


		#bash splitscrollmessage.sh "$BLTXT" AA0000
		#bash splitscrollmessage.sh "$CLTXT" 0033FF
		#bash splitscrollmessage.sh "$PLTXT" CC3366
		#bash splitscrollmessage.sh "$PDTXT" AAAA00
		#bash splitscrollmessage.sh "$FLTXT" 00AA00
		#bash splitscrollmessage.sh "$SATXT" 00AAAA
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

