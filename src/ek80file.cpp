/*
 * ek80file.cpp
 *
 *      Author: mariuss
 */
#include <iostream>
#include <filesystem>
#include "Debug.hpp"
#include "ek80file.hpp"
#include "xmlparser.hpp"
#include <cstdio>
#include <map>



ek80file::ek80file(std::string *fname)
{
	this->filename = new std::filesystem::path(*fname);

	next.DgHeader = NULL;
	next.size = 0;
	next.readbuffer = NULL;

	parser = NULL;
}


ek80file::ek80file(std::string fname)
{
	this->filename = new std::filesystem::path(fname);

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
/*
 * Copy out from next struct
 *
 */

{
	bool ret = false;

	if(next.DgHeader != NULL && next.status && datagram != NULL)
	{


		datagram->size = next.size;
		if(datagram->DgHeader != NULL && sizeof(datagram->DgHeader) >= sizeof(DatagramHeader))
		{
			memcpy(datagram->DgHeader, next.DgHeader, sizeof(DatagramHeader));

		}


		if(datagram->buffer == NULL && datagram->readbuffer != NULL)
		{
			datagram->readbuffer->clear();
			datagram->readbuffer->resize(next.size);
			std::copy(next.readbuffer->begin(), next.readbuffer->end(), datagram->readbuffer->data());
			datagram->status= next.status;
			ret = next.status;
		}else if(datagram->buffer != NULL )
		{
			try{
				std::copy(next.readbuffer->begin(), next.readbuffer->end(), datagram->buffer);
			} catch(std::exception &e)
			{
				std::cout << "std::copy failed: "<< e.what() << std::endl;
			}

			datagram->status= next.status;
			ret = next.status;
		}


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

	if(std::filesystem::exists(*filename)  )
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

DgTypes ek80file::read(void)
{
	/*
	 * long size
	 * DatagramHeader header
	 * Datagram
	 * long size
	 */


	DgTypes type = noneDatagram;
	//DEBUG("0");
	if(!file->is_open()) return type;

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
		next.status = (bool)(size == next.size);

		type = (DgTypes) next.DgHeader->DatagramType;
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

/* Set pointers */
{
	bool ret = false;
	if(next.status && next.DgHeader->DatagramType == sampleDatagram && datagram != NULL)
	{

		nextDatagram dg = {0,header,NULL, (char *)datagram, 0};

		ret = getDatagram(&dg);


	}
	return ret;
}

bool ek80file::getDatagram(FilterDatagram *datagram, DatagramHeader *header)
/* Set pointers */
{
	bool ret = false;
	if(next.status && next.DgHeader->DatagramType == filterDatagram && datagram != NULL)
	{

		nextDatagram dg = {0,header,NULL, (char *)datagram, 0};
		ret = getDatagram(&dg);

	}

	return ret;
}

bool ek80file::getDatagram(DepthDatagram * datagram, DatagramHeader *header)
/* Set pointers */
{
	bool ret = false;
	if(next.status && next.DgHeader->DatagramType == noneDatagram && datagram != NULL)
	{

		nextDatagram dg = {0,header,NULL, (char *)datagram, 0};

		ret = getDatagram(&dg);


	}

	return ret;
}

bool ek80file::getDatagram(MRUDatagram *datagram, DatagramHeader *header)
/* Set pointers */
{
	bool ret = false;
	if(next.status && (next.DgHeader->DatagramType == mru0Datagram || next.DgHeader->DatagramType == mru1Datagram) && datagram != NULL)
	{

		nextDatagram dg = {0,header,NULL, (char *)datagram, 0};

		ret = getDatagram(&dg);


	}

	return ret;
}

bool ek80file::getDatagram(TextDatagram *datagram, DatagramHeader *header)
/* Set pointers */
{
	bool ret = false;
	if(next.status && next.DgHeader->DatagramType == textDatagram && datagram != NULL)
	{
		nextDatagram dg = {0,header,NULL, (char *)datagram, 0};
		ret = getDatagram(&dg);
	}

	return ret;
}


bool ek80file::getDatagram(XmlDatagram *datagram, DatagramHeader *header)
/* Set pointers */
{

	bool ret = false;
	if(next.status && next.DgHeader->DatagramType == xmlDatagram && datagram != NULL)
	{
		nextDatagram dg = {0,header,new std::vector<char>(1),NULL,0};
		ret = getDatagram(&dg);
		if(ret)
		{
			this->parser->parser(dg.readbuffer, datagram);
		}

	}

	return ret;
}

