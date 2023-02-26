/**
*			@file XmlManagerBase.cpp
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

#include <xmlmgr/XmlManagerBase.h>
#include "ngoerr/NgoError.h"

#include <libxml/xmlreader.h>
#include <libxml/xpath.h>

#include <sstream>
#include <iostream>

_xmlDoc * XmlMgrParseFile(const char * filename)
{ return xmlParseFile(filename); };

int XmlMgrSaveFormatFileEnc(const char * filename, _xmlDoc * cur, const char * encoding, int format)
{ return xmlSaveFormatFileEnc(filename,cur,encoding,format); }

_xmlDoc * XmlMgrNewDoc(const char * version)
{ return xmlNewDoc((const xmlChar *)version); };

void XmlMgrFreeDoc(_xmlDoc * cur)
{ return xmlFreeDoc(cur); };

_xmlNode * XmlMgrNewDocNode(_xmlDoc * doc, _xmlNs * ns, const char * name, const char * content)
{ return xmlNewDocNode(doc,ns,(const xmlChar *)name,(const xmlChar *)content); }

_xmlNode * XmlMgrDocSetRootElement(_xmlDoc * doc, _xmlNode * root)
{ return xmlDocSetRootElement(doc,root); };

_xmlNode * XmlMgrDocGetRootElement(_xmlDoc * doc)
{ return xmlDocGetRootElement(doc); };

void XmlMgrUnlinkNode(_xmlNode * cur)
{ return xmlUnlinkNode(cur); };

void XmlMgrFreeNode(_xmlNode * cur)
{ return xmlFreeNode(cur); };



template <> XmlManagerBase * NgoSingleton<XmlManagerBase>::instance_ = 0L;

XmlManagerBase::~XmlManagerBase()
{
   // need to clean up xml parser
   xmlCleanupParser();
}

inline void XmlManagerBase::Collapse(std::string& str) const
{
    const char *src = str.c_str();
    char *dst = (char*) src;
    char c;
    size_t len = 0;

    while ((c = *src))
    {
        ++src;

        *dst = c;
        ++dst;
        ++len;

        if (c == '/')
            while (*src == '/')
                ++src;
    }
    str = str.substr( 0 , len );
};

xmlNode* XmlManagerBase::AssertPath( std::string& path,
                                     xmlNode* pathNode, bool create_unexisting )
{
    if ( pathNode == NULL )
        throw NgoErrorInvalidArgument(2,"Error, you are trying to write xml elements in an inexistant xml node","XmlManagerBase::AssertPath");

    if ( path.empty() || path == "/" )
        return pathNode;

    /** @note A path can be viewed as a directory, a key is an element contained in a path
    *						key and path are unique.
    */
    Collapse(path);

    std::string illegal(" -:.\"\'$&()[]<>+#");
    size_t i = 0;

    while ((i = path.find_first_of(illegal, i)) != std::string::npos)
        path[i] = '_';

    xmlNode *localPath = pathNode;

    if (path[0] == '/') // absolute path
    {
        path = path.substr( 1 );
    }

    std::string sub;
    std::vector<std::string> SubPaths;

    /* Extract Sub Paths from path */

    while ( path.find('/') != std::string::npos)
    {
        unsigned int index = path.find('/');
        sub = path.substr(0,index);

        if ( index != path.size() - 1 )
            path = path.substr(index+1);
        else
            path = "";

        SubPaths.push_back( sub );
    }

    /* Now check all paths */
    for ( unsigned int i = 0; i < SubPaths.size() ; i++ )
    {
        sub = SubPaths[i];
        xmlNode* subNode;
        subNode = localPath->children;

        /* locate subnode */
        while ( subNode != NULL )
        {
            std::string nodeName = (const char*) subNode->name ;
            if ( nodeName == sub )
            {
                break;
            }
            subNode = subNode->next;
        }

        if ( subNode == NULL )
        {
            if ( create_unexisting )
            {
                subNode = xmlNewNode( NULL , (const xmlChar*) sub.c_str() );
                xmlAddChild( localPath , subNode );
            }
            else
            {
                return NULL;
            }
        }

        localPath = subNode;
    }

    return localPath;
}

void XmlManagerBase::Clear(xmlNode *rootNode)
{
    xmlNode *sub = rootNode->children;
    xmlNode *child;

    while ( sub != NULL )
    {
        child = sub;
        sub = sub->next;
        xmlReplaceNode( child , NULL );
    }
}

void XmlManagerBase::Delete(xmlDoc* doc)
{
//    CfgMgrBldr * bld = CfgMgrBldr::get();
//    wxString ns(cbC2U(root->Value()));
//
//    root->Clear();
//    doc->RootElement()->RemoveChild(root);
//
//    wxCriticalSectionLocker(bld->cs);
//    NamespaceMap::iterator it = bld->namespaces.find(ns);
//    if(it != bld->namespaces.end())
//        bld->namespaces.erase(it);
//
//    delete this;
}

xmlNode* XmlManagerBase::GetUniqElement(xmlNode* p, const std::string& q, bool create_unexisting)
{
    if ( p == NULL )
        throw NgoErrorInvalidArgument(1,"trying to get a child from an unexisting node","XmlManagerBase::GetUniqElement");

    if ( q.empty() || q == "/" )
        return p;

    xmlNode* r = p->children;

    while ( r != NULL )
    {
        std::string nodeName = (const char*) r->name ;
        if ( nodeName == q )
            return r;

        r = r->next;
    }
    if ( create_unexisting )
    {
        r = xmlNewNode( NULL , (const xmlChar*) q.c_str() );
        return  xmlAddChild( p , r );
    }
    return NULL;
}

void XmlManagerBase::SetNodeText(xmlNode* n, const char  *t)
{
    if ( n == NULL )
        throw NgoErrorInvalidArgument(1,"trying to set the content of an unexisting node","XmlManagerBase::SetNodeText");

    xmlNodeSetContent( n , (const xmlChar*) t );
}

/* ------------------------------------------------------------------------------------------------------------------
*  Write and read values
*  Regardless of namespaces, the string keys app_path and data_path always refer to the location of the application's executable
*  and the data path, respectively. These values are never saved to the configuration, but kept in static variables.
*  The application makes use of this by "writing" to the configuration file after determining these values at runtime.
*/
void XmlManagerBase::Write(const std::string& name, xmlNode* pathNode,  const std::string& value, bool ignoreEmpty)
{
    if (ignoreEmpty && value.empty())
    {
        //UnSet(name);
        return;
    }

    if ( pathNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write to an undefined rootNode","XmlManagerBase::Write");

    std::string key(name);
    xmlNode* e = AssertPath(key, pathNode);
    xmlNode *str = GetUniqElement(e, key);
    xmlNodeSetContent(str, (const xmlChar*) value.c_str() );
}

std::string XmlManagerBase::Read(const std::string& name, xmlNode* rootNode, const std::string& defaultVal)
{
    std::string ret;

    if (Read(name, &ret, rootNode))
        return ret;
    else
        return defaultVal;
}

bool XmlManagerBase::Read(const std::string& name, std::string* str, xmlNode* rootNode )
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read to an undefined rootNode","XmlManagerBase::Read");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return false;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return false;

   xmlChar * value = xmlNodeGetContent(n);
   str->assign( (const char*) value );
   xmlFree(value);

   return !str->empty();
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------
* Writing and Reading ints
---------------------------------------------------------------------------------------------------------------------------------------------------*/
void XmlManagerBase::Write(const std::string& name, xmlNode* rootNode,  int value)
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write to an undefined rootNode","XmlManagerBase::Write");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );

    std::stringstream sStream;
    sStream << value;

    std::string val;
    sStream >> val;

    xmlNodeSetContent( e , (const xmlChar*) val.c_str() );
}

int  XmlManagerBase::ReadInt(const std::string& name, xmlNode* rootNode,  int defaultVal)
{
    int ret;

    if (Read(name, rootNode, &ret))
        return ret;
    else
        return defaultVal;
}

bool XmlManagerBase::Read(const std::string& name, xmlNode* rootNode ,  int* value)
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read from an undefined rootNode","XmlManagerBase::Read");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return false;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return false;

   xmlChar * content = xmlNodeGetContent(n);
   std::string str = (const char*) content ;
   xmlFree(content);

	 if( !str.empty() )
	 {
			std::stringstream sStream;
			sStream << str;
			sStream >> *value;

			if( NaN( *value ) )
				return false;

	 }else{
			return false;
	 }

    return true;
}

/*--------------------------------------------------------------------------------------------------------------------------------------------------
* Writing and Reading bools
---------------------------------------------------------------------------------------------------------------------------------------------------*/
void XmlManagerBase::Write(const std::string& name, xmlNode* rootNode,  bool value)
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write to an undefined rootNode","XmlManagerBase::Write");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );

    std::stringstream sStream;
    sStream << value;

    std::string val;
    sStream >> val;

    xmlNodeSetContent( e , (const xmlChar*) val.c_str() );
}

bool  XmlManagerBase::ReadBool(const std::string& name, xmlNode* rootNode,  bool defaultVal)
{
    bool ret;

    if (Read(name, rootNode, &ret))
        return ret;
    else
        return defaultVal;
}

bool XmlManagerBase::Read(const std::string& name, xmlNode* rootNode, bool* value)
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read from an undefined rootNode","XmlManagerBase::Read");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return false;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return false;

   xmlChar * content = xmlNodeGetContent(n);
   std::string str = (const char*) content ;
   xmlFree(content);

	 if( !str.empty() )
	 {
			std::stringstream sStream;
			sStream << str;
			sStream >> *value;

			if( NaN( *value ) )
				return false;

	 }else{
			return false;
	 }

    return true;

}


void XmlManagerBase::Write(const std::string& name,   xmlNode* rootNode, double value)
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write to an undefined rootNode","XmlManagerBase::Write");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );

    std::stringstream sStream;
    sStream.precision(12);
    sStream << std::scientific << value;

    std::string val;
    sStream >> val;

    xmlNodeSetContent( e , (const xmlChar*) val.c_str() );

}

double  XmlManagerBase::ReadDouble(const std::string& name,  xmlNode* rootNode,  double defaultVal)
{
    double ret;

    if (Read(name, rootNode , &ret))
        return ret;
    else
        return defaultVal;
}

bool XmlManagerBase::Read(const std::string& name,  xmlNode* rootNode, double* value)
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read from an undefined rootNode","XmlManagerBase::Read");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return false;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return false;

   xmlChar * content = xmlNodeGetContent(n);
   std::string str = (const char*) content ;
   xmlFree(content);
	 if( !str.empty() )
	 {
			std::stringstream sStream;
			sStream << str;
			sStream >> *value;

			if( NaN( *value ) )
				return false;

	 }else{
			return false;
	 }
    return true;
}

void XmlManagerBase::Write(const std::string& name, xmlNode* rootNode,  const std::vector<std::string>& arrayString)
{
   size_t found = name.find_last_of("/");
//   std::string key(name);
   std::string key = name.substr(0,found);
   std::string last = name.substr(found+1);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write to an undefined rootNode","XmlManagerBase::Write");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );
    xmlReplaceNode( e , NULL );
    e = GetUniqElement(node,key);

    for (unsigned int i = 0; i < arrayString.size(); ++i)
    {
//        xmlNode *Child = xmlNewNode( NULL, (const xmlChar*) "value" );
        xmlNode *Child = xmlNewNode( NULL, (const xmlChar*) last.c_str() );
        xmlNodeSetContent( Child , (const xmlChar*) arrayString[i].c_str() );
        xmlAddChild( e , Child );
    }
}

void XmlManagerBase::Read(const std::string& name, xmlNode* rootNode, std::vector<std::string> *arrayString)
{
   size_t found = name.find_last_of("/");
//   std::string key(name);
   std::string key = name.substr(0,found);
   std::string last = name.substr(found+1);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read from an undefined rootNode","XmlManagerBase::Read");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return;

    xmlNode *curr = n->children;
    while ( (curr != NULL) )
    {
		  std::string currName = (const char*) curr->name ;
//		  if( currName == "value" ){
		  if( currName == last ){
            xmlChar * value = xmlNodeGetContent(curr);
            std::string Value = (const char*) value ;
            xmlFree(value);
				arrayString->push_back( Value );
        }
        curr = curr->next;
    }
}

std::vector<std::string> XmlManagerBase::ReadStdArrayString(const std::string& name, xmlNode* rootNode)
{
    std::vector<std::string> as;
    Read(name, rootNode, &as);
    return as;
}

void XmlManagerBase::Write(const std::string& name, xmlNode* rootNode,  const std::vector<int>& arrayInt)
{
   size_t found = name.find_last_of("/");
//   std::string key(name);
   std::string key = name.substr(0,found);
   std::string last = name.substr(found+1);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write to an undefined rootNode","XmlManagerBase::Write");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );
    xmlReplaceNode( e , NULL );
    e = GetUniqElement(node,key);

    for (unsigned int i = 0; i < arrayInt.size(); ++i)
    {
//        xmlNode *Child = xmlNewNode( NULL, (const xmlChar*) "value" );
        xmlNode *Child = xmlNewNode( NULL, (const xmlChar*) last.c_str() );
        std::string val;
        std::stringstream sStream;
        sStream << arrayInt[i] ;
        sStream >> val;
        xmlNodeSetContent( Child , (const xmlChar*) val.c_str() );
        xmlAddChild( e , Child );
    }
}

void XmlManagerBase::Read(const std::string& name, xmlNode* rootNode, std::vector<int> *arrayInt)
{
   size_t found = name.find_last_of("/");
//   std::string key(name);
   std::string key = name.substr(0,found);
   std::string last = name.substr(found+1);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read from an undefined rootNode","XmlManagerBase::Read");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return;

    xmlNode *curr = n->children;
    while ( (curr != NULL) )
    {
		 std::string currName = (const char*) curr->name ;
//		  if( currName == "value" ){
		  if( currName == last ){
               xmlChar * value = xmlNodeGetContent( curr );
					std::string Value = (const char*) value;
					xmlFree(value);
					if( !Value.empty() )
					{
							std::stringstream sStream;
							sStream << Value;
							int val;
							sStream >> val;
							arrayInt->push_back( val );
					}else{
							arrayInt->push_back(-1);
					}
		  }
        curr = curr->next;
    }
}

std::vector<int> XmlManagerBase::ReadStdArrayInt(const std::string& name, xmlNode* rootNode)
{
    std::vector<int> as;
    Read(name, rootNode, &as);
    return as;
}

void XmlManagerBase::Write(const std::string& name, xmlNode* rootNode,  const std::vector<bool>& arrayBool)
{
   size_t found = name.find_last_of("/");
//   std::string key(name);
   std::string key = name.substr(0,found);
   std::string last = name.substr(found+1);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write to an undefined rootNode","XmlManagerBase::Write");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );
    xmlReplaceNode( e , NULL );
    e = GetUniqElement(node,key);

    for (unsigned int i = 0; i < arrayBool.size(); ++i)
    {
//        xmlNode *Child = xmlNewNode( NULL, (const xmlChar*) "value" );
        xmlNode *Child = xmlNewNode( NULL, (const xmlChar*) last.c_str() );
        std::string val;
        std::stringstream sStream;
        sStream << arrayBool[i] ;
        sStream >> val;
        xmlNodeSetContent( Child , (const xmlChar*) val.c_str() );
        xmlAddChild( e , Child );
    }
}

void XmlManagerBase::Read(const std::string& name, xmlNode* rootNode, std::vector<bool>*arrayBool)
{
   size_t found = name.find_last_of("/");
//   std::string key(name);
   std::string key = name.substr(0,found);
   std::string last = name.substr(found+1);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read from an undefined rootNode","XmlManagerBase::Read");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return;

    xmlNode *curr = n->children;
    while ( (curr != NULL) )
    {
        std::string currName = (const char*) curr->name ;
//		  if( currName == "value" ){
		  if( currName == last.c_str() ){
               xmlChar * value = xmlNodeGetContent( curr );
					std::string Value = (const char*) value;
					xmlFree(value);
					if( !Value.empty() )
					{
							std::stringstream sStream;
							sStream << Value;
							bool val;
							sStream >> val;
							arrayBool->push_back( val );
					}else{
							arrayBool->push_back(false);
					}
		  }
        curr = curr->next;
    }
}

std::vector<bool> XmlManagerBase::ReadStdArrayBool(const std::string& name, xmlNode* rootNode)
{
    std::vector<bool> as;
    Read(name, rootNode, &as);
    return as;
}

void XmlManagerBase::Write(const std::string& name, xmlNode* rootNode,  const std::vector<double>& arrayDouble)
{
   size_t found = name.find_last_of("/");
//   std::string key(name);
   std::string key = name.substr(0,found);
   std::string last = name.substr(found+1);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write to an undefined rootNode","XmlManagerBase::Write");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );
//    xmlReplaceNode( e , NULL );
//    e = GetUniqElement(node,key);

    for (unsigned int i = 0; i < arrayDouble.size(); ++i)
    {
//        xmlNode *Child = xmlNewNode( NULL, (const xmlChar*) "value" );
        xmlNode *Child = xmlNewNode( NULL, (const xmlChar*) last.c_str() );
        std::string val;
        std::stringstream sStream;
        sStream.precision(12);
        sStream << std::scientific << arrayDouble[i] ;
        sStream >> val;
        xmlNodeSetContent( Child , (const xmlChar*) val.c_str() );
        xmlAddChild( e , Child );
    }
}

void XmlManagerBase::Read(const std::string& name, xmlNode* rootNode, std::vector<double>*arrayDouble)
{
   size_t found = name.find_last_of("/");
//   std::string key(name);
   std::string key = name.substr(0,found);
   std::string last = name.substr(found+1);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read from an undefined rootNode","XmlManagerBase::Read");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return;

    xmlNode *curr = n->children;
    while ( (curr != NULL) )
    {
        std::string currName = (const char*) curr->name ;
//		  if( currName == "value" ){
		  if( currName == last ){
               xmlChar * value = xmlNodeGetContent( curr );
					std::string Value = (const char*) value;
					xmlFree(value);
					if( !Value.empty() )
					{
							std::stringstream sStream;
							sStream << Value;
							double val;
							sStream >> val;
							arrayDouble->push_back( val );
					}else{
							arrayDouble->push_back(-1);
					}
		  }
        curr = curr->next;
    }
}

std::vector<double> XmlManagerBase::ReadStdArrayDouble(const std::string& name, xmlNode* rootNode)
{
    std::vector<double> as;
    Read(name, rootNode, &as);
    return as;
}

std::vector<std::string> XmlManagerBase::EnumerateChildrens(const std::string& path, xmlNode* rootNode)
{
    std::vector<std::string> ret;
    std::string key(path);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to enumerate children from an undefined rootNode","XmlManagerBase::EnumerateChildrens");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return ret;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return ret;

    xmlNode *child = n->children;

    while ( child != NULL )
    {
        std::string childName = (const char*) child->name;
        ret.push_back( childName );
        child = child->next;
    }

    return ret;
}

void XmlManagerBase::DeleteChildrens(const std::string& strPath, xmlNode* rootNode)
{
    std::string key(strPath);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to delete children from an undefined rootNode","XmlManagerBase::DeleteChildrens");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return ;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return ;

    xmlNode *child = n->children;

    while ( child != NULL )
    {
        xmlNode* sub = child;
        child = child->next;

        xmlReplaceNode( sub , NULL );
    }

    return ;
}

void XmlManagerBase::WriteAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, const std::string& value,  bool ignoreEmpty)
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write an attribute to an undefined rootNode","XmlManagerBase::WriteAttribute");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );

    if ( value.empty() && !ignoreEmpty )
        return;

    xmlAttr* attr = xmlHasProp( e , (const xmlChar*) attribute.c_str() );
    if ( attr != NULL )
    {
        xmlSetProp( e , (const xmlChar*) attribute.c_str() , (const xmlChar*) value.c_str() );
    }
    else
    {
        xmlNewProp( e , (const xmlChar*) attribute.c_str() , (const xmlChar*) value.c_str() );
    }
}

bool XmlManagerBase::ReadAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, std::string* value )
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read an attribute from an undefined rootNode","XmlManagerBase::ReadAttribute");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return false;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return false;

    xmlAttr* attr = xmlHasProp( n , (const xmlChar*) attribute.c_str() );
    if ( attr != NULL )
    {
       xmlChar * attr =  xmlGetProp( n , (const xmlChar*) attribute.c_str() );
       value->assign( (const char*) attr);
       xmlFree(attr);
       return true;
    }
    return false;
}

std::string XmlManagerBase::ReadAttribute(const std::string& name, xmlNode* rootNode, const std::string& attribute, const std::string& defaultVal )
{
    std::string ret;
    if ( ReadAttribute( name , rootNode, attribute, &ret ) )
        return ret;

    return defaultVal;
}

void XmlManagerBase::WriteAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, const int& value )
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write an attribute to an undefined rootNode","XmlManagerBase::WriteAttribute");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );

	 std::stringstream sStream;
	 sStream << value;
	 std::string val;
	 sStream >> val;

    xmlAttr* attr = xmlHasProp( e , (const xmlChar*) attribute.c_str() );
    if ( attr != NULL )
    {
        xmlSetProp( e , (const xmlChar*) attribute.c_str() , (const xmlChar*) val.c_str() );
    }
    else
    {
        xmlNewProp( e , (const xmlChar*) attribute.c_str() , (const xmlChar*) val.c_str() );
    }
}

bool XmlManagerBase::ReadAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, int* value )
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read an attribute from an undefined rootNode","XmlManagerBase::ReadAttribute");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return false;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return false;

    xmlAttr* attr = xmlHasProp( n , (const xmlChar*) attribute.c_str() );
    if ( attr != NULL )
    {
		  std::string val;
		  xmlChar * prop = xmlGetProp( n , (const xmlChar*) attribute.c_str() );
        val.assign( (const char*) prop);
        xmlFree(prop);

        std::stringstream sStream;
		  sStream << val;

		  sStream >> *value;
        return true;
    }
    return false;
}

int XmlManagerBase::ReadAttributeInt(const std::string& name, xmlNode* rootNode, const std::string& attribute, const int& defaultVal )
{
    int ret;
    if ( ReadAttribute( name , rootNode, attribute, &ret ) )
        return ret;

    return defaultVal;
}

void XmlManagerBase::WriteAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, const bool& value )
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write an attribute to an undefined rootNode","XmlManagerBase::WriteAttribute");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );

	 std::stringstream sStream;
	 sStream << value;
	 std::string val;
	 sStream >> val;

    xmlAttr* attr = xmlHasProp( e , (const xmlChar*) attribute.c_str() );
    if ( attr != NULL )
    {
        xmlSetProp( e , (const xmlChar*) attribute.c_str() , (const xmlChar*) val.c_str() );
    }
    else
    {
        xmlNewProp( e , (const xmlChar*) attribute.c_str() , (const xmlChar*) val.c_str() );
    }
}

bool XmlManagerBase::ReadAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, bool* value )
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read an attribute from an undefined rootNode","XmlManagerBase::ReadAttribute");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return false;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return false;

    xmlAttr* attr = xmlHasProp( n , (const xmlChar*) attribute.c_str() );
    if ( attr != NULL )
    {
		  std::string val;
		  xmlChar * prop = xmlGetProp( n , (const xmlChar*) attribute.c_str() );
        val.assign( (const char*) prop);
        xmlFree(prop);

        std::stringstream sStream;
		  sStream << val;

		  sStream >> *value;

        if (val=="true")
           *value = true;
        if (val=="false")
           *value = false;

        return true;
    }
    return false;
}

bool XmlManagerBase::ReadAttributeBool(const std::string& name, xmlNode* rootNode, const std::string& attribute, const bool& defaultVal )
{
   bool ret;
   if ( ReadAttribute( name , rootNode, attribute, &ret ) )
       return ret;
   else
   {
      std::string boolstring;
      if ( ReadAttribute( name , rootNode, attribute, &boolstring ) )
      {
         if (boolstring == "false")
            return false;
         if (boolstring == "true")
            return true;
      }
   }

   return defaultVal;
}

void XmlManagerBase::WriteAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, const double& value )
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to write an attribute to an undefined rootNode","XmlManagerBase::WriteAttribute");

    xmlNode* node = AssertPath(key,rootNode);
    xmlNode* e = GetUniqElement( node , key );

	 std::stringstream sStream;
	 sStream << value;
	 std::string val;
	 sStream >> val;

    xmlAttr* attr = xmlHasProp( e , (const xmlChar*) attribute.c_str() );
    if ( attr != NULL )
    {
        xmlSetProp( e , (const xmlChar*) attribute.c_str() , (const xmlChar*) val.c_str() );
    }
    else
    {
        xmlNewProp( e , (const xmlChar*) attribute.c_str() , (const xmlChar*) val.c_str() );
    }
}

bool XmlManagerBase::ReadAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, double* value )
{
    std::string key(name);

    if ( rootNode == NULL )
        throw NgoErrorInvalidArgument(2,"trying to read an attribute from an undefined rootNode","XmlManagerBase::ReadAttribute");

    xmlNode* e = AssertPath( key, rootNode, false );
    if ( e == NULL )
        return false;

    xmlNode *n = GetUniqElement(e, key, false);
    if ( n == NULL )
        return false;

    xmlAttr* attr = xmlHasProp( n , (const xmlChar*) attribute.c_str() );
    if ( attr != NULL )
    {
		  std::string val;
        val.assign( (const char*) xmlGetProp( n , (const xmlChar*) attribute.c_str() ) );

        std::stringstream sStream;
		  sStream << val;

		  sStream >> *value;
        return true;
    }
    return false;
}

double XmlManagerBase::ReadAttributeDouble(const std::string& name, xmlNode* rootNode, const std::string& attribute, const double& defaultVal )
{
    double ret;
    if ( ReadAttribute( name , rootNode, attribute, &ret ) )
        return ret;

    return defaultVal;
}
