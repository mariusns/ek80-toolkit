/*
 * XmlDatagram.hpp
 *
 *      Author: marius
 */

#ifndef XMLDATAGRAM_HPP_
#define XMLDATAGRAM_HPP_

#include <list>
#include <utility>
#include <string>

typedef struct XmlDatagram
{
	std::string type;
	std::list<std::pair<std::string, std::string>> datagram;
}XmlDatagram,*pXmlDatagram;




#endif /* XMLDATAGRAM_HPP_ */
