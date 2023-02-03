/*
 * DatagramHeader.h
 *
 *      Author: marius
 */

#ifndef DATAGRAMHEADER_H_
#define DATAGRAMHEADER_H_
#include "DataTypes.h"

extern "C" {

typedef struct DatagramHeader
{
	uint32_t DatagramType;
	struct {
		uint32_t LowDateTime;
		uint32_t HighDateTime;
	} DateTime;
}DatagramHeader, *pDatagramHeader;

}





#endif /* DATAGRAMHEADER_H_ */
