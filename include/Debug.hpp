/*
'
* Distributed under the terms of the GNU General Public License v2
* Copyright 2023 Marius Norvoll Sletten
*
*/

/*
 * Debug.h
 *
 *  Created on: 8. feb. 2021
 *      Author: marius
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include<iostream>



#define DEBUG(STR) \
	std::cerr << "DEBUG: " << __FILE__  <<", " <<__func__ << ", (" <<__LINE__ << "): " << (STR) << std::endl







#endif /* DEBUG_HPP_ */
