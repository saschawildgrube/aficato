<?php
	
	
	function GetEntityDefinitionSession()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "session_id",
			"default" => true,
			"indexstate" => "list",
			"deletewithoutconfirmation" => true,
			"userfriendlyattribute" => "session_id", 
			"blocksize" => 10,
			"dbtablename" => "System-Session",
			"noitemsfoundmessage" => "?TID_SESSION_INFO_NO_SESSIONS_FOUND?",
			"webservice" => array(
				"name" => "system/session",
				"itemtag" => "SESSION",
				"newitemidtag" => "NEW_SESSION_ID",
				"commands" => array(
					"add" => array(
						"name" => "create",
						"additionalparams" => array("owner_id","duration","user_ip")
						),
					"delete" => array(
						"name" => "destroy",
						"additionalparams" => array()
						),
					"get" => array(
						"name" => "read",
						"additionalparams" => array()
						),
					"set" => array(
						"name" => "write",
						"additionalparams" => array("owner_id")
						),
					"list" => array(
						)
					)
				)
			,
			"list" => array(
				"refresh" => true)
			,
			"view" => array(
				"refresh" => true)
			,
			"sorting" => array(
				"defaultattribute" => "creation_datetime",
				"defaultorder" => "desc")
			,
			"states" => array(
				"list",
				"view",
				"delete")
			,
			"tasks" => array(
				"view" => array(
					"state" => "view",
					"icon" => "view")
				,
				"delete" => array(
					"state" => "delete",
					"icon" => "delete")
				)
			,
			"attributes" => array( 
				"session_id" => array(
					"webserviceparam" => true,
					"type" => "string",
					"minlen" => 40,
					"maxlen" => 40,
					"sharecolumn" => true,
					"readonly" => true,
					"indexlink" => false)
				,
				"owner_id" => array(
					"webserviceparam" => true,
					"type" => "string",
					"maxlen" => 32,
					"readonly" => true,
					"indexlink" => false)
				,
				"creation_datetime" => array(
					"webserviceparam" => true,
					"sharecolumn" => true,
					"readonly" => true,
					"sortoption" => true,
					"type" => "datetime")
				,
				"end_datetime" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"readonly" => true,
					"type" => "datetime")
				,
				"lastaccess_datetime" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"sortoption" => true,
					"type" => "datetime")
				,
				"data" => array(
					"webserviceparam" => true,
					"type" => "string",
					"maxlen" => ENTITY_STRINGLENGTH_MAX,
					"hideinlist" => true)
				,
				"user_id_authentication" => array(
					"type" => "numericid",
					"readonly" => true,
					"sharecolumn" => true)
				,
				"user_ip" => array(
					"webserviceparam" => true,
					"type" => "string",
					"maxlen" => 15,
					"charset" => CHARSET_IPV4)
			)		
		);
		
		return $arrayEntityDefinition;
	}

	
