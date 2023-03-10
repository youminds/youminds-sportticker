#!/bin/bash



MYID=$(cat ../fdid)

# Receive Matches
MATJSON=$(curl -s -XGET -H "X-Auth-Token: $MYID" https://api.football-data.org/v4/competitions/$1/matches)

# Parse current match day
CMD=$(echo "$MATJSON" | jq ".matches[0].season.currentMatchday")

# Check Mode 
if [[ $3 == "todayonly" || $3 == "todayorgameday" || $3 == "" ]] ; then


	# Parse matches only today
	RESULTS=$(echo "$MATJSON" | jq -r --argjson CMD $CMD '[.matches[]|select(.utcDate[:10]==(now | strftime("%Y-%m-%d")) and (.status=="FINISHED" or .status=="PAUSED" or .status=="IN_PLAY") )|"\(.homeTeam.shortName) - \(.awayTeam.shortName) \(.score.fullTime.home) : \(.score.fullTime.away)"]|join(" .. ")')


	if [[ $RESULTS == "" ]] ; then

		if [[ $3 == "todayorgameday" ]] ; then

			# Parse all matches of gameday with regular season (no group plays) instead
			RESULTS=$(echo "$MATJSON" | jq -r --argjson CMD $CMD '[.matches[]|select(.matchday==$CMD and .stage=="REGULAR_SEASON" and (.status=="FINISHED" or .status=="PAUSED" or .status=="IN_PLAY") )|"\(.homeTeam.tla) - \(.awayTeam.tla) \(.score.fullTime.home):\(.score.fullTime.away)"]|join(" .. ")')

			if [[ $RESULTS != "" ]] ; then

				TXT=$2" Matchday "$CMD":;"$RESULTS

			fi

		fi
	else

		TXT=$2" Matchday "$CMD" today:;"$RESULTS

	fi

else

	# Parse all matches of gameday with regular season (no group plays) instead
	RESULTS=$(echo "$MATJSON" | jq -r --argjson CMD $CMD '[.matches[]|select(.matchday==$CMD and .stage=="REGULAR_SEASON" and (.status=="FINISHED" or .status=="PAUSED" or .status=="IN_PLAY") )|"\(.homeTeam.tla) - \(.awayTeam.tla) \(.score.fullTime.home):\(.score.fullTime.away)"]|join(" .. ")')

	if [[ $RESULTS != "" ]] ; then

		TXT=$2" Matchday "$CMD":;"$RESULTS
	else

		if [[ $3 == "gamedayandstandings" ]] ; then

			# Receive standings instead
			STDJSON=$(curl -s -XGET -H "X-Auth-Token: $MYID" https://api.football-data.org/v4/competitions/$1/standings)

			# Parse standings, mind groups, set text block delimiter ; before group
			STANDINGS=$(echo "$STDJSON" | jq -r --argjson CMD $CMD '[.standings[]|select(.type=="TOTAL")|[.group,[.table[]|"\(.position). \(.team.shortName)"]]|(if (.[0] | length) > 6 then ";Group \(.[0][6:]):" else ";" end),"\(.[1][])"]|join(" ")')

			if [[ $STANDINGS != "" ]] ; then

				# Delimiter in the 12th position creates several scroll text blocks
				TXT=$2" Standings: ""${STANDINGS//12./;12.}"

			fi

		fi

	fi
fi




echo $TXT

