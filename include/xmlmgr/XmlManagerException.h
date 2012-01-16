/**
*			@file XmlManagerException.h
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*
*			This file is owned by Nicolas Macherey and Cedric Roman and cannot be used by any ohter
*			third party without the written consent of one of the two authors
*/

#ifndef _XmlManagerException_h_
#define _XmlManagerException_h_

#include <xmlmgr/XmlManagerExports.h>

#include <string>

/**
*		@class XmlManagerException
*
*		@brief The XmlManagerException class is the class for managing error messages and
*							exceptions
*
*		@author Nicolas Macherey (nm@graymat.fr)
*		@date	31-Jan-2009
*		@version 0.0.1
*/

class XMLMGR_IMPORT XmlManagerException
{
public:

	 /** @brief Base Constructor.( STL string version )
    	*
    	*		@param msg Generated exception error message
    	*		@param file Generated exception source file name (__FILE__)
    	*		@param line Respective error line in the file
    	*/
    XmlManagerException(const std::string& msg, const std::string& file, int line);

     /** @brief Base Constructor ( char* version )
    	*
    	*		@param msg Generated exception error message
    	*		@param file Generated exception source file name (__FILE__)
    	*		@param line Respective error line in the file
    	*/
    XmlManagerException(const char* msg, const char* file, int line);

    /** Default destructor */
    virtual ~XmlManagerException();

    /** @brief Display exception error message.
    	*
    	*		@param safe If true, wxSafeShowMessage will be used to display the error,
    	*                 else a normal message box will be used.
    	*/
    void ShowErrorMessage(bool safe = true);

    /***************************************************************************************************************
    *	PUBLIC MEMBERS
    **************************************************************************************************************/
    std::string Message; ///< The exception's error message.
    std::string File; ///< The file where the exception was raised.
    int Line; ///< The line in the file where the exception was raised.
};

#define XmlMgrThrow(message) throw XmlManagerException(message, __FILE__, __LINE__)

#ifndef XML_MGR_DEBUG
#define XmlMgrAssert(expr)
#else

// In unix we use kill to terminate the application, that makes gdb
// keep it alive which makes debugging easier.
// (thanks go to an unknown author)
#ifdef WIN32
#define DIE() exit(1)
#else
#include <csignal>
#define DIE() kill(0, SIGTERM)
#endif

// non-fatal assertion
#define RheiaAssertNonFatal(expr) \
        if (!(expr)) \
        { \
				std::cerr << "Assertion failed in " <<  __PRETTY_FUNCTION__ << " in " << __FILE__ << " at line : " << __LINE__ << std::endl << "Error message is : " << #expr << std::endl; \
        }

// fatal assertion
#define RheiaAssert(expr) \
        if (!(expr)) \
        { \
            std::cerr << "Assertion failed in " <<  __PRETTY_FUNCTION__ << " in " << __FILE__ << " at line : " << __LINE__ << std::endl << "Error message is : " << #expr << std::endl; \
            DIE(); \
        }
#endif

#endif
