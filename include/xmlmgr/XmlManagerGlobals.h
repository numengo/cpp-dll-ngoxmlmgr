/**
*			@file XmlManagerGlobals.h
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*
*			This file is owned by Nicolas Macherey and Cedric Roman and cannot be used by any ohter
*			third party without the written consent of one of the two authors
*/

#ifndef _XmlManagerGlobals_h_
#define _XmlManagerGlobals_h_

#include <iostream>
#include <vector>
#include <map>

#define NaN(x) (x != x)

/*******************************************************************************************************************
*	Define basic container for xml Manager
*******************************************************************************************************************/

/**
*			@struct xmNumengoReference
*
*			@brief This structure is used for reading and writing reference nodes in an xml file
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*/
struct xmNumengoReference
{
		std::string name;						/*!< refenrence's name string */
		int year;											/*!< refenrence's date year int */
		int month;										/*!< refenrence's date month int */
		int day;											/*!< refenrence's date day int */
		std::string source;					/*!< refenrence's source string */
		std::string comment;			/*!< refenrence's comment string */
};

/**
*			@struct xmLibrarySubSystem
*
*			@brief This structure is used for reading and writing dynamic library subsystems nodes to/from an xml file
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*/
struct xmLibrarySubSytem
{
		std::string system_type;					/*!< system type node */
		std::string library_name;					/*!< library name node */
};

/** Define the basic vector for libraries subsystems */
typedef std::vector< xmLibrarySubSytem > xmArrayLibrarySubSystems;

/**
*			@struct xmDynamicLibrary
*
*			@brief This structure is used for reading and writing dynamic library nodes to/from an xml file
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*/
struct xmDynamicLibrary
{
		std::string constructor_node;											/*!< Dynamic library constructor in the library */
		xmArrayLibrarySubSystems SubSystems;			/*!< Dynamic library subsystems nodes */
};

/**
*			@struct xmValueUnit
*
*			@brief This structure is used for reading and writing value/units nodes to/from an xml file
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*/
struct xmValueUnit
{
		double value;											/*!< value numeric double */
		std::string units;										/*!< value units */
};

/**
*			@struct xmTemperaturePressureDensity
*
*			@brief This structure is used for reading and writing pressure/temperature/density nodes to/from an xml file
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*/
struct xmTemperaturePressureDensity
{
		xmValueUnit temperature;											/*!< temperature node in group */
		xmValueUnit pressure;													/*!< pressure node in group */
		xmValueUnit density;														/*!< density node in group */
};

/**
*			@struct xmElement
*
*			@brief This structure is used for reading and writing elements nodes to/from an xml file
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*/
struct xmElement
{
		std::string name;
		int num_of_atoms;
};

/** define an element table */
typedef std::vector< xmElement > xmArrayElement;

#endif
