
dofile (path.join(os.getenv("DirScriptsRoot"),"premake_common.lua"))

-- solution NgoXmlMgr --
solution "NgoXmlMgr"

    SolutionConfiguration()

    -- COMMON CONFIGURATION MODIFICATION - START --
    configuration {}
        -- common defines (adapt if necessary) --
       defines {
                "NGO_ERR_USE_DYN"
			   ,"NGO_COM_USE_DYN"
               ,"XMLMGR_USE_DYNAMIC"
               }                
       -- for shared libs, export statement
       local _exportSymbol = "XMLMGR_MAKING_DLL"
       -- suffix to use for library versionning
       local _version = ""
       -- common libs  --
       links { 
                "NgoLibxml2"
               ,"NgoErr"
               ,"NgoCommon"
               }
    -- COMMON CONFIGURATION MODIFICATION - END --

project "NgoXmlMgr"
    -- PROJECT MODIFICATIONS START--
    local _targetname = "NgoXmlMgr"
    -- additional defines --
    defines {_exportSymbol}
    -- PROJECT MODIFICATIONS END--

    AppendSharedLibBuildOptions(_targetname.._version)

project "test_NgoXmlMgr"
    -- PROJECT MODIFICATIONS START--
    local _targetname = "test_NgoXmlMgr"
    links {"NgoXmlMgr"}
    -- PROJECT MODIFICATIONS END--

    AppendTestBuildOptions(_targetname.._version)
