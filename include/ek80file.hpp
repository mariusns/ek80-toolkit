/*
 * ek80reader.hpp
 *
 *      Author: marius
 */

#ifndef EK80FILE_HPP_
#define EK80FILE_HPP_

#include <DepthDatagram.h>
#include <FilterDatagram.h>
#include <MRUDatagram.h>
#include <TextDatagram.h>
#include <SampleDatagram.h>
#include <DgTypes.hpp>
#include "xmlparser.hpp"

#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

template<typename Base, typename T>
inline bool instanceof(const T*) {
   return std::is_base_of<Base, T>::value;
}

class ek80file {
private:
	std::filesystem::path *filename;
	std::ifstream *file = NULL;
	xmlparser *parser;
	struct nextDatagram{
		int size;
		DatagramHeader *DgHeader;
		std::vector<char> *readbuffer;
		char * buffer = NULL;
		bool status;
	};
	struct nextDatagram next;
	bool clearNext(void);
	bool getDatagram(nextDatagram *datagram);
	std::map<std::string, std::string> processXML(std::string *xmldatagram);
	bool clean(void);

public:
	/* Open and close file */
	ek80file(std::string fname);
	ek80file(std::string *fname);

	bool open(void);
	bool close(void);
	bool reset(void);
	bool eof(void);
	bool is_open(void);

	/* Read next datagram and the size  */
	DgTypes read(void);

	/* Size of current datagram */
	size_t size(void);

	DgTypes getLastDatagramType(void);

	bool getDatagram(SampleDatagram *datagram, DatagramHeader *header=NULL);
	bool getDatagram(FilterDatagram *datagram, DatagramHeader *header=NULL);
	bool getDatagram(DepthDatagram * datagram, DatagramHeader *header=NULL);
	bool getDatagram(MRUDatagram *datagram, DatagramHeader *header=NULL);
	bool getDatagram(TextDatagram *datagram, DatagramHeader *header=NULL);
	bool getDatagram(XmlDatagram *datagram, DatagramHeader *header=NULL);

};


#endif /* EK80FILE_HPP_ */
