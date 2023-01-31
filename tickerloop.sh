#!/bin/bash

cd /home/pi/youminds-sportticker



echo "Start YouMinds Sportticker Loop"

# Hostname for Webserver
HN=$(hostname  -s)
# IP Adress for Webserver
IPA=$(hostname  -I | cut -f1 -d' ')


# Intro einmal scrollen
bash scrollmessage.sh "Welcome to YouMinds Sportticker -> http://$HN or http://$IPA" 1 999999

while
{
	bash printmessage.sh "-->>--" 999999

	#Iterate over the league configurations files

	declare -a LEAGUETEXTARRAY

	LEAGUETEXTARRAY=()

	declare -i TEXTLEN
	TEXTLEN=0

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

			LEAGUETEXT=$(bash ticker.sh $LEAGUE "$NAME" $MODE)

			TEXTLEN=$TEXTLEN+${#LEAGUETEXT}

			if [[ $LEAGUETEXT != "" ]] ; then

				LEAGUETEXTARRAY+=("$LEAGUETEXT")
				LEAGUETEXTARRAY+=("$COLOR")
				LEAGUETEXTARRAY+=("$SPEED")

			fi

		fi
	done


	if [[ ${#LEAGUETEXTARRAY[@]} == 0 ]] ; then

		# Nothing to show
		bash scrollmessage.sh "No data available, select leagues. Will try again in one minute." 1 999999

		bash printmessage.sh "..." 999999

		sleep 60

	else


		# Scroll Ticker several times
		declare -i LOOPS
		LOOPS=1

		if [[ $TEXTLEN < 200 ]] ; then

			((LOOPS++))

		fi
		if [[ $TEXTLEN < 100 ]] ; then

			((LOOPS++))

		fi
		if [[ $TEXTLEN < 50 ]] ; then

			((LOOPS++)
			((LOOPS++))

		fi

		for ((chrono=0; chrono < $LOOPS; chrono++))
		do
	        	for ((i=0;i < ${#LEAGUETEXTARRAY[@]};i++))
        		do
				TEXT=${LEAGUETEXTARRAY[i]}
				((i++))
				COLOR=${LEAGUETEXTARRAY[i]}
				((i++))
				SPEED=${LEAGUETEXTARRAY[i]}



				bash splitscrollmessage.sh "$TEXT" $COLOR $SPEED
        		done


		done

	fi


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

