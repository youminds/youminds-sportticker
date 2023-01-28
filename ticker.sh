#!/bin/bash

MYID=$(cat ../fdid)

# Receive Matches
MATJSON=$(curl -XGET -H "X-Auth-Token: $MYID" https://api.football-data.org/v4/competitions/$1/matches)

# Parse current match day
CMD=$(echo "$MATJSON" | jq ".matches[0].season.currentMatchday")

# Parse matches in play
#RESULTS=$(echo "$MATJSON" | jq -r --argjson CMD $CMD '[.matches[]|select(.matchday==$CMD and (.status=="PAUSED" or .status=="IN_PLAY") )|"\(.homeTeam.shortName) - \(.awayTeam.shortName) \(.score.fullTime.home) : \(.score.fullTime.away)"]|join(" .. ")')

# Parse matches only today
RESULTS=$(echo "$MATJSON" | jq -r --argjson CMD $CMD '[.matches[]|select(.utcDate[:10]==(now | strftime("%Y-%m-%d")) and (.status=="FINISHED" or .status=="PAUSED" or .status=="IN_PLAY") )|"\(.homeTeam.shortName) - \(.awayTeam.shortName) \(.score.fullTime.home) : \(.score.fullTime.away)"]|join(" .. ")')

if [[ $RESULTS == "" ]] ; then

	# No results
	# Parse all matches of gameday with regular season (no group plays) instead
	RESULTS=$(echo "$MATJSON" | jq -r --argjson CMD $CMD '[.matches[]|select(.matchday==$CMD and .stage=="REGULAR_SEASON" and (.status=="FINISHED" or .status=="PAUSED" or .status=="IN_PLAY") )|"\(.homeTeam.tla) - \(.awayTeam.tla) \(.score.fullTime.home):\(.score.fullTime.away)"]|join(" .. ")')

	if [[ $RESULTS == "" ]] ; then

		# No results
		# Receive standings instead
		STDJSON=$(curl -XGET -H "X-Auth-Token: $MYID" https://api.football-data.org/v4/competitions/$1/standings)

		# Parse standings, mind groups, set text block delimiter ; before group
		STANDINGS=$(echo "$STDJSON" | jq -r --argjson CMD $CMD '[.standings[]|select(.type=="TOTAL")|[.group,[.table[]|"\(.position). \(.team.shortName)"]]|(if (.[0] | length) > 6 then ";Group \(.[0][6:]):" else ";" end),"\(.[1][])"]|join(" ")')

		# Delimiter in the 12th position creates several scroll text blocks
		TXT=$2" Standings: ""${STANDINGS//12./;12.}"

	else

		TXT=$2" Matchday "$CMD":;"$RESULTS

	fi

else

	TXT=$2" Matchday "$CMD" today:;"$RESULTS

fi


echo $TXT



