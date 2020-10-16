<?php
	
	
	function GetTestStatusEnum()
	{
		$arrayStatus = array(
			"PASSED",
			"INACTIVE",
			"FAILED",
			"UNDEFINED",
			"REQUESTERROR",
			"TESTING",
			"UNKNOWN"
			);
		return $arrayStatus;
	}
	
	function GetEntityDefinitionTest()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "status_id",
			"userfriendlyattribute" => "test_path", 
			"blocksize" => 100,
			"dbtablename" => "System-TestStatus",
			"uniqueindices" => array(
				"unique" => array("site_path","assembly_id","group_id","test_id") 
				)
			,	
			"noitemsfoundmessage" => "?TID_TESTMANAGER_INFO_NOTESTSFOUND?",
			"webservice" => array(
				"name" => "system/test",
				"itemtag" => "TEST",
				"commands" => array(
					"list" => array(
						"additionalparams" => array("dotest","getstatus")
						)
					)
				)
			,
			"sorting" => array(
				"defaultattribute" => "test_path",
				"defaultorder" => "asc")
			,
			"attributes" => array(
				"status_id" => array(
					"type" => "numericid",
					"excludefromget" => true,
					"readonly" => true
					)
				,
				"test_path" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"charset" => CHARSET_TESTPATH,
					"minlen" => 7,
					//"maxlen" => 255,
					"maxlen" => 512,
					"nostorage" => true,
					"readonly" => true,
					"sortoption" => true
					)
				,
				"site_path" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"charset" => CHARSET_TESTID,
					"maxlen" => 64,
					"sortoption" => true,
					"filteroption" => true
					)
				,
				"assembly_id" => array(
					"webserviceparam" => true,
					"type" => "string",
					"charset" => CHARSET_TESTID,
					"maxlen" => 64,
					"sortoption" => true,
					"filteroption" => true
					)
				,					
				"group_id" => array(
					"webserviceparam" => true, 					
					"type" => "string",
					"charset" => CHARSET_TESTID,
					"maxlen" => 64,
					"sortoption" => true,
					"filteroption" => true
					)
				,					
				"test_id" => array(
					"type" => "string",
					"webserviceparam" => true,
					"charset" => CHARSET_TESTID,					
					"maxlen" => 64,
					"sortoption" => true,
					"filteroption" => true
					)
				,
				"url" => array(
					"type" => "string",
					"webserviceparam" => true,
					"charset" => CHARSET_URL,					
					"maxlen" => 256
					)
				,
				"count" => array(
					"webserviceparam" => true,
					"type" => "integer",
					"min" => 0,
					"max" => INTEGER_MAX
					)
				,
				"firstexecution_datetime" => array(
					"webserviceparam" => true,
					"type" => "datetime",
					"sortoption" => true
					) 
				,
				"lastexecution_status" => array(
					"webserviceparam" => true,
					"type" => "enum",
					"enum" => GetTestStatusEnum(),
					"filteroption" => true,
					"default" => "UNKNOWN"
					)
				,
				"lastexecution_datetime" => array(
					"webserviceparam" => true,
					"type" => "datetime",
					"sortoption" => true
					)
				,
				"lastexecution_runtime_seconds" => array(
					"webserviceparam" => true,
					"type" => "float",
					"floatprecision" => 3,
					"min" => 0,
					"max" => INTEGER_MAX
					)
				,
				"lastexecution_hash" => array(
					"webserviceparam" => true,
					"type" => "string",
					"charset" => CHARSET_HEX,					
					"minlen" => 40,
					"maxlen" => 40
					)
				,
				"lastexecution_attempts" => array(
					"webserviceparam" => true,
					"type" => "integer",
					"min" => 0,
					"max" => INTEGER_MAX
					)
				,
				"lastexecution_log" => array(
					"webserviceparam" => true,
					"excludefromlist" => true,
					"type" => "string",
					"minlen" => 0,
					"maxlen" => ENTITY_STRINGLENGTH_MAX
					)
				,
				"test_script" => array(
					"type" => "string",
					"maxlen" => ENTITY_STRINGLENGTH_MAX,
					"hideinlist" => true,
					"excludefromlist" => true,
					"nostorage" => true,
					"readonly" => true
					)
				)
			);
			
		return $arrayEntityDefinition;
	}
	
	function GetEntityDefinitionTestStatusHistory()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "status_history_id",
			"userfriendlyattribute" => "datetime", 
			"blocksize" => 100,
			"dbtablename" => "System-TestStatusHistory",
			"noitemsfoundmessage" => "",
			"uniqueindices" => array(
				"unique" => array("site_path","assembly_id","group_id","test_id","status","datetime")
				)
			,	
			"webservice" => array(
				"name" => "system/test",
				"itemtag" => "TEST",
				"commands" => array(
					"list" => array()
					)
				)
			,
			"sorting" => array(
				"defaultattribute" => "datetime",
				"defaultorder" => "desc")
			,
			"attributes" => array(
				"status_history_id" => array(
					"type" => "numericid",
					"excludefromget" => true,
					"readonly" => true
					)
				,
				"site_path" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"charset" => CHARSET_TESTID,
					"maxlen" => 64,
					"sortoption" => true,
					"filteroption" => true
					)
				,
				"assembly_id" => array(
					"webserviceparam" => true,
					"type" => "string",
					"charset" => CHARSET_TESTID,
					"maxlen" => 64,
					"sortoption" => true,
					"filteroption" => true
					)
				,					
				"group_id" => array(
					"webserviceparam" => true, 					
					"type" => "string",
					"charset" => CHARSET_TESTID,
					"maxlen" => 64,
					"sortoption" => true,
					"filteroption" => true
					)
				,					
				"test_id" => array(
					"type" => "string",
					"webserviceparam" => true,
					"charset" => CHARSET_TESTID,					
					"maxlen" => 64,
					"sortoption" => true,
					"filteroption" => true
					)
				,
				"status" => array(
					"webserviceparam" => true,
					"type" => "enum",
					"enum" => GetTestStatusEnum(),
					"default" => "UNKNOWN",
					"filteroption" => true
					)
				,
				"datetime" => array(
					"webserviceparam" => true,
					"type" => "datetime",
					"sortoption" => true
					)
				,
				"runtime_seconds" => array(
					"webserviceparam" => true,
					"type" => "float",
					"floatprecision" => 3,
					"min" => 0,
					"max" => INTEGER_MAX
					)
				,
				"hash" => array(
					"webserviceparam" => true,
					"type" => "string",
					"charset" => CHARSET_HEX,					
					"minlen" => 40,
					"maxlen" => 40
					)
				,
				"log" => array(
					"webserviceparam" => true,
					"type" => "string",
					"minlen" => 0,
					"maxlen" => ENTITY_STRINGLENGTH_MAX
					)
				)
			);

		
		return $arrayEntityDefinition;
	}	

	function GetEntityDefinitionTestSite()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "site_id",
			"userfriendlyattribute" => "site_id", 
			"blocksize" => 100,
			"noitemsfoundmessage" => "",
			"webservice" => array(
				"name" => "system/test",
				"itemtag" => "SITE",
				"commands" => array(
					"list" => array()
					)
				)
			,
			"sorting" => array(
				"defaultattribute" => "site_name",
				"defaultorder" => "asc")
			,
			"attributes" => array(
				"site_path" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"maxlen" => 64,
					"charset" => CHARSET_TESTID,
					"sortoption" => true,
					"readonly" => true
					)
				,
				"site_id" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"maxlen" => 64,
					"sortoption" => true,
					"filteroption" => true,
					"readonly" => true
					)
				,
				"site_name" => array(
					"webserviceparam" => true,
					"type" => "string",
					"maxlen" => 64,
					"sortoption" => true,
					"readonly" => true
					)
				,
				"url" => array(
					"type" => "string",
					"maxlen" => 255,
					"readonly" => true
					)

				)
			);

		
		return $arrayEntityDefinition;
	}



	function GetEntityDefinitionTestAssembly()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "assembly_id",
			"userfriendlyattribute" => "assembly_id", 
			"blocksize" => 100,
			"noitemsfoundmessage" => "",
			"webservice" => array(
				"name" => "system/test",
				"itemtag" => "SITE",
				"commands" => array(
					"list" => array()
					)
				)
			,
			"sorting" => array(
				"defaultattribute" => "assembly_id",
				"defaultorder" => "asc")
			,
			"attributes" => array(
				"assembly_id" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"maxlen" => 64,
					"charset" => CHARSET_TESTID,
					"sortoption" => true,
					"readonly" => true
					)
				,
				"site_path" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"maxlen" => 64,
					"charset" => CHARSET_TESTID,
					"sortoption" => true,
					"filteroption" => true,
					"readonly" => true
					)
				)
			);

		
		return $arrayEntityDefinition;
	}

	
	
	function GetEntityDefinitionTestGroup()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "group_id",
			"userfriendlyattribute" => "group_id", 
			"blocksize" => 100,
			"noitemsfoundmessage" => "",
			"webservice" => array(
				"name" => "system/test",
				"itemtag" => "SITE",
				"commands" => array(
					"list" => array()
					)
				)
			,
			"sorting" => array(
				"defaultattribute" => "group_id",
				"defaultorder" => "asc")
			,
			"attributes" => array(
				"group_id" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"maxlen" => 64,
					"charset" => CHARSET_TESTID,
					"sortoption" => true,
					"filteroption" => true,
					"readonly" => true
					)
				,
				"assembly_id" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"maxlen" => 64,
					"charset" => CHARSET_TESTID,
					"sortoption" => true,
					"filteroption" => true,
					"readonly" => true
					)
				,
				"site_path" => array(
					"webserviceparam" => true, 
					"type" => "string",
					"maxlen" => 64,
					"charset" => CHARSET_TESTID,
					"sortoption" => true,
					"filteroption" => true,
					"readonly" => true
					)
				)
			);

		
		return $arrayEntityDefinition;
	}
	
	
