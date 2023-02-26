
/**
*			@file XmlManagerExports.h
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*
*			This file is owned by Nicolas Macherey and Cedric Roman and cannot be used by any ohter
*			third party without the written consent of one of the two authors
*/

#ifndef _XmlManagerExports_h_
#define _XmlManagerExports_h_

#ifdef XMLMGR_USE_DYNAMIC
#ifdef  _MSC_VER
	#ifndef XMLMGR_IMPORT
		#ifdef XMLMGR_MAKING_DLL
			#define XMLMGR_IMPORT __declspec( dllexport )
		#else
			#define XMLMGR_IMPORT __declspec( dllimport )
		#endif
	#endif
	#pragma warning( disable: 4251 )
	#pragma warning( disable: 4275 )
	#pragma warning( disable: 4996 )
	#pragma warning( disable: 4661 )
#else
	#ifndef XMLMGR_IMPORT
		#ifdef XMLMGR_MAKING_DLL
			#define XMLMGR_IMPORT
		#else
			#define XMLMGR_IMPORT
		#endif
	#endif
#endif
#else
	#define XMLMGR_IMPORT
#endif


#endif

