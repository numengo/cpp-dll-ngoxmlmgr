
dofile (path.join(os.getenv("DIR_SCRIPTS_ROOT"),"premake5_common.lua"))

workspace "NgoXmlMgr" 

    SolutionConfiguration()
    defines {
        "NGOLIBXML2_USE_DYN",
        "NGOERR_USE_DYN",
        "NGOCOM_USE_DYN",
        "XMLMGR_USE_DYN"
    }
    local _exportSymbol = "XMLMGR_MAKE_DLL"
    links { 
        "NgoLibxml2",
        "NgoErr",
        "NgoCommon"
    }
    
    -- PROTECTED REGION ID(NgoXmlMgr.premake.solution) ENABLED START
    -- Insert here user code

    -- End of user code
    -- PROTECTED REGION END

project "NgoXmlMgr"

    PrefilterSharedLibBuildOptions("NgoXmlMgr")
    defines {_exportSymbol}
    
   -- PROTECTED REGION ID(NgoXmlMgr.premake.sharedlib) ENABLED START
   -- Insert here user code

   -- End of user code
   -- PROTECTED REGION END

    FilterSharedLibBuildOptions("NgoXmlMgr")


    
