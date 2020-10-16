<?php

	require_once(GetWDKDir()."wdk_language.inc");
	require_once(GetWDKDir()."wdk_country.inc");

	function GetUserEmailValidationStatusEnum()
	{
		$arrayStatus = array(
			0 => "NOTVALIDATED",
			1 => "VALIDATION_SIGNIN",
			2 => "VALIDATED",
			3 => "VALIDATION_INITIAL",
			4 => "VALIDATION_CHANGE"
			);
		return $arrayStatus;
	}

	
	
	function GetEntityDefinitionUser()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "user_id",
			"default" => true,
			"indexstate" => "view",
			"userfriendlyattribute" => "user_name", 
			"blocksize" => 10,
			"noitemsfoundmessage" => "?TID_USER_INFO_NOUSERSFOUND?",
			"dbtablename" => "System-User",
			"uniqueindices" => array(
				"unique" => array("user_name","silo_id")
				),
			"webservice" => array(
				"name" => "system/user",
				"itemtag" => "USER",
				"newitemidtag" => "NEW_USER_ID",
				"commands" => array(
					"list" => array(
						"name" => "list",
						"additionalparams" => array("silo_id")
						),
					"add" => array( 
						"name" => "add",
						"additionalparams" => array("ignoreblacklist","user_email_validation","user_email_validation_duration")
						),
					"get" => array(
						"name" => "get",
						"additionalparams" => array("silo_id","user_name")
						),
					"set" => array(
						"name" => "set"
						),
					"delete" => array(
						"name" => "delete",
						"additionalparams" => array("silo_id","user_name")
						)
					)
				)
			,
			"view" => array(
				"style" => "vertical",
				"hideheader" => true)
			,
			"sorting" => array(
				"defaultattribute" => "user_name",
				"defaultorder" => "asc")
			,
			"filteroptions" => array(
				"submitbuttontext" => "?TID_USER_BUTTON_FILTER?",
				"reset" => 1,
				"reseticon" => "undo",
				"resettooltip" => "?TID_USER_TOOLTIP_RESETFILTERS?",
				"formstyle" => "horizontal",
				"formid" => "",
				"filters" => array(
					"searchbyname" => array(
						"webservicefilteroption" => "user_name",
						"label" => "?TID_USER_LABEL_SEARCH?",
						"type" => "search"),
					"filterbyactive" => array(
						"webservicefilteroption" => "user_active",
						"label" => "?TID_USER_LABEL_FILTERBYACTIVE?",
						"type" => "select"),
					"filterbylanguage" => array(
						"webservicefilteroption" => "user_language",
						"label" => "?TID_USER_LABEL_FILTERBYLANGUAGE?",
						"type" => "select"
						)
					)
				)
			,
			"states" => array(
				"add",
				"view",
				"modify",
				"delete")
			,
			"tasks" => array(
				"view" => array(
					"state" => "view",
					"icon" => "view"
					)
				,
				"modify" => array(
					"state" => "modify",
					"icon" => "modify"
					)
				, 
				"delete" => array(
					"state" => "delete",
					"icon" => "delete"
					)
				)
			,
			"tabs" => array(
				"local" => "?TID_USER_TAB_LOCAL?",
				"validation" => "?TID_USER_TAB_VALIDATION?",
				"history" => "?TID_USER_TAB_HISTORY?"
				)
			,
			"attributes" => array( 
				"silo_id" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"filteroption" => false,
					"type" => "numericid",
					"readonly" => false,
					"hideinlist" => true,
					"hideinview" => true,
					"readonlyfrontend" => true)
				,
				"user_id" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"sharecolumn" => true,
					"type" => "numericid",
					"readonly" => true,
					"hideinlist" => false,
					"hideinview" => false,
					"indexlink" => false)
				,
				"user_name" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"filteroption" => true,
					"type" => "string",
					"minlen" => 1,
					"maxlen" => 255,
					"indexlink" => true)
				,
				"user_active" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"type" => "bool",
					"default" => true,
					"sharecolumn" => false
					)
				,					
				"password" => array(
					"webserviceparam" => true,
					"type" => "password",
					"hideinlist" => true,
					"hideinview" => true,
					"excludefromget" => true,
					"nostorage" => true)
				,
				"passwordhash" => array(
					"webserviceparam" => true,
					"type" => "string",
					"maxlen" => 40,
					"minlen" => 40,
					"charset" => CHARSET_HEX,
					"hideinlist" => true,
					"hideinview" => true,
					"excludefromget" => true,
					"readonlyfrontend" => true)
				,
				"passwordhash_seed" => array(
					"webserviceparam" => true,
					"type" => "string",
					"maxlen" => 40,
					"minlen" => 40,
					"charset" => CHARSET_HEX,
					"hideinlist" => true,
					"hideinview" => true,
					"readonlyfrontend" => true)
				,
				"user_email" => array(
					"webserviceparam" => true,
					"type" => "email",
					//"maxlen" => 255,
					"hideinlist" => true)
				,
				"user_country" => array(
					"webserviceparam" => true,
					"sharecolumn" => true,
					"type" => "enum",
					"dbtype" => "string",
					"enum" => ArrayAppend(array(""),GetAllCountryIDs()),
					"tidpattern" => "?TID_COUNTRY_{VALUE}?",
					"control" => "select",
					"tab" => "local")
				,
				"user_language" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"type" => "language",
					"control" => "select",
					"inputrequired" => false,
					"tab" => "local")
				,
				"user_timezone" => array(
					"webserviceparam" => true,
					"type" => "enum",
					"dbtype" => "string",
					"enum" => GetAllTimezoneIDs(),
					"tidpattern" => "{VALUE}",
					"default" => "UTC",
					"control" => "select",
					"tab" => "local")
				,
				"user_email_validation_status" => array(
					"type" => "enum",
					"enum" => GetUserEmailValidationStatusEnum(),
					"tidpattern" => "{VALUE}",
					"default" => "NOTVALIDATED",
					"readonly" => true,
					"hideinlist" => true,
					"tab" => "validation")
				,
				"user_email_validation_token" => array(
					"webserviceparam" => true,
					"type" => "string",
					"charset" => CHARSET_INTEGER,
					"maxlen" => 10,
					"minlen" => 10,
					"readonly" => true,
					"hideinlist" => true,
					"tab" => "validation")
				,
				"user_email_validation_end_datetime" => array(
					"type" => "datetime",
					"readonly" => true,
					"hideinlist" => true,
					"tab" => "validation")
				,
				"user_email_validation_new" => array(
					"type" => "string",
					"maxlen" => 255,
					"readonly" => true,
					"hideinlist" => true,
					"tab" => "validation")
				,				
				"creation_datetime" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"sharecolumn" => true,
					"type" => "datetime",
					"readonly" => true,
					"tab" => "history")	
				,
				"lastchange_datetime" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"type" => "datetime",
					"readonly" => true,
					"tab" => "history")
				,
				"lastauthentication_datetime" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"type" => "datetime",
					"readonly" => true,
					"tab" => "history")
				)		
		);
		
		return $arrayEntityDefinition;
	}

	
