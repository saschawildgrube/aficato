<?php
	
	
	function GetEntityDefinitionJob()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "job_id",
			"default" => true,
			"indexstate" => "modify",
			"userfriendlyattribute" => "job_name", 
			"blocksize" => 10,
			"dbtablename" => "System-Scheduler",
			"noitemsfoundmessage" => "?TID_INFO_NOJOBSFOUND?",
			"webservice" => array(
				"name" => "system/scheduler",
				"itemtag" => "JOB",
				"newitemidtag" => "NEW_JOB_ID",
				"commands" => array(
					"list" => array(),
					"add" => array(),
					"get" => array(),
					"set" => array(),
					"delete" => array()
					)
				)
			,
			"sorting" => array(
				"defaultattribute" => "job_name",
				"defaultorder" => "asc")
			,
			/*
			"filteroptions" => array(
				"submitbuttontext" => "?TID_USERMANAGER_BUTTON_FILTER?",
				"reset" => 1,
				"reseticon" => "undo",
				"resettooltip" => "?TID_TOOLTIP_RESETFILTERS?",
				"formstyle" => "horizontal",
				"formid" => "",
				"filters" => array(
					"searchbyname" => array(
						"webservicefilteroption" => "username",
						"label" => "?TID_USERMANAGER_LABEL_SEARCH?",
						"type" => "search")
					)
				)
			,
			*/
			"states" => array(
				"add",
				"modify",
				"delete")
			,
			"list" => array(
				"refresh" => true)
			,
			"tasks" => array(
				"modify" => array(
					"state" => "modify",
					"icon" => "modify")
				, 
				"delete" => array(
					"state" => "delete",
					"icon" => "delete")
				)
			,
			"attributes" => array( 
				"job_id" => array(
					"webserviceparam" => true,
					"sharecolumn" => true,
					"sortoption" => true,
					"type" => "numericid",
					"readonly" => true,
					"indexlink" => false)
				,
				"job_name" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "string",
					"minlen" => 1,
					"maxlen" => 255,
					"indexlink" => true)
				,
				"lastexecution_datetime" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"sharecolumn" => true,
					"type" => "datetime",
					"readonly" => true)
				, 
				"nextexecution_datetime" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"type" => "datetime",
					"readonly" => true)
				,
				"creation_datetime" => array(
					"webserviceparam" => true,	
					"sharecolumn" => true,
					"sortoption" => true,
					"type" => "datetime",
					"readonly" => true)	
				,
				"lastchange_datetime" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"type" => "datetime",
					"readonly" => true)
				,
				"job_url" => array(
					"webserviceparam" => true,
					"sharecolumn" => true,
					"type" => "string",
					"charset" => CHARSET_URL,
					"minlen" => 3, // because the url could be a configured web service name like "a/b"
					"maxlen" => 255
					)
				,
				"job_postparams" => array(
					"webserviceparam" => true,
					"hideinlist" => true,
					"type" => "string",
					"control" => "textarea",
					"charset" => CHARSET_URL."\n\r",
					"minlen" => 0,
					"maxlen" => 2000
					)
				,				
				"timeout_seconds" => array(
					"webserviceparam" => true,
					"type" => "integer",
					"max" => 180,
					"zerounset" => true
					)
				,						
				"job_active" => array(
					"webserviceparam" => true,
					"type" => "bool",
					"hideinlist" => true
					)
				,
				"schedule_minute" => array(
					"webserviceparam" => true,
					"type" => "string",
					"minlen" => 1,
					"maxlen" => 200,
					"charset" => CHARSET_CRONTAB,
					"hideinlist" => true)
				,
				"schedule_hour" => array(
					"webserviceparam" => true,					
					"type" => "string",
					"minlen" => 1,
					"maxlen" => 200,
					"charset" => CHARSET_CRONTAB,
					"hideinlist" => true)
				,
				"schedule_dayofmonth" => array(
					"webserviceparam" => true,
					"type" => "string",
					"minlen" => 1,
					"maxlen" => 200,
					"charset" => CHARSET_CRONTAB,
					"hideinlist" => true)
				,
				"schedule_month" => array(
					"webserviceparam" => true,
					"type" => "string",
					"minlen" => 1,					
					"maxlen" => 200,
					"charset" => CHARSET_CRONTAB,
					"hideinlist" => true)
				,
				"schedule_dayofweek" => array(
					"webserviceparam" => true,
					"type" => "string",
					"minlen" => 1,					
					"maxlen" => 200,
					"charset" => CHARSET_CRONTAB,
					"hideinlist" => true)
			)		
		);
		
		return $arrayEntityDefinition;
	}

	
