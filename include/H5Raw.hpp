/*
 * h5tool.hpp
 *
 *      Author: mariuss
 */

#include <map>
#include <vector>
#include <string>
#include "H5Cpp.h"
#include "DgTypes.hpp"

#ifndef H5RAW_HPP_
#define H5RAW_HPP_

class H5Raw
{
private:

	struct Dataset{
		H5::CompType  *type = NULL;  /* Type of data */
		H5::DataSet   *data = NULL;   /* File size dataset */
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
	std::map<RawDType, struct  Dataset > *datasets;
	bool initData(auto *Datagram, RawDType type);
public:
	H5Raw(std::string *fname);
	H5Raw(std::string fname);
	bool open(std::string *fname= NULL);
	bool close(void);
	bool create(std::vector<size_t> *chunk_size);
	template <typename T> bool appendData(T *t, pDatagramHeader header=NULL);

};


#endif /* H5TOOL_HPP_ */
