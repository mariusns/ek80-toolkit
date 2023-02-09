/*
 * ek80file.cpp
 *
 *      Author: mariuss
 */
#include <iostream>
#include <boost/filesystem.hpp>
#include "Debug.hpp"
#include "ek80file.hpp"
#include "xmlparser.hpp"
#include <cstdio>
#include <map>

using namespace boost::filesystem;


ek80file::ek80file(std::string *fname)
{
	this->filename = new std::string(*fname);

	next.DgHeader = NULL;
	next.size = 0;
	next.readbuffer = NULL;

	parser = NULL;
}


ek80file::ek80file(std::string fname)
{
	this->filename = new std::string(fname);

	next.DgHeader = NULL;
	next.size = 0;
	next.readbuffer = NULL;

	parser = NULL;
}

/* Private methods */


bool ek80file::clearNext(void)
{
	bool ret = false;
	next.size = 0;
	if (next.DgHeader != NULL)
	{
		delete[] next.DgHeader;
	}

	next.DgHeader = new DatagramHeader;

	if(next.readbuffer != NULL)
	{
		delete[] next.readbuffer;
	}

	next.readbuffer = new std::vector<char>(10000);
	next.readbuffer->clear();
	next.status = false;

	return ret = true;
}

bool ek80file::getDatagram(nextDatagram *datagram)
{
	bool ret = false;

	if(next.DgHeader != NULL && next.status && datagram != NULL)
	{


		datagram->size = next.size;
		if(datagram->DgHeader != NULL)
		{
			memcpy(datagram->DgHeader, next.DgHeader, sizeof(DatagramHeader));
		}

		if(datagram->buffer == NULL && datagram->readbuffer != NULL)
		{
			datagram->readbuffer->clear();
			datagram->readbuffer->resize(next.size);
			std::copy(next.readbuffer->begin(), next.readbuffer->end(), datagram->readbuffer->data());
		} else
		{
			std::copy(next.readbuffer->begin(), next.readbuffer->end(), datagram->buffer);
		}
		datagram->status= next.status;
		ret = true;

	}

	return ret;
}


bool ek80file::clean(void)
{
	delete this->parser;
	this->parser = NULL;

	delete this->next.readbuffer;
	delete this->next.DgHeader;

	next.readbuffer = NULL;
	next.DgHeader = NULL;
	next.status = false;

	return true;
}

/* Public methods */

bool ek80file::open(void)
{

	bool ret = false;
	if(file != NULL && file->is_open())
	{
		return ret = true;
	}

	if( exists(boost::filesystem::path(this->filename->c_str())) )
	{
		try{
			file = new std::ifstream(filename->c_str(), std::fstream::binary);
		} catch(int e)
		{
			std::cout << "Error opening the file: " << filename << std::endl;
			return ret;
		}
		file->exceptions ( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit  );

		if(file != NULL)
			ret = clearNext();
		delete[] parser;
		parser = new xmlparser();
	}

	return ret;


}

bool ek80file::close(void)
{
	bool ret = false;
	if(instanceof<std::ifstream>(file) && file->is_open())
	{
		try{

			file->close();
		} catch (std::ifstream::failure &e) {

			std::cout << "Exception closing file: " << this->filename << ", " << e.code() << std::endl;
		}


		ret = clean();



	}
	return ret;
}


bool ek80file::reset(void)
{
	bool ret = false;
	if(file->is_open())
	{
		file->clear();
		file->seekg(0);

		clearNext();
		ret = true;
	}

	return ret;
}


bool ek80file::eof(void)
{
	if(file == NULL || ! file->is_open())
		return true;
	return file->eof();
}

bool ek80file::is_open(void)
{
	bool ret = false;
	if(file != NULL)
		ret = file->is_open();
	return ret;
}

int ek80file::read(void)
{
	/*
	 * long size
	 * DatagramHeader header
	 * Datagram
	 * long size
	 */


	int type = noneDatagram;
	//DEBUG("0");
	if(!file->is_open()) return type;

	//auto print =[](char * a){std::cout << a << std::endl;};
	if(!file->eof())
	{


		if(next.DgHeader == NULL)
		{
			clearNext();
		}

		try
		{
			file->read((char *)&next.size, sizeof(int));
		}catch(std::ifstream::failure e)
		{
			DEBUG("Closing file");
			close();
			return type;
		}

		next.size -= sizeof(DatagramHeader);
		try {
			file->read((char *)next.DgHeader, sizeof(DatagramHeader));
		}catch(std::ifstream::failure e)
		{
			close();
			return type;
		}
		//std::cout << "Reading position:  " << file->tellg() << std::endl;
		next.readbuffer->resize(next.size);

		try{
			file->read((char *)next.readbuffer->data(), next.size);
		}catch(std::ifstream::failure e)
		{
			close();
			return type;
		}
		int size = next.size + sizeof(DatagramHeader);

		try
		{
		file->read((char *)&next.size, sizeof(int));
		}catch(std::ifstream::failure e)
		{
			close();
			return type;
		}
		//DEBUG("6");
		next.status = (bool)(size == next.size);

		type = next.DgHeader->DatagramType;
		//std::cout << "Reading in file:  " << ", " << ((char*)&type)[0] <<((char*)&type)[1] << ((char*)&type)[2] << ((char*)&type)[3] << std::endl;
	}
	return type;
}


size_t ek80file::size(void)
{
	size_t size = 0;
	if(next.status)
	{
		size = next.size - sizeof(DatagramHeader);
	}
	return size;
}

DgTypes ek80file::getLastDatagramType(void)
{
	DgTypes type = noneDatagram;
	if(file->is_open() || ( next.DgHeader != NULL ))

	{
		type = (DgTypes)next.DgHeader->DatagramType;
	}
	return type;
}

bool ek80file::getDatagram(SampleDatagram *datagram, DatagramHeader *header)
{
	bool ret = false;
	if(next.status && next.DgHeader->DatagramType == sampleDatagram && datagram != NULL)
	{

		nextDatagram *dg = new nextDatagram;

		if(header != NULL)
		{
			dg->DgHeader = new DatagramHeader;
		}
		else
		{
			dg->DgHeader = NULL;
		}
		dg->readbuffer = new std::vector<char>(1);
		dg->buffer = (char*) datagram;

		ret = getDatagram(dg);

		if(header != NULL)
		{
			memcpy(header, dg->DgHeader, sizeof(DatagramHeader));
		}
		//std::copy(dg->readbuffer->begin(), dg->readbuffer->end(), reinterpret_cast<char *>(datagram));

		delete dg->DgHeader;
		delete dg->readbuffer;
		delete dg;
		ret = true;
	}

	return ret;
}

bool ek80file::getDatagram(FilterDatagram *datagram, DatagramHeader *header)
{

	bool ret = false;
	if(next.status && next.DgHeader->DatagramType == filterDatagram && datagram != NULL)
	{

		nextDatagram *dg = new nextDatagram;

		if(header != NULL)
		{
			dg->DgHeader = header;
			//dg->DgHeader = new DatagramHeader;
		}
		else
		{
			dg->DgHeader = NULL;
		}

		dg->readbuffer = new std::vector<char>(1);
		dg->buffer = (char*)datagram;

		ret = getDatagram(dg);

		if(header != NULL)
		{
			//memcpy(header, dg->DgHeader, sizeof(DatagramHeader));
		}


		//delete dg->DgHeader;
		delete dg->readbuffer;
		delete dg;
		ret = true;
	}

	return ret;
}

bool ek80file::getDatagram(DepthDatagram * datagram, DatagramHeader *header)
{
	bool ret = false;
	if(next.status && next.DgHeader->DatagramType == mru0Datagram && datagram != NULL)
	{

		nextDatagram *dg = new nextDatagram;

		if(header != NULL)
		{
			dg->DgHeader = new DatagramHeader;
		}
		else
		{
			dg->DgHeader = NULL;
		}
		dg->readbuffer = new std::vector<char>(1);
		dg->buffer = (char *) datagram;

		ret = getDatagram(dg);

		if(header != NULL)
		{
			memcpy(header, dg->DgHeader, sizeof(DatagramHeader));
		}
		//std::copy(dg->readbuffer->begin(), dg->readbuffer->end(), reinterpret_cast<char *>(datagram));

		delete dg->DgHeader;
		delete dg->readbuffer;
		delete dg;
		ret = true;
	}


	return ret;
}

bool ek80file::getDatagram(MRUDatagram *datagram, DatagramHeader *header)
{
	bool ret = false;
	if(next.status && (next.DgHeader->DatagramType == mru0Datagram || next.DgHeader->DatagramType == mru1Datagram) && datagram != NULL)
	{

		nextDatagram *dg = new nextDatagram;

		if(header != NULL)
		{
			dg->DgHeader = new DatagramHeader;
		}
		else
		{
			dg->DgHeader = NULL;
		}

		dg->readbuffer = NULL;
		dg->buffer = (char *) datagram;

		ret = getDatagram(dg);

		if(header != NULL)
		{
			memcpy(header, dg->DgHeader, sizeof(DatagramHeader));
		}

		delete dg->DgHeader;
		delete dg->readbuffer;
		delete dg;
		ret = true;
	}

	return ret;
}

bool ek80file::getDatagram(TextDatagram *datagram, DatagramHeader *header)
{
	bool ret = false;
	if(next.status && next.DgHeader->DatagramType == nmeaDatagram  && datagram != NULL)
	{

		nextDatagram *dg = new nextDatagram;

		if(header != NULL)
		{
			dg->DgHeader = new DatagramHeader;
		}
		else
		{
			dg->DgHeader = NULL;
		}

		dg->readbuffer = new std::vector<char>(1);
		dg->buffer = (char *) datagram;

		ret = getDatagram(dg);

		if(header != NULL)
		{
			memcpy(header, dg->DgHeader, sizeof(DatagramHeader));
		}

		delete dg->DgHeader;
		delete dg->readbuffer;
		delete dg;
		ret = true;
	}

	return ret;
}


bool ek80file::getDatagram(XmlDatagram *datagram, DatagramHeader *header)
{
	bool ret = false;


	if(next.DgHeader->DatagramType == xmlDatagram)
	{

		nextDatagram *dg = new nextDatagram;

		if(header != NULL)
		{
			dg->DgHeader = header;
		}
		else
		{
			dg->DgHeader = NULL;
		}
		dg->readbuffer = new std::vector<char>(1);
		ret = getDatagram(dg);
		if(ret)
		{

			parser->parser(dg->readbuffer, datagram);
		}
		//delete dg->DgHeader;
		delete dg->readbuffer;
		delete dg;
	}
	return ret;
}

