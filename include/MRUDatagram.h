/*
 * MRUDatagram.h
 *
 *      Author: mariuss
 */

#ifndef MRUDATAGRAM_H_
#define MRUDATAGRAM_H_
#include <DatagramHeader.h>

extern "C"
{

typedef struct MRUDatagram
{
	//DatagramHeader DgHeader;
	float Heave;
	float Roll;
	float Pitch;
	float Heading;
}MRUDatagram,*pMRUDatagram;

}

#endif /* MRUDATAGRAM_H_ */
