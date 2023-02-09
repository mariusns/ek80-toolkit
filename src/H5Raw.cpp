/*
 * h5tool.cpp
 *
 *      Author: mariuss
 */

#include "H5Raw.hpp"
#include <filesystem>


const static struct RawLayout Layout;


static H5::CompType CreateDatagramHeader(void)
{
	H5::CompType  datetime = H5::CompType();
	datetime.insertMember("LowDateTime", 0 , H5::PredType::STD_U32LE);
	datetime.insertMember("HighDateTime", 4 , H5::PredType::STD_U32LE);
	H5::CompType  header = H5::CompType();
	header.insertMember("DatagramType", HOFFSET(DatagramHeader, DatagramType), H5::PredType::STD_U32LE);
	header.insertMember("DateTime", HOFFSET(DatagramHeader, DateTime), datetime);
	datetime.close();

	return header;

}

/* Private function */

template<>
H5Raw::Dataset::Dataset<XmlDatagram>(H5::Group &loc, std::string &name, std::vector<size_t> &dims)
{
	/* Resolve how xml are stored later on when we are using the values */
	hsize_t dim[2];
	hsize_t dims_loc[2] = {128, 0};
	hsize_t dims_max[2] = {H5S_UNLIMITED,H5S_UNLIMITED};
	H5::CompType  header = CreateDatagramHeader();
	H5::ArrayType str = H5::ArrayType(H5::PredType::C_S1, 1, dims_loc);

	type = new H5::CompType();
	type->insertMember("DgHeader", 0, header);
	type->insertMember("type", header.getSize(), str);

	if(dims.size() >= 2)
	{
		dims_loc[0] = dims.at(0); /* Number of lines, varable */
		dims_loc[1] = dims.at(1); /* Number of Columns, = 2, tag and value */
	}

	H5::ArrayType xml = H5::ArrayType(H5::PredType::C_S1, 1, dims_loc);
	type->insertMember("datagram", header.getSize() + str.getSize(), xml);

	str.close();
	xml.close();
	header.close();

	dims_loc[0] = 50;
	prop = new H5::DSetCreatPropList();
	prop->setChunk(1, dims_loc);
	prop->setDeflate(6);


	fspace = new H5::DataSpace(1, dims_loc, dims_max);


	data = new H5::DataSet(loc.createDataSet((const char *)name.c_str(),
							(const H5::CompType)*type,
							(const H5::DataSpace)*fspace,
							(const H5::DSetCreatPropList) *prop));
}


template<>
H5Raw::Dataset::Dataset<DepthDatagram>(H5::Group &loc, std::string &name, std::vector<size_t> &dims)
{
	/* Resolve how xml are stored later on when we are using the values */
	hsize_t dims_loc[2] = {128, 0};
	hsize_t dims_max[2] ={H5S_UNLIMITED,H5S_UNLIMITED};
	H5::CompType  header = CreateDatagramHeader();

	if(dims.size() >= 1)
	{
		dims_loc[0] = dims.at(0);
	}
	H5::ArrayType Channel = H5::ArrayType(H5::PredType::IEEE_F64LE, 1, dims_loc );

	type = new H5::CompType();
	type->insertMember("DgHeader", 0, header);
	type->insertMember("lTransducerCount", header.getSize(), H5::PredType::STD_U32LE);
	type->insertMember("Channel", header.getSize(), Channel);

	dims_loc[0] = 50;
	prop = new H5::DSetCreatPropList();
	prop->setChunk(1, dims_loc);
	prop->setDeflate(6);
	fspace = new H5::DataSpace(1, dims_loc, dims_max);

	data = new H5::DataSet(loc.createDataSet((const char *)name.c_str(),
							(const H5::CompType)*type,
							(const H5::DataSpace)*fspace,
							(const H5::DSetCreatPropList) *prop));

	Channel.close();
	header.close();
}

template<>
H5Raw::Dataset::Dataset<FilterDatagram>(H5::Group &loc, std::string &name, std::vector<size_t> &dims)
{
	/* Resolve how xml are stored later on when we are using the values */
	hsize_t dims_loc[2] = {128, 0};
	hsize_t dims_max[2] ={H5S_UNLIMITED,H5S_UNLIMITED};
	H5::CompType  header = CreateDatagramHeader();


	H5::ArrayType ChannelId_t = H5::ArrayType(H5::PredType::STD_U8LE, 1, dims_loc );
	if(dims.size() >= 1)
	{
		dims_loc[0] = dims.at(0);
	}

	H5::ArrayType Coeff_t = H5::ArrayType(H5::PredType::IEEE_F32LE, 1, dims_loc);
	type = new H5::CompType();
	type->insertMember("DgHeader", 0, header);
	type->insertMember("Stage", header.getSize() + HOFFSET(FilterDatagram, Stage), H5::PredType::STD_U32LE);
	type->insertMember("Spare", header.getSize()+ HOFFSET(FilterDatagram, Spare), H5::PredType::STD_U16LE);
	type->insertMember("FilterType", header.getSize()+ HOFFSET(FilterDatagram, FilterType), H5::PredType::STD_U8LE);
	type->insertMember("ChanneId", header.getSize()+ HOFFSET(FilterDatagram, ChannelId), ChannelId_t);
	type->insertMember("NoOfCoefficients", header.getSize() + HOFFSET(FilterDatagram, NoOfCoefficients), H5::PredType::STD_I16LE);
	type->insertMember("DecimationFactor", header.getSize()+ HOFFSET(FilterDatagram, DecimationFactor), H5::PredType::STD_I16LE);


	type->insertMember("Coefficients", header.getSize()+ HOFFSET(FilterDatagram, Coefficients), Coeff_t);

	dims_loc[0] = 50;
	prop = new H5::DSetCreatPropList();
	prop->setChunk(1, dims_loc);
	prop->setDeflate(6);
	fspace = new H5::DataSpace(1, dims_loc, dims_max);

	data = new H5::DataSet(loc.createDataSet((const char *)name.c_str(),
							(const H5::CompType)*type,
							(const H5::DataSpace)*fspace,
							(const H5::DSetCreatPropList) *prop));


	header.close();
	ChannelId_t.close();
	Coeff_t.close();


}

template<>
H5Raw::Dataset::Dataset<IndexFileDatagram>(H5::Group &loc, std::string &name, std::vector<size_t> &dims)
{

}

template<>
H5Raw::Dataset::Dataset<MRUDatagram>(H5::Group &loc, std::string &name, std::vector<size_t> &dims)
{
	/* Resolve how xml are stored later on when we are using the values */
	hsize_t dims_loc[2] = {128, 0};
	hsize_t dims_max[2] ={H5S_UNLIMITED,H5S_UNLIMITED};
	H5::CompType  header = CreateDatagramHeader();

	type = new H5::CompType();
	type->insertMember("DgHeader", 0, header);
	type->insertMember("Heave", header.getSize() + HOFFSET(MRUDatagram, Heave), H5::PredType::IEEE_F32LE);
	type->insertMember("Roll", header.getSize()+ HOFFSET(MRUDatagram, Roll), H5::PredType::IEEE_F32LE);
	type->insertMember("Pitch", header.getSize()+ HOFFSET(MRUDatagram, Pitch), H5::PredType::IEEE_F32LE);
	type->insertMember("Heading", header.getSize()+ HOFFSET(MRUDatagram, Heading), H5::PredType::IEEE_F32LE);

	dims_loc[0] = 50;
	prop = new H5::DSetCreatPropList();
	prop->setChunk(1, dims_loc);
	prop->setDeflate(6);

	fspace = new H5::DataSpace(1, dims_loc, dims_max);

	data = new H5::DataSet(loc.createDataSet((const char *)name.c_str(),
							(const H5::CompType)*type,
							(const H5::DataSpace)*fspace,
							(const H5::DSetCreatPropList) *prop));

	header.close();

}

template<>
H5Raw::Dataset::Dataset<SampleDatagram>(H5::Group &loc, std::string &name, std::vector<size_t> &dims)
{

	bdim = dims.size();
	bsize = new hsize_t[bdim];
	buffer = NULL;

	hsize_t dims_max[3] ={H5S_UNLIMITED, H5S_UNLIMITED, H5S_UNLIMITED};
	H5::CompType  header = CreateDatagramHeader();

	H5::ArrayType ChannelID_t = H5::ArrayType(H5::PredType::STD_U8LE, 1, bsize);


	for(int i = 0; i < bdim; i++)
		bsize[i] = dims[i];

	H5::ArrayType Samples_t = H5::ArrayType(H5::PredType::IEEE_F32LE, bdim, bsize);
	type = new H5::CompType();
	type->insertMember("DgHeader", 0, header);
	type->insertMember("ChannelID", header.getSize() + HOFFSET(SampleDatagram, ChannelID), ChannelID_t);
	type->insertMember("Datatype", header.getSize() + HOFFSET(SampleDatagram, Datatype), H5::PredType::STD_I16LE);
	type->insertMember("Spare", header.getSize()+ HOFFSET(SampleDatagram, Spare), H5::PredType::STD_U16LE);
	type->insertMember("Offset", header.getSize()+ HOFFSET(SampleDatagram, Offset), H5::PredType::STD_U32LE);
	type->insertMember("Count", header.getSize()+ HOFFSET(SampleDatagram, Count), H5::PredType::STD_U32LE);
	type->insertMember("Samples", header.getSize()+ HOFFSET(SampleDatagram, Samples), Samples_t);

	bsize[bdim -1]  = 50
	prop = new H5::DSetCreatPropList();
	prop->setChunk(bdim, bsize);
	prop->setDeflate(6);

	fspace = new H5::DataSpace(1, bsize, dims_max);

	data = new H5::DataSet(loc.createDataSet((const char *)name.c_str(),
							(const H5::CompType)*type,
							(const H5::DataSpace)*fspace,
							(const H5::DSetCreatPropList) *prop));
	header.close();
	ChannelID_t.close();
	Samples_t.close();

}

template<>
H5Raw::Dataset::Dataset<TextDatagram>(H5::Group &loc, std::string &name, std::vector<size_t> &dims)
{
	/* Resolve how xml are stored later on when we are using the values */
	hsize_t dims_loc[2] = {128, 0};
	hsize_t dims_max[2] ={H5S_UNLIMITED,H5S_UNLIMITED};
	H5::CompType  header = CreateDatagramHeader();

	if(dims.size() > 0)
	{
		dims_loc[0] = dims.at(0);
	}
	H5::ArrayType Text_t = H5::ArrayType(H5::PredType::C_S1, 1, dims_loc);
	type = new H5::CompType();
	type->insertMember("DgHeader", 0, header);
	type->insertMember("Text", header.getSize() + HOFFSET(TextDatagram, Text), Text_t);

	dims_loc[0] = 50;
	prop = new H5::DSetCreatPropList();
	prop->setChunk(1, dims_loc);
	prop->setDeflate(6);

	fspace = new H5::DataSpace(1, dims_loc, dims_max);

	data = new H5::DataSet(loc.createDataSet((const char *)name.c_str(),
							(const H5::CompType)*type,
							(const H5::DataSpace)*fspace,
							(const H5::DSetCreatPropList) *prop));

	header.close();
	Text_t.close();

}


size_t H5Raw::Dataset::size()
{
	if(type == NULL)
		return 0;
	return type->getSize();
}



bool H5Raw::Dataset::close(void)
{
	bool ret = false;

	if(group != NULL)
	{
		group->close();
		group = NULL;
	}

	if(data != NULL)
	{
		data->close();
		data = NULL;
	}

	if (type != NULL)
	{
		type->close();
		type = NULL;
	}

	if(fspace != NULL)
	{
		fspace->close();
		fspace = NULL;
	}

	if(bspace != NULL)
	{
		bspace->close();
		bspace = NULL;

	}

	if(prop != NULL)
	{
		prop->close();
		prop = NULL;

	}

	if(buffer != NULL)
	{
		delete[] buffer;
		buffer = NULL;
		delete[] bsize;
		bsize = NULL;
		bdim = 0;
	}

	return ret = true;

}

template<typename T> bool H5Raw::initData(T *chunk_size, RawDType type)
{
	bool ret = false;

	if(datasets == NULL)
	{
		datasets = new std::map<RawDType, struct Dataset>();
	}

	switch (type)
	{
		case RawDType::Text:break;
		case RawDType::Xml0: break;
		case RawDType::Depth:break;
		case RawDType::Xml0Configuration: break;
		case RawDType::Xml0Init: break;
		case RawDType::Xml0Filt1: break;
		case RawDType::Xml0Filt2: break;
		case RawDType::Filter: break;
		case RawDType::Xml0Environment: break;
		case RawDType::Xml0Parameter: break;
		case RawDType::Mru1:
		case RawDType::Mru2:
		case RawDType::Mru:break;
		case RawDType::Sample:

			datasets->insert(std::pair<RawDType, struct Dataset *>(RawDType::Sample, new struct Dataset));
			struct Dataset * samples = datasets->at(RawDType::Sample);

			samples->group = H5::Group(file->createGroup(Layout.SampleData));
			samples->Dataset(samples->group, "Sample", samples->bsize);
			break;
		case RawDType::Sv:break;
		case RawDType::Pw:break;
		case RawDType::Angle:break;
		case RawDType::Phase:break;
		default: break;

	}


	return ret;
}



/* Public function  implementation */





H5Raw::H5Raw(std::string *fname)
{
	fileName.copy((char *)fname->c_str(), fname->length(),0);
	file = NULL;
	datasets  = NULL;

}

H5Raw::H5Raw(std::string fname)
{
	fileName.copy((char *)fname.c_str(), fname.length(),0);
	file = NULL;
	datasets  = NULL;

}


bool H5Raw::open(std::string *fname)
{
	bool ret = false;

	return false;
}


bool H5Raw::close(void)
{

	if(file != NULL && file->isValid(file->getId()))
	{
		if(datasets != NULL)
		{
			for(auto &s: *datasets)
			{
				s.second.close();
			}
			datasets->clear();
			delete datasets;
			datasets = NULL;
			file->close();
			file = NULL;
		}
	}

	return false;
}


bool H5Raw::create(std::vector<size_t>  *chunk_size)
{
	bool ret = false;
	std::filesystem::path *p = new std::filesystem::path(fileName);


	if (std::filesystem::exists(p->parent_path()))
	{
		if(file != NULL && file->getId() <= 0 )
		{
				file->close();
				file = NULL;
				file = new H5::H5File(fileName, H5F_ACC_TRUNC);
		}else if(file == NULL)
		{
			file = new H5::H5File(fileName, H5F_ACC_RDWR);
			if(file->getId() <= 0)
			{
				file->close();
				file = NULL;
				file = new H5::H5File(fileName, H5F_ACC_TRUNC);
				if(file->getId() <= 0)
				{
					file->close();
					file = NULL;
					delete p;
					return ret;
				}
			}
		}

		delete p;

	}else{
		delete p;
		return ret;
	}

	if(file!= NULL  && file->getId() <= 0)
	{
		for(int i = (int)RawDType::Text; i < (int)RawDType::NumTypes; ++i)
		{
			ret = initData<std::vector<size_t>>(chunk_size, static_cast<RawDType>(i));
			if(!ret) break;
		}
	}

	return ret;
}

template<typename T>
bool H5Raw::appendData(T * data, pDatagramHeader header )
{
	bool ret = false;

	return ret = true;
}
