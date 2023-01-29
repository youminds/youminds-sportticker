#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <regex>
#include <sstream>
#include <map>





std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (std::getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}


enum LeagueMode
{
	Full,
	Gamedayonly,
	Gamedayandstandings
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


	std::string Id;
	std::string Name;
	std::string Logo;
	std::string Country;
	std::string CountryCode;
};


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
               line = std::regex_replace(line,std::regex("\\{league\\}"),Id);
                line = std::regex_replace(line,std::regex("\\{name\\}"), Name);
                line = std::regex_replace(line,std::regex("\\{logo\\}"), Logo);
                line = std::regex_replace(line,std::regex("\\{country\\}"), Country);
                line = std::regex_replace(line,std::regex("\\{countrycode\\}"), CountryCode);

                line = std::regex_replace(line,std::regex("\\{detailslink\\}"),"?leaguedetails&id=" + Id);


                line = std::regex_replace(line,std::regex("\\{onofflink\\}"),"?leaguedetails&id=" + Id + "&action=toggleonoff");

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


        return ret.str();
}



class LeagueConf
{
public:
	std::string Id;

	bool Active;
	std::string Color;
	int Speed;
	LeagueMode Mode;
	std::string Name;
};


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
	std::cout<<"Content-type: text/html"<<std::endl<<std::endl;

	std::string squery = getenv("QUERY_STRING");


	std::vector<std::string> vquery = split(squery,'&');

	std::string templatefile = "main.html";

	if ( vquery.size() > 0 )
		templatefile = vquery[0] + ".html";


	std::map<std::string,std::string> querymap;

	for (auto const& queryitem : vquery)
	{
		std::vector<std::string> querytuple = split(queryitem,'=');

		if ( querytuple.size() == 2 )
		{
			querymap[querytuple[0]] = querytuple[1];
		}
	}


	std::ifstream input( templatefile );

	std::stringstream list_leages;

	for (auto const& linfo : LEAGUE_INFO_MAP)
	{
		list_leages << linfo.second->ExpandTemplateFile("leagueitem.inc");
	}


	std::string  id = querymap["id"];

	std::cout << "ID=";

	std::cout << id;

	LeagueInfo * queriedleagueinfo = NULL;

	//if ( NULL != id )
	{
		queriedleagueinfo = LEAGUE_INFO_MAP[id];
	}


	for( std::string line; getline( input, line ); )
	{
		line = std::regex_replace(line,std::regex("\\{list:league\\}"), list_leages.str());
		line = std::regex_replace(line,std::regex("\\{homelink\\}"), "?main");

		if ( NULL != queriedleagueinfo )
			line = queriedleagueinfo->ExpandTemplateLine(line);

		std::cout << line<<std::endl;
	}



	return 0;
}



