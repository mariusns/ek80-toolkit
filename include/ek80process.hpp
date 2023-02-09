/*
 * ek80process.hpp
 *
 *      Author: marius
 */

#ifndef EK80PROCESS_HPP_
#define EK80PROCESS_HPP_
#include <utility>
#include <vector>
#include "ek80file.hpp"
#include <type_traits>
#include "Debug.hpp"


/*
 * General purpose tool for ek80 raw files, read samples, calculate Sv/Angles
 *
 */

/*
 * 	XmlDatagram *out = new XmlDatagram;
 *	SampleDatagram *sample;
 *	FilterDatagram *filter;
 *	MRUDatagram *mru;
 */

template<typename T>
	using DgVector = std::vector<std::pair<DatagramHeader, T>>;

template<typename K, typename T>
	using Dictionary = std::vector<std::pair<K, T>>;

template<typename T>
class DgData {
private:
	size_t idx = 0;
	size_t typesize = 0;
	DgVector<T> * data = NULL;
public:


	std::pair<DatagramHeader, T>*  next(void)
	{

		if(data ==  NULL)
		{
			return NULL;
		}
		if(idx >= data->size())
		{
			return NULL;
		}
		return &data->at(idx++);

	}

	bool full(void)
	{
		if(data == nullptr)
			return true;
		return (bool)(idx >= data->size());
	}


	size_t size()
	{
		if(data == NULL)
		{
			return 0;
		}
		return data->size();
	}

	bool resize(size_t inc, size_t s)
	{
		bool ret = false;
		if(data == NULL)
		{
			data = new DgVector<T>;
			typesize = sizeof(T);
		}
		data->resize(data->size() + inc);

		if(std::is_pointer<T>::value)
		{
			typesize = s;
			for(size_t i = idx; i < data->size(); i++ )
			{
				//data->at(i).second = (T) new char[s];
			}
		}
		else{
			return ret;
		}
		return ret = true;
	}

	bool resize(size_t inc)
	{

		bool ret = false;
		if(data == NULL)
		{
			data = new DgVector<T>;
			typesize = sizeof(T);
		}
		data->resize(data->size() + inc);
		return ret = true;
	}


	bool close(void)
	{
		if(std::is_pointer<T>::value)
		{
			for(int i = idx; i < data->size(); i++ )
			{
				delete[] data->at(i).first;
				delete[] data->at(i).second;
			}
		}
		delete data;
		data = NULL;
		idx = 0;

		return true;
	}
};





class ek80process{
private:
	struct Xml_s
	{
		DgVector<XmlDatagram> 	 						*Configuration = NULL;
		DgVector<XmlDatagram> 	 						*InitialParameters = NULL;
		DgVector<XmlDatagram> 	 						*Filter = NULL;
		DgVector<XmlDatagram> 	 						*Environment = NULL;
		DgVector<XmlDatagram>						 	*Parameter = NULL;
		/* Iterate over */
		int idx = 0;
		std::pair<DatagramHeader, XmlDatagram> *next(int i = -1);
	};

	ek80file 					*file = NULL;
	DgData<pSampleDatagram> 	*sample = NULL;
	struct Xml_s 				*xml 	= NULL;
	DgData<pFilterDatagram> 	*filter = NULL;
	DgData<MRUDatagram> 		*mru = NULL;
	DgData<pTextDatagram>		*text = NULL;
	Dictionary<int, std::string> *channels = NULL;

	Dictionary<int, std::string>  *channelFilter = NULL;
	size_t idx = 0;
	size_t bufferLength = 100;
	size_t length = 0;
	size_t pingInFile = 0;
	bool init(int length = 100);

public:
	ek80process(std::string *fname = NULL);
	ek80process(std::string &fname);

	bool open(std::string *fname = NULL);
	bool close(void);
	bool clear(void);
	bool reset(void);
	bool read(void);
	bool readAll(void);

	bool convertAll(bool WithSv);
	bool setBufferSize(int length = -1);
	int getLength(void);

	Dictionary<int, std::string> *getChannels(void);
	bool filterChannels(Dictionary<int, std::string> filter);
	bool haveSamples(int index = -1);
	bool getXml(int num, int offset , RawDType type, DgVector<pXmlDatagram> *data);
	bool getFilter(int num, int offset, short stage, DgVector<pFilterDatagram> *data);
	bool getMru(int num, int offset, DgVector<pMRUDatagram> *data);
	bool getNmea(int num, int offset, DgVector<pTextDatagram> *data);
	/* Different Data formats for samples, all data are  quadrature data */

	/* Complex Samples, no processing */
	bool getSamples(int num, int offset, DgVector<pSampleDatagram> *data);

	/* Power */
	bool getPower(int num, int offset, DgVector<pSampleDatagram> *data);
	/* Sv */
	bool getSv(int num, int offset, DgVector<pSampleDatagram> *data);
	/* Angle */
	bool getAngle(int num, int offset, DgVector<pSampleDatagram> *data);
	/* Phase */
	bool getPhase(int num, int offset, DgVector<pSampleDatagram> *data);
};


#endif /* EK80PROCESS_HPP_ */

