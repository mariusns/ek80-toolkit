/*
'
* Distributed under the terms of the GNU General Public License v2
* Copyright 2023 Marius Norvoll Sletten
*
*/

/*
 * IndexFileDatagram.h
 *
 *      Author: mariuss
 */

#ifndef INDEXFILEDATAGRAM_H_
#define INDEXFILEDATAGRAM_H_
#include <DatagramHeader.h>

extern "C" {

typedef struct IndexFileDatagram
{
	//DatagramHeader DgHeader;
	ULONG PingNumber;
	double VesselDistance;
	double Latitude;
	double Longitude;
	DWORD  FileOffset;

}IndexFileDatagram, *pIndexFileDatagram;

}

#endif /* INDEXFILEDATAGRAM_H_ */
