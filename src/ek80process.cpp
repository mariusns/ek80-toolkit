/*
 * ek80process.cpp
 *
 *      Author: mariuss
 */

#include <filesystem>
#include "ek80process.hpp"
#include "DgTypes.hpp"
#include <memory>
#include <string>


ek80process::ek80process(std::string *fname)
{

	if(fname == NULL)
	{
		std::filesystem::path *p = new std::filesystem::path(*fname);
		if (std::filesystem::exists(*p))
		{
			file = new ek80file(fname);
		}
		delete p;
	}

}

ek80process::ek80process(std::string &fname)
{

	std::filesystem::path *p = new std::filesystem::path(fname.c_str());
	if (std::filesystem::exists(*p))
	{
		file = new ek80file(fname);
	}
	delete p;

}


/*Private methods */


bool ek80process::init(int length)
{
	bool ret = false;

	if(sample == NULL)
		sample = new DgData<pSampleDatagram>();

	if(xml == NULL)
	{
		xml = new struct Xml_s;
		xml->Configuration = new DgVector<XmlDatagram>();

		xml->Environment = new DgVector<XmlDatagram>();
		xml->Filter = new DgVector<XmlDatagram>();
		xml->InitialParameters = new DgVector<XmlDatagram>();
		xml->Parameter = new DgVector<XmlDatagram>();
	}
	if(filter == NULL)
		filter = new DgData<pFilterDatagram>();
	if(mru == NULL)
		mru = new DgData<MRUDatagram>();
	if(text == NULL)
		text = new DgData<pTextDatagram>();
	if(channels == NULL)
		channels = new Dictionary<int, std::string>();

	return ret = true;
}

std::pair<DatagramHeader, XmlDatagram>* ek80process::Xml_s::next(int i)
{

	if (this->Parameter == nullptr)
	{
		return nullptr;
	}

	if(i > idx)
	{
		return nullptr;
	}

	if(idx >= Parameter->size())
	{
		Parameter->resize(Parameter->size() + 100);
	}

	if(i == -1)
	{
		return (std::pair<DatagramHeader, XmlDatagram>*)&Parameter->at(idx++);
	}
	else
	{
		return (std::pair<DatagramHeader, XmlDatagram>*)&Parameter->at(i);
	}

}

bool ek80process::open(std::string *fname)
{


	bool ret = false;

	if(file== NULL && fname != NULL)
	{
		std::filesystem::path *p = new std::filesystem::path(*fname);
		if(std::filesystem::exists(*p))
		{
			std::cout << "File exists " << std::endl;
			file = new ek80file(fname);
		}
		else
		{
			std::cout << "ek80process::open, cant find file: " << fname << std::endl;
		}
		delete p;
	}


	if(file != NULL && !file->is_open())
	{


		ret = file->open();

	}
	else
	{
		std::cout << "File is null " << std::endl;

	}

	return ret;

}

bool ek80process::close(void)
{
	bool ret = false;

	if(file != NULL && file->is_open())
	{
		ret = file->close();
		delete file;
		file = NULL;
	}

	return ret;
}

bool ek80process::clear(void)
{
	bool ret = false;

	return ret;

}

bool ek80process::reset(void)
{
	bool ret = false;

	return ret;

}

bool ek80process::read(void)
{
	/* Read all data until next ping */

	bool ret = false;
	if(file== NULL ||  !file->open() || file->eof())
	{
		DEBUG("File is null");
		return ret;
	}
	if(!setBufferSize())
	{

		return ret;
	}
	if(!init())
	{
		return ret;
	}
	bool read = true;

	DgTypes type = noneDatagram;
	while(read && !file->eof())
	{
		try{
			type = (DgTypes)file->read();
		}catch(std::exception &e)
		{

			std::cerr << "Error reading file: " << e.what() << std::endl;
			type = noneDatagram;
			file->close();
			break;
		}

		std::cout << "Reading:  " <<", " << ((char*)&type)[0] <<((char*)&type)[1] << ((char*)&type)[2] << ((char*)&type)[3] << std::endl;

		switch(type)
		{
		case xmlDatagram:
		{

			if(this->pingInFile == 0)
			{
				/* Slow parsing */
				std::pair<DatagramHeader, XmlDatagram> *next =  new std::pair<DatagramHeader, XmlDatagram>();
				try {
					file->getDatagram(&next->second, &next->first);
				} catch(std::exception &e)
				{
					std::cerr << "Get xmldatagram: " << e.what() << std::endl;
				}

				std::cout << "XML type: " << next->second.type << std::endl;

				if(next->second.type.compare("Configuration")) /* Configuration */
				{
					xml->Configuration->push_back(*next);
				}else if(next->second.type.compare("")) /* Initial parameter */
				{
					xml->InitialParameters->push_back(*next);
				}else if(next->second.type.compare("")) /* Filter */
				{
					xml->Filter->push_back(*next);
				}else if(next->second.type.compare("")) /* NMEA */
				{

				}else if(next->second.type.compare("Environment")) /* Environment */
				{
					xml->Environment->push_back(*next);
				}else if(next->second.type.compare("Parameter")) /* Parameter */
				{
					xml->Parameter->push_back(*next);
				}

				delete next;
			}else
			{
				std::pair<DatagramHeader, XmlDatagram> *next = xml->next();
				if(next != nullptr)
				{
					file->getDatagram(&next->second, &next->first);
				}
				else
				{
					break;
					ret = false;
				}
				std::cout << "XML datagram type #2: " << next->second.type << std::endl;
			}

			break;
		}
		case sampleDatagram:
		{
			if(sample->full())
			{
				sample->resize(bufferLength, file->size());
			}
			std::pair<DatagramHeader, pSampleDatagram> *next = sample->next();
			file->getDatagram(next->second, &next->first);
			++pingInFile;
			read = false;
			break;
		}
		case nmeaDatagram:
		{

			if(text->full())
			{
				text->resize(100, 2048);
			}

			std::pair<DatagramHeader, pTextDatagram> *next = text->next();
			file->getDatagram(next->second, &next->first);


			break;
		}
		case filterDatagram:
		{

			if(filter->full())
			{

				filter->resize(5, file->size());
			}

			std::pair<DatagramHeader, pFilterDatagram> *next = filter->next();
			file->getDatagram(next->second, &next->first);

			break;
			}
		case mru0Datagram:
		case mru1Datagram:
		{
			if(mru->full())
			{
				mru->resize(100, file->size());
			}

			std::pair<DatagramHeader, MRUDatagram> *next = mru->next();
			file->getDatagram(&next->second, &next->first);

			break;
		}
		case noneDatagram:
		{
			break;
		}
		default:
		{
			break;
		}
		}
	}

	return ret = true;
}

bool ek80process::readAll(void)
{
	bool ret = true;
	while(file->eof() && ret)
	{
		ret = read();
	}
	return ret;
}

bool ek80process::convertAll(bool WithSv)
{
	bool ret = false;

	return ret;

}

bool ek80process::setBufferSize(int length)
{
	bool ret = false;

	this->length = length;

	return ret = true;

}

int ek80process::getLength(void)
{
	bool ret = false;

	return ret;

}

Dictionary<int, std::string> * ek80process::getChannels(void)
{

	return channels;

}

bool ek80process::filterChannels(Dictionary<int, std::string> filter)
{
	bool ret = false;
	if(channelFilter == NULL)
	{
		channelFilter == new Dictionary<int, std::string>();
	}

	for(int i = 0; i < filter.size(); i)
	{

	}
	return ret = true;
}

bool ek80process::haveSamples(int index)
{
	bool ret = false;
	return ret;
}

bool ek80process::getXml(int num, int offset , RawDType type, DgVector<pXmlDatagram> *data)
{
	bool ret = false;
	if(data== nullptr || data== NULL)
	{
		return ret;
	}

	return ret;

}

bool ek80process::getFilter(int num, int offset, short stage, DgVector<pFilterDatagram> *data)
{
	bool ret = false;
	if(data== nullptr || data== NULL)
	{
		return ret;
	}

	return ret;

}

bool ek80process::getMru(int num, int offset , DgVector<pMRUDatagram> *data)
{
	bool ret = false;
	if(data== nullptr || data== NULL)
	{
		return ret;
	}

	return ret;

}

bool ek80process::getSamples(int num, int offset, DgVector<pSampleDatagram> *data)
{
	bool ret = false;
	if(data== nullptr || data== NULL)
	{
		return ret;
	}




	return ret;

}

bool ek80process::getPower(int num, int offset, DgVector<pSampleDatagram> *data)
{
	bool ret = false;
	if(data== nullptr || data== NULL)
	{
		return ret;
	}

	return ret;

}

bool ek80process::getSv(int num, int offset,DgVector<pSampleDatagram> *data)
{
	bool ret = false;
	if(data== nullptr || data== NULL)
	{
		return ret;
	}

	return ret;

}

bool ek80process::getAngle(int num, int offset,DgVector<pSampleDatagram> *data)
{
	bool ret = false;
	if(data== nullptr || data== NULL)
	{
		return ret;
	}

	return ret;

}


bool ek80process::getPhase(int num, int offset, DgVector<pSampleDatagram> *data)
{
	bool ret = false;
	if(data == nullptr || data == NULL)
	{
		return ret;
	}

	return ret = true;
}
