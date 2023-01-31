#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <regex>
#include <sstream>
#include <map>


const std::string QUERY_PARAM_ID = "id";
const std::string QUERY_PARAM_LEAGUE_DETAILS_PAGE = "leaguedetails";
const std::string QUERY_PARAM_ACTION = "action";
const std::string QUERY_PARAM_ACTION_TOGGLE_ONOFF = "toggleonoff";
const std::string QUERY_PARAM_MAIN_PAGE = "main";

const std::string QUERY_PARAM_SET_SPEED = "setspeed";
const std::string QUERY_PARAM_SET_COLOR = "setcolor";
const std::string QUERY_PARAM_SET_MODE = "setmode";

const std::string MODE_TODAY_ONLY = "todayonly";
const std::string MODE_TODAY_AND_STANDINGS = "todayandstandings";
const std::string MODE_GAMEDAY_ONLY = "gamedayonly";
const std::string MODE_GAMEDAY_AND_STANDINGS = "gamedayandstandings";


std::string currentpage = "";

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (std::getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}




class LeagueConf
{
public:

        LeagueConf(std::string leagueID_p);

        void Save();
        std::string Id;

        bool Active;
        std::string Color;
        int Speed;
        std::string Mode;
        std::string Name;

private:
        std::string filepath;

};


class LeagueInfo
{
public:
	LeagueInfo(
        std::string id_p,
        std::string name,
        std::string logo,
        std::string country,
        std::string countrycode
	);

	std::string ExpandTemplateFile(std::string file);

	std::string ExpandTemplateLine(std::string line);

	LeagueConf * GetLeagueConf();

private:
	LeagueConf * _LeagueConf;

public:
	std::string Id;
	std::string Name;
	std::string Logo;
	std::string Country;
	std::string CountryCode;
};


LeagueConf * LeagueInfo::GetLeagueConf()
{
	if ( NULL == _LeagueConf )
		_LeagueConf = new LeagueConf(Id);

	return _LeagueConf;
}

LeagueInfo::LeagueInfo(
	std::string id_p,
	std::string name_p,
	std::string logo_p,
	std::string country_p,
	std::string countrycode_p
	)
{
	Id = id_p;
	Name = name_p;
	Logo = logo_p;
	Country = country_p;
	CountryCode = countrycode_p;
}


std::string LeagueInfo::ExpandTemplateLine(std::string line)
{
	// Values from Info
               line = std::regex_replace(line,std::regex("\\{league\\}"),Id);
                line = std::regex_replace(line,std::regex("\\{name\\}"), Name);
                line = std::regex_replace(line,std::regex("\\{logo\\}"), Logo);
                line = std::regex_replace(line,std::regex("\\{country\\}"), Country);
                line = std::regex_replace(line,std::regex("\\{countrycode\\}"), CountryCode);

	// Values from Config
	line = std::regex_replace(line,std::regex("\\{color\\}"), GetLeagueConf()->Color);
	line = std::regex_replace(line,std::regex("\\{speed\\}"), std::to_string(GetLeagueConf()->Speed));
	line = std::regex_replace(line,std::regex("\\{mode\\}"), GetLeagueConf()->Mode);
	line = std::regex_replace(line,std::regex("\\{active\\}"), GetLeagueConf()->Active ? "on" : "off");


	// Links
                line = std::regex_replace(line,std::regex("\\{detailslink\\}"),"?" + QUERY_PARAM_LEAGUE_DETAILS_PAGE + "&" + QUERY_PARAM_ID + "=" + Id);

	// Actions
                line = std::regex_replace(line,std::regex("\\{onofflink\\}"),"?" + currentpage + "&" + QUERY_PARAM_ID + "=" + Id + "&" + QUERY_PARAM_ACTION + "=" + QUERY_PARAM_ACTION_TOGGLE_ONOFF);
                line = std::regex_replace(line,std::regex("\\{setspeed\\}"),"?" + currentpage + "&" + QUERY_PARAM_ID + "=" + Id + "&" + QUERY_PARAM_SET_SPEED + "=");
                line = std::regex_replace(line,std::regex("\\{setcolor\\}"),"?" + currentpage + "&" + QUERY_PARAM_ID + "=" + Id + "&" + QUERY_PARAM_SET_COLOR + "=");
                line = std::regex_replace(line,std::regex("\\{setmode\\}"),"?" + currentpage + "&" + QUERY_PARAM_ID + "=" + Id + "&" + QUERY_PARAM_SET_MODE + "=");

	return line;

}

std::string LeagueInfo::ExpandTemplateFile(std::string file)
{
        std::ifstream input( file);

        std::stringstream ret;

        for( std::string line; getline( input, line ); )
        {
		line = ExpandTemplateLine(line);

                ret << line<<std::endl;
        }

	input.close();

        return ret.str();
}




// Read Config e.g.: off;00AA00;35;gamedayonly;Eredivise
LeagueConf::LeagueConf(std::string leagueID_p)
{
	filepath = "/home/pi/youminds-sportticker/conf/" + leagueID_p + ".cfg";

	std::ifstream input( filepath );

	std::string line;

	getline( input, line );

	input.close();

	std::vector<std::string> values = split(line,';');

	if ( values.size()== 5 )
	{
		Active = ( values[0] == "on" );

		Color = values[1];

		Speed = std::stoi(values[2]);

		Mode = values[3];

		Name = values[4];
	}
	else
	{
		Color = "FF0000";
		Speed = 35;
		Mode = MODE_GAMEDAY_ONLY;
		Name = "?";
	}
}

void LeagueConf::Save()
{
	std::cout << "<br>" << std::endl;
	std::cout << filepath << std::endl;

	std::ofstream output( filepath);

	output << (Active ? "on;" : "off;");

	output << Color << ";";
	output << std::to_string(Speed) << ";";
	output << Mode << ";";
	output << Name;

	output << std::endl;

	output.close();

}

std::map<std::string, LeagueInfo*> LEAGUE_INFO_MAP = {
{"CL", new LeagueInfo("CL","Chamnpions League", "logo_cl", "Europa", "EUR")},
{"PPL", new LeagueInfo("PPL", "Primeira Liga", "portugueseprimeradivision", "Portugal", "765")},
{"PL", new LeagueInfo("PL", "Premier League", "logo_premier_league", "England", "770")},
{"DED", new LeagueInfo("DED", "Eredivisie", "logo_eredivisie", "Netherlands", "8601")},
{"BL1", new LeagueInfo("BL1", "Bundesliga", "logo_bundesliga", "Deutschland", "759")},
{"FL1", new LeagueInfo("FL1", "Ligue 1", "logo_ligue_1", "France", "773")},
{"SA", new LeagueInfo("SA", "Serie A", "logo_serie_a", "Italy", "784")},
{"PD", new LeagueInfo("PD", "La Liga", "logo_laliga", "Spain", "760")},
{"ELC", new LeagueInfo("ELC", "Championship", "championship", "England", "770")},
{"BSA", new LeagueInfo("BSA", "Serie A", "logo_brazil_serie_a", "Brazil", "764")}
};



int main()
{
	// Write Header
	std::cout<<"Content-type: text/html"<<std::endl;
	std::cout<<"Cache-Control: no-cache"<<std::endl;
	std::cout<<std::endl;

	std::string squery = getenv("QUERY_STRING");

	// Read and parse query string
	std::vector<std::string> vquery = split(squery,'&');

	currentpage = QUERY_PARAM_MAIN_PAGE;

	if ( vquery.size() > 0 )
		currentpage = vquery[0];

	std::string templatefile = currentpage + ".html";

	std::map<std::string,std::string> querymap;

	for (auto const& queryitem : vquery)
	{
		std::vector<std::string> querytuple = split(queryitem,'=');

		if ( querytuple.size() == 2 )
		{
			querymap[querytuple[0]] = querytuple[1];
		}
	}



	// Handle parameters and actions
	// Leage ID available the resolve league info ?
	std::string  id = querymap[QUERY_PARAM_ID];

	LeagueInfo * queriedleagueinfo = NULL;

	if ( id != "" )
		queriedleagueinfo = LEAGUE_INFO_MAP[id];

	// Action available ?
	std::string  action = querymap[QUERY_PARAM_ACTION];

	if ( action.length() != 0 )
	{
		// Handle action
		if ( action == QUERY_PARAM_ACTION_TOGGLE_ONOFF )
		{
			if ( NULL != queriedleagueinfo )
			{
				// Load configuration
				LeagueConf * conf = queriedleagueinfo->GetLeagueConf();
				// Modify value
				conf->Active = (! conf->Active);
				// Persist
				conf->Save();
			}
		}
	}


	if ( NULL != queriedleagueinfo )
	{
		std::string  action_set_color = querymap[QUERY_PARAM_SET_COLOR];
        	if ( action_set_color.length() != 0 )
        	{
			// Load configuration
			LeagueConf * conf = queriedleagueinfo->GetLeagueConf();
			// Modify value
			conf->Color = action_set_color;
			// Persist
			conf->Save();
		}

               std::string  action_set_speed = querymap[QUERY_PARAM_SET_SPEED];
                if ( action_set_speed.length() != 0 )
                {
                        // Load configuration
                        LeagueConf * conf = queriedleagueinfo->GetLeagueConf();
                        // Modify value
                        conf->Speed = std::stoi(action_set_speed);
                        // Persist
                        conf->Save();
                }

               std::string  action_set_mode = querymap[QUERY_PARAM_SET_MODE];
                if ( action_set_mode.length() != 0 )
                {
                        // Load configuration
                        LeagueConf * conf = queriedleagueinfo->GetLeagueConf();
                        // Modify value
                        conf->Mode = action_set_mode;
                        // Persist
                        conf->Save();
                }
	}

	// Expand template file
	std::ifstream input( templatefile );

	// Prepare replacement text block for league item's in list:league token
	std::stringstream list_leages;

	for (auto const& linfo : LEAGUE_INFO_MAP)
	{
		list_leages << linfo.second->ExpandTemplateFile("leagueitem.inc");
	}

	// Expand all lines of template file
	for( std::string line; getline( input, line ); )
	{
		line = std::regex_replace(line,std::regex("\\{list:league\\}"), list_leages.str());
		line = std::regex_replace(line,std::regex("\\{homelink\\}"), "?" + QUERY_PARAM_MAIN_PAGE);

		if ( NULL != queriedleagueinfo )
			line = queriedleagueinfo->ExpandTemplateLine(line);

		std::cout << line<<std::endl;
	}


	input.close();

	return 0;
}



