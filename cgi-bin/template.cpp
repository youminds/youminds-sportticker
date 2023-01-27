#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <regex>
#include <sstream>

std::string GetLeageItem(
	std::string league,
	std::string name,
	std::string logo,
	std::string country,
	std::string countrycode
	)
{
	std::ifstream input( "leagueitem.html" );

	std::stringstream ret;

       	for( std::string line; getline( input, line ); )
        {
                line = std::regex_replace(line,std::regex("\\{league\\}"), league);
		line = std::regex_replace(line,std::regex("\\{name\\}"), name);
		line = std::regex_replace(line,std::regex("\\{logo\\}"), logo);
		line = std::regex_replace(line,std::regex("\\{country\\}"), country);
		line = std::regex_replace(line,std::regex("\\{countrycode\\}"), countrycode);

               	ret << line<<std::endl;
        }


	return ret.str();
}

int main()
{
	std::string squery = getenv("QUERY_STRING");

	std::string templatefile = squery + ".html";

	std::cout<<"Content-type: text/html"<<std::endl<<std::endl;

	std::ifstream input( templatefile );

	std::stringstream list_leages;

	list_leages << GetLeageItem("CL","Chamnpions League","logo_cl","Europa","EUR");
	list_leages << GetLeageItem("PPL","Primeira Liga","portugueseprimeradivision","Portugal","765");
	list_leages << GetLeageItem("PL","Premier League","logo_premier_league","England","770");
	list_leages << GetLeageItem("DED","Eredivisie","logo_eredivisie","Netherlands","8601");
	list_leages << GetLeageItem("BL1","Bundesliga","logo_bundesliga","Deutschland","759");
	list_leages << GetLeageItem("FL1","Ligue 1","logo_ligue_1","France","773");
	list_leages << GetLeageItem("SA","Serie A","logo_serie_a","Italy","784");
	list_leages << GetLeageItem("PD","La Liga","logo_laliga","Spain","760");
	list_leages << GetLeageItem("ELC","Championship","championship","England","770");
	list_leages << GetLeageItem("BSA","Serie A","logo_brazil_serie_a","Brazil","764");


	for( std::string line; getline( input, line ); )
	{
		line = std::regex_replace(line,std::regex("\\{list:league\\}"), list_leages.str());

		std::cout << line<<std::endl;
	}



	return 0;
}



