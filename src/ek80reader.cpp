#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "ek80file.hpp"
#include <boost/cstdfloat.hpp>
#include <filesystem>



using namespace std;



void print(SampleDatagram * data)
{

	if(data == NULL)
		return;

	std::cout << "SampleDatagram: " << std::endl;
	std::cout << "\t- " << "ChannelID: " << data->ChannelID << std::endl;
	std::cout << "\t- " << "Datatype: "  << (short)data->Datatype << ", Power: " << (short) (data->Datatype & SampleTypes::Power);
	std::cout << ", Angle: " << (short) ((data->Datatype & Angle) >> 1);
	std::cout << ", CFloat16: " << (short)((data->Datatype & SampleTypes::ComplexFloat16) >> 2);
	std::cout << ", CFloat32: " << (short)((data->Datatype & SampleTypes::ComplexFloat32) >> 3);
	std::cout << ", NumSamples: " <<  (short)((data->Datatype & SampleTypes::NumSamples) >> 8) << std::endl;
	std::cout << "\t- " << "Offset: " << data->Offset << std::endl;
	std::cout << "\t- " << "Count: " << data->Count << std::endl;


}

int main(int argc, char **argv)
{
		filesystem::path *p;
	    for (int i = 1; i < argc; ++i)
	    {
	    	p = new filesystem::path(argv[i]);
	    	if(filesystem::exists(*p))
	    	{
	    		break;
	    	}
	    }

	ek80file file(p->c_str());
	file.open();

	delete p;
	DgTypes type = noneDatagram;
	std::vector<char > *out = new std::vector<char>();
	XmlDatagram xml;
	SampleDatagram *sample;
	FilterDatagram *filter;
	MRUDatagram *mru;
	TextDatagram *nmea;

	int num_samples = 0;
	for(int i = 0; !file.eof() && file.is_open(); i++)
	{
		type = (DgTypes)file.read();
		if(out == NULL)
		{
			out->resize(file.size());
		} else if( sizeof(out) < file.size())
		{
			out->resize(file.size());
		}
		switch(type)
		{
			case xmlDatagram:
			{
				std::cout << "Processing:  " << "xmlDatagram: " << file.size() << ", " << sizeof(out) << std::endl;

				file.getDatagram(&xml);

				std::cout << "XML output: " << xml.type << std::endl;

				break;
			}
			case textDatagram:
			{
				/*
				TextDatagram *text;
				text = new char[file.size()];
				file.getDatagram(text);
				*/
				std::cout << "Processing:  " << "textDatagram" << std::endl;

				break;
			}
			case sampleDatagram:
			{
				std::cout << "Processing:  " << "sampleDatagram" << std::endl;
				sample = reinterpret_cast<SampleDatagram *>(out->data());
				file.getDatagram(sample);

				print(sample);

				num_samples++;
				if(num_samples >= 5)
					file.close();
				break;
			}
			case filterDatagram:
			{
				std::cout << "Processing:  " << "filterDatagram " << std::endl;
				filter = reinterpret_cast<FilterDatagram *>(out->data());
				file.getDatagram(filter);


				break;
			}
			case mru0Datagram:
			case mru1Datagram:
			{
				std::cout << "Processing:  " << "mru1Datagram" << std::endl;
				mru = reinterpret_cast<MRUDatagram *>(out->data());
				file.getDatagram(mru);

				break;
			}
			case nmeaDatagram:
			{

				std::cout << "Processing:  " << "nmeaDatagram" << std::endl;
				nmea = reinterpret_cast<TextDatagram *>(out->data());
				file.getDatagram(nmea);
				break;
			}
			case noneDatagram:
				std::cout << "Processing:  " << "noneDatagram" << std::endl;
				break;
			default:
				std::cout << "Processing:  default " << type << std::endl;
				break;

		}
	}

	delete out;
	file.close();



	std::cout  << "End reading: " <<  true << std::endl;

	return 0;

}
