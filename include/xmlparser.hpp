/*
 * xmlparser.hpp
 *
 *      Author: marius
 */

#ifndef XMLPARSER_HPP_
#define XMLPARSER_HPP_

/*
	Filter XML datagram
	Initial Parameter XML datagram
	Motion binary datagram 1
 	Ping sequence XML datagram
 	Pulse XML datagram
	Sensor XML datagram
	Updated datagrams
	Environment XML datagram
	Parameter XML datagram
*/



#include <map>
#include <iostream>
#include <vector>
#include <XmlDatagram.hpp>
#include <boost/property_tree/ptree.hpp>

typedef boost::property_tree::ptree xmlTree;

class xmlparser {
private:
	XmlDatagram *dg;
	xmlTree *tree;
	std::string *input;
	void walk(std::string label);
public:

	void parser(std::vector<char> * input,  XmlDatagram *out);
};





#endif /* XMLPARSER_HPP_ */
