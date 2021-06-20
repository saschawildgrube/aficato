<?php
	
	
	function GetEntityDefinitionDataitem()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "data_id",
			"default" => true,
			"indexstate" => "view",
			"userfriendlyattribute" => "data_name", 
			"showreadonlyinmodify" => true,
			"blocksize" => 10,
			"noitemsfoundmessage" => "?TID_DATAITEM_INFO_NOITEMSFOUND?",
			"dbtablename" => "System-Data",
			"uniqueindices" => array(
				"unique" => array("data_name","data_context_type","data_context_id")
				)
			,
			"webservice" => array(
				"name" => "system/data",
				"itemtag" => "DATA",
				"newitemidtag" => "NEW_DATA_ID"
				,
				"commands" => array(
					"list" => array(), 
					"set" => array(), 
					"get" => array(
						"additionalparams" => array("data_name","data_context_type","data_context_id")),
					"delete" => array(
						"additionalparams" => array("data_name","data_context_type","data_context_id"))
						
					)
				)
			,
			"view" => array(
				"hideheader" => true,
				"refresh" => true
				)
			,
			"list" => array(
				"refresh" => true
				)
			,
			"sorting" => array(
				"defaultattribute" => "data_name",
				"defaultorder" => "asc")
			,
			"filteroptions" => array(
				"submitbuttontext" => "?TID_DATAITEM_BUTTON_FILTER?",
				"reset" => 1,
				"reseticon" => "undo",
				"resettooltip" => "?TID_TOOLTIP_RESETFILTERS?",
				"formstyle" => "horizontal",
				"formid" => "",
				"filters" => array(
					"searchbydataname" => array(
						"webservicefilteroption" => "data_name",
						"label" => "?TID_DATAITEM_LABEL_SEARCH_DATA_NAME?",
						"type" => "search"),
					"searchbydatacontent" => array(
						"webservicefilteroption" => "data_content",
						"label" => "?TID_DATAITEM_LABEL_SEARCH_DATA_CONTENT?",
						"type" => "search"),
					"searchbycontexttype" => array(
						"webservicefilteroption" => "data_context_type",
						"label" => "?TID_DATAITEM_LABEL_SEARCH_DATA_CONTEXT_TYPE?",
						"type" => "search")
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
					"icon" => "view")
				,
				"modify" => array(
					"state" => "modify",
					"icon" => "modify")
				, 
				"delete" => array(
					"state" => "delete",
					"icon" => "delete")
				)
			,
			"tabs" => array(
				"content" => "?TID_DATAITEM_TAB_DATA_CONTENT?",
				"history" => "?TID_DATAITEM_TAB_HISTORY?"
				)
			,
			"attributes" => array( 
				"data_id" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "numericid",
					"readonly" => true,
					"indexlink" => true,
					"sharecolumn" => false)
				,
				"data_context_type" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "string",
					"maxlen" => 64,
					"indexlink" => false,
					"sharecolumn" => true)
				,
				"data_context_id" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "string",
					"maxlen" => 64,
					"indexlink" => false)
				,
				"data_name" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "string",
					"maxlen" => 64,
					"indexlink" => true)
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
				"data_content" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"type" => "string",
					"control" => "textarea",
					"maxlen" => ENTITY_STRINGLENGTH_MAX,
					"indexlink" => false,
					"tab" => "content")
			)
		);
		
		return $arrayEntityDefinition;
	}

	
