/*
*
* Distributed under the terms of the GNU General Public License v2
* Copyright 2023 Marius Norvoll Sletten
*
*/

/*
 * xmlparser.cpp
 *
 *      Author: marius
 */
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <exception>
#include <iostream>
#include <list>


namespace pt = boost::property_tree;

#include "xmlparser.hpp"


void xmlparser::walk(std::string label)
{


	BOOST_FOREACH(pt::ptree::value_type &v, tree->get_child(label))
	{
		std::string module = label + "."  + v.first;

		if(module.find("<xmlattr>")!=  std::string::npos && v.second.data().length() != 0)
		{
			std::string module_ = module;
			boost::erase_all(module_, ".<xmlattr>");
			module_.erase(std::remove(module_.begin(), module_.end(), '\n'), module_.end());
			dg->datagram.push_back(std::pair<std::string, std::string>(module_, v.second.data()));
		}

		walk(module);
	}

}

void xmlparser::parser(std::vector<char> * input, XmlDatagram *out)
{

	tree = new xmlTree;
	dg = out;
	std::string s(input->data());

	s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
	std::stringstream ss(s);

	pt::read_xml(ss, *tree);

	auto it = tree->begin();

	std::string  title = (*it).first;
	dg->type = title;

	walk(title);
}
