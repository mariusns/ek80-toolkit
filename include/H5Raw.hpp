/*
*   Copyright 2023 Marius Norvoll Sletten
*   Distributed under the terms of the GNU General Public License v2
*
*/

#include <map>
#include <vector>
#include <string>
#include "H5Cpp.h"
#include "DgTypes.hpp"

#ifndef H5RAW_HPP_
#define H5RAW_HPP_


const struct RawLayout
{
	const char SampleData[] = "/SampleData";
	const char Xml[] = "/Xml";
	const char Filter[] = "/Filter";
	const char MRU[] = "/MRU";
	const char Text[] = "/Text";
	const char Depth[] = "/SampleData/Depth"

};




class H5Raw
{
private:

	struct Dataset{
		H5::CompType  *type = NULL;  /* Type of data */
		H5::DataSet   *data = NULL;   /* File dataset */
		H5::DataSpace *fspace = NULL; /* File space */
		H5::DataSpace *bspace = NULL;
		H5::DSetCreatPropList  *prop = NULL;
		H5::Group *group = NULL;
		hsize_t bdim = 0;
		hsize_t *bsize = NULL;
		char *buffer = NULL;
		template<typename T>Dataset(H5::Group &loc, std::string &name ,std::vector<size_t> &dims);
		size_t size();
		bool close(void);
	};

	std::string fileName;
	H5::H5File  *file = NULL;
	std::map<RawDType, struct  Dataset * > *datasets;
	template<typename T> bool initData(T *Datagram, RawDType type);
public:
	H5Raw(std::string *fname);
	H5Raw(std::string fname);
	bool open(std::string *fname= NULL);
	bool close(void);
	bool create(std::vector<size_t> *chunk_size);
	template <typename T> bool appendData(T *t, pDatagramHeader header=NULL);

};


#endif /* H5TOOL_HPP_ */
