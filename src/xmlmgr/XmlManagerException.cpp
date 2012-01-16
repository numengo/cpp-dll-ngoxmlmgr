/**
*			@file XmlManagerException.cpp
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*
*			This file is owned by Nicolas Macherey and Cedric Roman and cannot be used by any ohter
*			third party without the written consent of one of the two authors
*/

/*******************************************************************************
   LICENSE
*******************************************************************************
 Copyright (C) 2009 Numengo (admin@numengo.com)
 
 This document is released under the terms of the numenGo EULA.  You should have received a 
 copy of the numenGo EULA along with this file; see  the file LICENSE.TXT. If not, write at 
 admin@numengo.com or at NUMENGO, 15 boulevard Vivier Merle, 69003 LYON - FRANCE 
 You are not allowed to use, copy, modify or distribute this file unless you  conform to numenGo
 EULA license.
*/



#include <xmlmgr/XmlManagerException.h>

#include <iostream>
#include <sstream>

XmlManagerException::XmlManagerException(const std::string& msg, const std::string& file, int line)
  : Message(msg),
    File(file),
    Line(line)
{}

XmlManagerException::XmlManagerException(const char* msg, const char* file, int line)
  : Message(msg),
    File(file),
    Line(line)
{}

XmlManagerException::~XmlManagerException()
{}


void XmlManagerException::ShowErrorMessage(bool safe)
{
   std::string gccvers;
#ifdef __GNUC__
		std::stringstream ss;
		ss << "gcc " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__;
		ss >> gccvers;
#endif

    std::string title = "Exception";
    std::stringstream errss;

    errss << "An exception has been raised!\n\n"
                 "The application encountered an error at " <<File.c_str() <<", on line "<< Line << ".\n"
                 "The error message is:\n\n" << Message.c_str() << "\n\n"
                 "XmlVersion Version revision " << 1123 << "(" << gccvers.c_str() <<", "
                 "build: "<< __DATE__ << " " << __TIME__ << ")" ;


	std::cerr <<  title << " : " << errss << std::endl;
}
