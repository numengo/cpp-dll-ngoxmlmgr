/**
*			@file XmlManagerBase.h
*
*			@author Nicolas Macherey (nm@graymat.fr)
*			@date 31-Janv-2009
*			@version 0.0.1
*
*			This file is owned by Nicolas Macherey and Cedric Roman and cannot be used by any ohter
*			third party without the written consent of one of the two authors
*/

#ifndef _XmlManagerBase_h_
#define _XmlManagerBase_h_

#include <xmlmgr/XmlManagerExports.h>
#include <xmlmgr/XmlManagerGlobals.h>

#include "ngocommon/NgoSingletonManager.h"

#include <iostream>
#include <vector>
#include <map>

/* lib xml imports */

typedef struct _xmlNode xmlNode;
typedef struct _xmlDoc xmlDoc;
typedef struct _xmlNs xmlNs;

/*! @brief wrapper to xmlParseFile
parse an XML file and build a tree. Automatic support for ZLIB/Compress compressed document is provided by default if found at compile-time.
@param filename the filename
@return the resulting document tree if the file was wellformed, NULL otherwise.
*/
XMLMGR_IMPORT _xmlDoc * XmlMgrParseFile(const char * filename);

/*! @brief wrapper to xmlDocSaveFormatFileEnc
Dump an XML document to a file or an URL.
@param filename the filename or URL to output
@param cur the document being saved
@param encoding the name of the encoding to use or NULL.
@param format should formatting spaces be added.
@return the number of bytes written or -1 in case of error. Note that format = 1 provide node indenting only if xmlIndentTreeOutput = 1 or xmlKeepBlanksDefault(0) was called
*/
XMLMGR_IMPORT int XmlMgrSaveFormatFileEnc(const char * filename, _xmlDoc * cur, const char * encoding, int format);

/*! @brief wrapper to xmlNewDoc
Creates a new XML document
@param version string giving the version of XML "1.0"
@return a new document */
XMLMGR_IMPORT _xmlDoc * XmlMgrNewDoc(const char * version);
/*! @brief wrapper to xmlFreeDoc
Free up all the structures used by a document, tree included.
@param cur pointer to the document */
XMLMGR_IMPORT void XmlMgrFreeDoc(_xmlDoc * cur);
/*! @brief wrapper to xmlNewDocNode
Creation of a new node element within a document. ns and content are optional (NULL).
@param doc the document
@param ns namespace if any
@param name the node name
@param content the XML text content if any
@return a pointer to the new node objec
*/
XMLMGR_IMPORT _xmlNode * XmlMgrNewDocNode(_xmlDoc * doc, _xmlNs * ns, const char * name, const char * content);
/*! @brief wrapper to xmlDocSetRootElement
Set the root element of the document (doc->children is a list containing possibly comments, PIs, etc ...).
@param doc the document
@param root the new document root element, if root is NULL no action is taken, to remove a node from a document use xmlUnlinkNode(root) instead.
@return the old root element if any was found, NULL if root was NULL
*/
XMLMGR_IMPORT _xmlNode * XmlMgrDocSetRootElement(_xmlDoc * doc, _xmlNode * root);
/*! @brief wrapper to xmlDocGetRootElement
Get the root element of the document (doc->children is a list containing possibly comments, PIs, etc ...).
@param doc the document
@return the #xmlNodePtr for the root or NULL
*/

XMLMGR_IMPORT _xmlNode * XmlMgrDocGetRootElement(_xmlDoc * doc);
/*! @brief wrapper to xmlUnlinkNode
Unlink a node from it's current context, the node is not freed
@param cur the node
*/
XMLMGR_IMPORT void XmlMgrUnlinkNode(_xmlNode * cur);
/*! @brief wrapper to xmlFreeNode
Free a node, this is a recursive behaviour, all the children are freed too. This doesn't unlink the child from the list, use xmlUnlinkNode() first.
@param cur the node
*/
XMLMGR_IMPORT void XmlMgrFreeNode(_xmlNode * cur);

/*******************************************************************************************************************
*	XmlManager Imports
*******************************************************************************************************************/
class XmlManagerNumengoBase;

/**
*		@class XmlManagerBase
*
*		@brief The XmlManagerBase class is managing basic xml Node for writing and readding standard streams
*
*		@author Nicolas Macherey (nm@graymat.fr)
*		@date	31-Jan-2009
*		@version 0.0.1
*/

class XMLMGR_IMPORT XmlManagerBase : public NgoSingleton<XmlManagerBase>
{
   /** Give our private members access to the NgoXmlManagerBase */
   friend class NgoSingleton<XmlManagerBase>;
   friend class NgoXmlManagerBase;

public :
    /*************************************************************************************************************************
    *	General tools
    *************************************************************************************************************************/

    /** Enumerates the first level subpath (child node names that are paths)
    *  @param path path to enumerate from rootNode
    *     @param rootNode root node from which to start the path node find
    */
    std::vector<std::string> EnumerateChildrens(const std::string& path, xmlNode* rootNode);

    /** Delete the node given by the path starting from root node
    *  @param strPath subpath to delete from rootNode
    *	 @param rootNode root node from which to start the path node find
    */
    void DeleteChildrens(const std::string& strPath, xmlNode *rootNode);

    /*************************************************************************************************************************
    *	Clearing and Deleting
    *************************************************************************************************************************/
    /**
    *		Clear the given node it means replace this node by NULL in the tree node
    */
    void Clear( xmlNode* node );

    /**
    *		Delete a document, it means delete the doc and free its memory on hard drive
    */
    void Delete( xmlDoc* doc );

    /*************************************************************************************************************************
    *	Standard String manipulation
    *************************************************************************************************************************/

    /**
    * @brief Write method for writing a string in a node given by its name path/key
    *
    *	@param name path/key string in which to write the string
    *	@param pathNode the node treated as root for the given path
    *	@param value the string to write in the node
    *	@param ignoreEmpty if false, do note write empty string
    */
    void Write(const std::string& name, xmlNode* pathNode, const std::string& value, bool ignoreEmpty = false);

	/**
    * @brief Read method for reading a string from a node given by its name path/key
    *
    *	@param name path/key string in which to write the string
    *	@param rootNode the node treated as root for the given path
    *	@param defaultVal the returned value if the string is not found
    */
    std::string Read(const std::string& key, xmlNode* rootNode, const std::string& defaultVal = "");

	/**
    * @brief Read method for reading a string from a node given by its name path/key
    *
    *	@param name path/key string in which to write the string
    *	@param str pointer to the string variable to fill
    *	@param rootNode root node from which to read
    *
    *	@return returns true if the string has been filled by the read
    */
    bool Read(const std::string& key, std::string* str, xmlNode* rootNode);

    /*************************************************************************************************************************
    *	Standard Int manipulation
    *************************************************************************************************************************/

	/**
    * @brief Write method for writing an int to the node given by its name path/key
    *
    *	@param name path/key string in which to write the int
    *	@param rootNode root node from which to read
    * @param value the value to write
    */
    void Write(const std::string& name, xmlNode* rootNode,  int value);

    /**
    * @brief Read method for reading an int from a node given by its name path/key
    *
    *	@param name path/key string in which to read the int
    *	@param rootNode root node from which to read
    *	@param value pointer to the int value to fill
    *
    *	@return returns true if the value has been filled
    */
    bool Read(const std::string& name, xmlNode* rootNode,  int* value);

	 /**
    * @brief Read method for reading an int from a node given by its name path/key
    *
    *	@param name path/key string in which to read the int
    *	@param rootNode root node from which to read
    *	@param defaultVal the value to return if the node is not found
    *
    *	@return returns the read value if the value has been read else returns the default value
    */
    int  ReadInt(const std::string& name, xmlNode* rootNode,  int defaultVal = 0);

    /*************************************************************************************************************************
    *	Standard Booleans manipulation
    *************************************************************************************************************************/

	/**
    * @brief Write method for writing a bool to the node given by its name path/key
    *
    *	@param name path/key string in which to write the string
    *	@param rootNode root node from which to read
    * @param value the value to write
    */
    void Write(const std::string& name,xmlNode* rootNode,  bool value);

    /**
    * @brief Read method for reading a bool from a node given by its name path/key
    *
    *	@param name path/key string in which to read the bool
    *	@param rootNode root node from which to read
    *	@param value pointer to the bool value to fill
    *
    *	@return returns true if the value has been filled
    */
    bool Read(const std::string& name,xmlNode* rootNode,  bool* value);

    /**
    * @brief Read method for reading a bool from a node given by its name path/key
    *
    *	@param name path/key string in which to read the bool
    *	@param rootNode root node from which to read
    *	@param defaultVal the value to return if the node is not found
    *
    *	@return returns the read value if the value has been read else returns the default value
    */
    bool ReadBool(const std::string& name,xmlNode* rootNode,  bool defaultVal = false);

	/*************************************************************************************************************************
    *	Standard Double manipulation
    *************************************************************************************************************************/

	/**
    * @brief Write method for writing a double to the node given by its name path/key
    *
    *	@param name path/key string in which to write the double
    *	@param rootNode root node from which to write
    * @param value the value to write
    */
    void Write(const std::string& name, xmlNode* rootNode, double value);

    /**
    * @brief Read method for reading a double from a node given by its name path/key
    *
    *	@param name path/key string in which to read the double
    *	@param rootNode root node from which to read
    *	@param value pointer to the double value to fill
    *
    *	@return returns true if the value has been filled
    */
    bool Read(const std::string& name, xmlNode* rootNode,  double* value);

    /**
    * @brief Read method for reading a double from a node given by its name path/key
    *
    *	@param name path/key string in which to read the double
    *	@param rootNode root node from which to read
    *	@param defaultVal the value to return if the node is not found
    *
    *	@return returns the read value if the value has been read else returns the default value
    */
    double ReadDouble(const std::string& name, xmlNode* rootNode, double defaultVal = 0.0f);

    /*************************************************************************************************************************
    *	Standard std::vector<std::string> manipulation
    *************************************************************************************************************************/

	/**
    * @brief Write method for writing an array string to the node given by its name path/key
    *
    *	@param name path/key string in which to write the array string
    *	@param rootNode root node from which to write
    * @param arrayString the array string to write
    */
    void Write(const std::string& name,  xmlNode* rootNode, const std::vector<std::string>& arrayString);

    /**
    * @brief Read method for reading an array string from the node given by its name path/key
    *
    *	@param name path/key string from which to read the array string
    *	@param rootNode root node from which to read
    * @param arrayString the array string to fill
    */
    void Read(const std::string& name, xmlNode* rootNode, std::vector<std::string>* arrayString);

     /**
    * @brief Read method for reading an array string from the node given by its name path/key
    *
    *	@param name path/key string from which to read the array string
    *	@param rootNode root node from which to read
    *
    *	@return Returns the read array string
    */
    std::vector<std::string> ReadStdArrayString(const std::string& name, xmlNode* rootNode);

	/*************************************************************************************************************************
    *	Standard std::vector<int> manipulation
    *************************************************************************************************************************/
	/**
    * @brief Write method for writing an array int to the node given by its name path/key
    *
    *	@param name path/key string in which to write the array int
    *	@param rootNode root node from which to write
    * @param arrayString the array string to write
    */
    void Write(const std::string& name,  xmlNode* rootNode, const std::vector<int>& arrayInt);

    /**
    * @brief Read method for reading an array int from the node given by its name path/key
    *
    *	@param name path/key string from which to read the array int
    *	@param rootNode root node from which to read
    * @param arrayInt the array int to fill
    */
    void Read(const std::string& name, xmlNode* rootNode, std::vector<int>* arrayInt);

    /**
    * @brief Read method for reading an array i frotm the node given by its name path/key
    *
    *	@param name path/key string from which to read the array int
    *	@param rootNode root node from which to read
    *
    *	@return Returns the read array int
    */
    std::vector<int> ReadStdArrayInt(const std::string& name,xmlNode* rootNode);

    /*************************************************************************************************************************
    *	Standard std::vector<double> manipulation
    *************************************************************************************************************************/

	/**
    * @brief Write method for writing an array double to the node given by its name path/key
    *
    *	@param name path/key string in which to write the array double
    *	@param rootNode root node from which to write
    * @param arrayDouble the array double to write
    */
	 void Write(const std::string& name,  xmlNode* rootNode, const std::vector<double>& arrayDouble);

	 /**
    * @brief Read method for reading an array double from the node given by its name path/key
    *
    *	@param name path/key string from which to read the array double
    *	@param rootNode root node from which to read
    * @param arrayDouble the array double to fill
    */
    void Read(const std::string& name, xmlNode* rootNode, std::vector<double>* arrayDouble);

     /**
    * @brief Read method for reading an array double from the node given by its name path/key
    *
    *	@param name path/key string from which to read the array double
    *	@param rootNode root node from which to read
    *
    *	@return Returns the read array double
    */
    std::vector<double> ReadStdArrayDouble(const std::string& name,xmlNode* rootNode);

    /*************************************************************************************************************************
    *	Standard std::vector<bool> manipulation
    *************************************************************************************************************************/

	/**
    * @brief Write method for writing an array bool to the node given by its name path/key
    *
    *	@param name path/key string in which to write the array bool
    *	@param rootNode root node from which to write
    * @param arrayBool the array bool to write
    */
    void Write(const std::string& name,  xmlNode* rootNode, const std::vector<bool>& arrayBool);

    /**
    * @brief Read method for reading an array bool from the node given by its name path/key
    *
    *	@param name path/key string from which to read the array bool
    *	@param rootNode root node from which to read
    * @param arrayBool the array bool to fill
    */
    void Read(const std::string& name, xmlNode* rootNode, std::vector<bool>* arrayBool);

	/**
    * @brief Read method for reading an array Bool from the node given by its name path/key
    *
    *	@param name path/key string from which to read the array bool
    *	@param rootNode root node from which to read
    *
    *	@return Returns the read array bool
    */
    std::vector<bool> ReadStdArrayBool(const std::string& name,xmlNode* rootNode);

	/*************************************************************************************************************************
    *	Standard String Attributes manipulation
    *************************************************************************************************************************/
    /**
    * @brief Write method for writing a string in a node attribute given by its name path/key
    *
    *	@param name path/key string in which to write the string
		*	@param rootNode the node treated as root for the given path
    *	@param attribute the attribute's name string to write in the node
    *	@param value the attribute's value string to write in the node
    *	@param ignoreEmpty if false, do note write empty string
    */
    void WriteAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, const std::string& value,  bool ignoreEmpty = false);

	/**
    * @brief Read method for reading a string from a node given by its name path/key
    *
    *	@param name path/key string in which to write the string
    *	@param rootNode the node treated as root for the given path
    *	@param attribute the attribute's name string to read from the node
    *	@param defaultVal the returned value if the string is not found
    */
    std::string ReadAttribute(const std::string& name, xmlNode* rootNode, const std::string& attribute, const std::string& defaultVal = "");

	/**
    * @brief Read method for reading a string from a node given by its name path/key
    *
    *	@param name path/key string in which to write the string
		*	@param rootNode root node from which to read
    *	@param attribute the attribute's name string to read from the node
    *	@param str pointer to the string variable to fill
    *
    *	@return returns true if the string has been filled by the read
    */
    bool ReadAttribute(const std::string& name, xmlNode* rootNode, const std::string& attribute, std::string* str);

    /*************************************************************************************************************************
    *	Standard Int Attributes manipulation
    *************************************************************************************************************************/
    /**
    * @brief Write method for writing an int in a node attribute given by its name path/key
    *
    *	@param name path/key string in which to write the int
		*	@param rootNode the node treated as root for the given path
    *	@param attribute the attribute's name string to write in the node
    *	@param value the attribute's value int to write in the node
    */
    void WriteAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, const int& value);

	/**
    * @brief Read method for reading an int from a node given by its name path/key
    *
    *	@param name path/key string in which to read the int
    *	@param rootNode the node treated as root for the given path
    *	@param attribute the attribute's name string to read from the node
    *	@param defaultVal the returned value if the int is not found
    */
    int ReadAttributeInt(const std::string& name, xmlNode* rootNode, const std::string& attribute, const int& defaultVal = -1);

	/**
    * @brief Read method for reading an int from a node given by its name path/key
    *
    *	@param name path/key string in which to read the int
		*	@param rootNode root node from which to read
    *	@param attribute the attribute's name string to read from the node
    *	@param value pointer to the int variable to fill
    *
    *	@return returns true if the int has been filled by the read
    */
    bool ReadAttribute(const std::string& name, xmlNode* rootNode, const std::string& attribute, int* value);

     /*************************************************************************************************************************
    *	Standard Bool Attributes manipulation
    *************************************************************************************************************************/
    /**
    * @brief Write method for writing an bool in a node attribute given by its name path/key
    *
    *	@param name path/key string in which to write the bool
		*	@param rootNode the node treated as root for the given path
    *	@param attribute the attribute's name string to write in the node
    *	@param value the attribute's value bool to write in the node
    */
    void WriteAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, const bool& value);

	/**
    * @brief Read method for reading an bool from a node given by its name path/key
    *
    *	@param name path/key string in which to read the bool
    *	@param rootNode the node treated as root for the given path
    *	@param attribute the attribute's name string to read from the node
    *	@param defaultVal the returned value if the bool is not found
    */
    bool ReadAttributeBool(const std::string& name, xmlNode* rootNode, const std::string& attribute, const bool& defaultVal = true);

	/**
    * @brief Read method for reading an bool from a node given by its name path/key
    *
    *	@param name path/key string in which to read the bool
		*	@param rootNode root node from which to read
    *	@param attribute the attribute's name string to read from the node
    *	@param value pointer to the bool variable to fill
    *
    *	@return returns true if the bool has been filled by the read
    */
    bool ReadAttribute(const std::string& name, xmlNode* rootNode, const std::string& attribute, bool* value);

     /*************************************************************************************************************************
    *	Standard Double Attributes manipulation
    *************************************************************************************************************************/
    /**
    * @brief Write method for writing an double in a node attribute given by its name path/key
    *
    *	@param name path/key string in which to write the double
		*	@param rootNode the node treated as root for the given path
    *	@param attribute the attribute's name string to write in the node
    *	@param value the attribute's value double to write in the node
    */
    void WriteAttribute(const std::string& name,  xmlNode* rootNode, const std::string& attribute, const double& value);

	/**
    * @brief Read method for reading an double from a node given by its name path/key
    *
    *	@param name path/key string in which to read the double
    *	@param rootNode the node treated as root for the given path
    *	@param attribute the attribute's name string to read from the node
    *	@param defaultVal the returned value if the double is not found
    */
    double ReadAttributeDouble(const std::string& name, xmlNode* rootNode, const std::string& attribute, const double& defaultVal = true);

	/**
    * @brief Read method for reading an double from a node given by its name path/key
    *
    *	@param name path/key string in which to read the double
		*	@param rootNode root node from which to read
    *	@param attribute the attribute's name string to read from the node
    *	@param value pointer to the double variable to fill
    *
    *	@return returns true if the double has been filled by the read
    */
    bool ReadAttribute(const std::string& name, xmlNode* rootNode, const std::string& attribute, double* value);
// protected because we use the Singleton template
protected :
    /**
    * Default constructor the one you cannot use
    */
    XmlManagerBase(){};

    /**
    * Default destructor the one you cannot use
    */
    ~XmlManagerBase();

// protected
private :
    /*************************************************************************************************************************
    *	Private methods for managing nodes
    *************************************************************************************************************************/
    /**
    * @brief AssertPath method is used for getting a node associated to a path
    *
    *	You can set the @code create_unexisting variable to true if you want to create your
    *	node in the document tree if does not exists.
    *	A path is a unique node container that contains key nodes (childrens) the key nodes are
    *	the default defined nodes used by the XmlManager
    *
    *	@param path relative path from the path node to node path to assert
    *	@param pathNode root node from which to start the path search (working same as directories)
    *	@param create_unexisting Set to true if we want to create the node if it does not exist.
    */
    xmlNode* AssertPath( std::string& path,
                         xmlNode* pathNode, bool create_unexisting = true );

    /**
    *	@brief GetUniqElement method is used for getting a node associated to a key
    *
    * This method is used for getting a node pointer to a key. You can set the
    * @code create_unexisting variable to false if you do not want to create the
    *	key node if it does not exist.
    *	A path is a unique node container that contains key nodes (childrens) the key nodes are
    *	the default defined nodes used by the XmlManager
    *
    *	@param p node from which to get the key node
    *	@param q name of the key node or key string
    *	@param create_unexisting Set to true if we want to create the node if it does not exist.
    */
    xmlNode* GetUniqElement(xmlNode* p, const std::string& q, bool create_unexisting = true);

    /**
    * 	@brief SetNodeText method is used to set the string content of a node
    *
    *		@param n the node in which to set the text
    *		@param t the text to set
    */
    void SetNodeText(xmlNode *n, const char* t);

    /**
    *		@brief Collapse method is used for collapsing paths
    *
    *		@param str the path string to collapse
    */
    inline void Collapse(std::string& str) const;
};


#endif
