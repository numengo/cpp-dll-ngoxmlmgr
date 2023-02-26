dofile (path.join(os.getenv("DIR_SCRIPTS_ROOT"),"premake5_common.lua"))

workspace "NgoXmlMgr"

    SolutionConfiguration()
    defines {
        "NGO_ERR_USE_DYN"
	    ,"NGO_COM_USE_DYN"
		,"XMLMGR_USE_DYNAMIC"
    }
    local _exportSymbol = "XMLMGR_MAKING_DLL"
    links { 
               "NgoErr"
               ,"NgoCommon"
    }
    
    -- PROTECTED REGION ID(NgoXmlMgr.premake.solution) ENABLED START
	configuration {"vs2010","Release"}
			links {"NgoLibxml2"}
	configuration {"vs2010","Debug"}
			links {"NgoLibxml2"}
	configuration {"linux"}
			links {"xml2"}

    -- PROTECTED REGION END


project "NgoXmlMgr"

    PrefilterSharedLibBuildOptions("NgoXmlMgr")
    defines {_exportSymbol}
    
    -- PROTECTED REGION ID(NgoXmlMgr.premake.sharedlib) ENABLED START

    -- PROTECTED REGION END

    FilterSharedLibBuildOptions("NgoXmlMgr")


project "test_NgoXmlMgr"

    PrefilterTestBuildOptions("test_NgoXmlMgr")
    links { "NgoXmlMgr"}
    
    -- PROTECTED REGION ID(NgoXmlMgr.premake.test) ENABLED START

    -- PROTECTED REGION END

    FilterTestBuildOptions("test_NgoXmlMgr")
