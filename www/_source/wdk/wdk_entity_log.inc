<?php
	
	require_once(GetWDKDir()."wdk_eventseverity.inc");
	
	
	function GetEntityDefinitionLog()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "log_id",
			"default" => true,
			"indexstate" => "view", 
			"userfriendlyattribute" => "log_id", 
			"blocksize" => 10,
			"maxblocksize" => 100,
			"dbtablename" => "System-Log",
			"noitemsfoundmessage" => "?TID_LOG_INFO_NOITEMSFOUND?",
			"webservice" => array(
				"name" => "system/log",
				"itemtag" => "LOG",
				"newitemidtag" => "NEW_LOG_ID",
				"commands" => array(
					"list" => array(),
					"get" => array()
					)
				)
			,
			"view" => array(
				"hideheader" => true)
			,
			"list" => array(
				"refresh" => true
				)
			,			
			"sorting" => array(
				"defaultattribute" => "log_id",
				"defaultorder" => "desc")
			,
			"filteroptions" => array(
				"submitbuttontext" => "?TID_LOG_BUTTON_FILTER?",
				"reset" => 1,
				"reseticon" => "undo",
				"resettooltip" => "?TID_LOG_TOOLTIP_RESETFILTERS?",
				"formstyle" => "horizontal",
				"formid" => "",
				"filters" => array(
					"filterbyseverity" => array(
						"webservicefilteroption" => "event_severity",
						"label" => "?TID_LOG_LABEL_FILTERBYSEVERITY?",
						"type" => "select")
					,
					"searchforeventid" => array(
						"webservicefilteroption" => "event_id",
						"label" => "?TID_LOG_LABEL_SEARCHFOREVENTID?",
						"type" => "search")
					,
					"searchinmessage" => array(
						"webservicefilteroption" => "event_details",
						"label" => "?TID_LOG_LABEL_SEARCHINMESSAGE?",
						"type" => "search")
					,
					"searchindetails" => array(
						"webservicefilteroption" => "event_details",
						"label" => "?TID_LOG_LABEL_SEARCHINDETAILS?",
						"type" => "search")
					)
				)
			,
			"states" => array(
				"list",
				"export",
				"view"
				)
			,
			"tasks" => array(
				"view" => array(
					"state" => "view",
					"icon" => "view"
				)
			)
			,
			"attributes" => array( 
				"log_id" => array(
					"webserviceparam" => true,
					"type" => "numericid",
					"sortoption" => true,
					"filteroption" => true,
					"readonly" => true,
					"indexlink" => true,
					"sharecolumn" => true)
				,
				"log_datetime" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"sortoption" => true,
					"type" => "datetime")
				,
				"reporter_id" => array(
					"webserviceparam" => true,
					"maxlen" => 32,
					"minlen" => 1,
					"charset" => CHARSET_TEXTID,
					"readonly" => true,
					"filteroption" => true,
					"type" => "string",
					"sharecolumn" => true)
				,
				"event_severity" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"filteroption" => true,
					"enum" => GetEventSeverities(),
					"type" => "enum",
					"tidpattern" => "{VALUE}",
					"default" => "INFORMATIONAL")
				,
				"event_id" => array(
					"webserviceparam" => true,
					"maxlen" => 32,
					"minlen" => 1,
					"charset" => CHARSET_TEXTID,
					"readonly" => true,
					"filteroption" => true,
					"type" => "string",
					"sharecolumn" => true)
				,
				"event_datetime" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"sortoption" => true,
					"type" => "datetime")
				,
				"event_message" => array(
					"webserviceparam" => true,
					"maxlen" => 200,
					"minlen" => 0,
					"readonly" => true,
					"filteroption" => true,
					"type" => "string",
					"hideinlist" => false)
				,
				"event_details" => array(
					"webserviceparam" => true,
					"maxlen" => 65535,
					"minlen" => 0,
					"readonly" => true,
					"filteroption" => true,
					"type" => "string",
					"hideinlist" => true)
				,
				"hash" => array(
					"type" => "string",
					"maxlen" => 40,
					"hideinlist" => true,
					"readonly" => true  
					)
				,
				"hashchain" => array(
					"type" => "string",
					"maxlen" => 40,
					"hideinlist" => true,
					"readonly" => true  
					)
			)		
		);
		
		return $arrayEntityDefinition;
	}

	
