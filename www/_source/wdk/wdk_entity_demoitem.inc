<?php
	
	
	function GetEntityDefinitionDemoitem()
	{
			$arrayEntityDefinition = array(
				"indexattribute" => "item_id",
				"default" => true,
				"indexstate" => "view",
				"userfriendlyattribute" => "item_name", 
				"blocksize" => 10,
				"maxblocksize" => 100,
				"dbtablename" => "Demo-Item", 
				"noitemsfoundmessage" => "?TID_DEMOITEM_INFO_NOITEMSFOUND?",
				"showreadonlyinmodify" => true,
				//"deletewithoutconfirmation" = true,
				"webservice" => array(
					"name" => "demo/databasedemo",
					"itemtag" => "ITEM",
					"newitemidtag" => "NEW_ITEM_ID",
					"commands" => array(
						"list" => array(),
						"add" => array(),
						"set" => array(),
						"get" => array(),
						"delete" => array()
						)
					)
				,
				"sorting" => array(
					"defaultattribute" => "item_name",
					"defaultorder" => "asc")
				,
				"filteroptions" => array(
					"submitbuttontext" => "?TID_DEMOITEM_BUTTON_FILTER?",
					"reset" => 1,
					"reseticon" => "undo",
					"resettooltip" => "?TID_TOOLTIP_RESETFILTERS?",
					"formstyle" => "horizontal",
					"formid" => "",
					//"webserviceparamprefix" => "",
					"filters" => array(
						"searchbyname" => array(
							"webservicefilteroption" => "item_name",
							"label" => "?TID_DEMOITEM_LABEL_SEARCH?",
							"type" => "search")
						)
					)
				,
				"states" => array(
					"add",
					"view",
					"modify",
					"modifysortorder",
					"delete")
				,
				"tasks" => array(
					"view" => array(
						"state" => "view",
						"icon" => "view",
						"tooltip" => "?TID_DEMOITEM_TOOLTIP_VIEW?")
					,
					"modify" => array(
						"state" => "modify",
						"icon" => "modify",
						"tooltip" => "?TID_DEMOITEM_TOOLTIP_MODIFY?")
					, 
					"delete" => array(
						"state" => "delete",
						"icon" => "delete",
						"tooltip" => "?TID_DEMOITEM_TOOLTIP_DELETE?")
					)
				,
				"tabs" => array(
					"tab1" => "?TID_DEMOITEM_TAB_CREATION?",
					"tab2" => "?TID_DEMOITEM_TAB_LASTCHANGE?"
					)
				,
				"attributes" => array( 
					"item_id" => array(
						"webserviceparam" => true,
						"sortoption" => true,
						"type" => "numericid",
						"readonly" => true,
						"hideinlist" => true,
						"hideinview" => true,
						"indexlink" => false)
					,
					"item_sortorder" => array(
						"webserviceparam" => true,
						"sortoption" => true,
						"type" => "sortorder")
					,
					"item_name" => array(
						"webserviceparam" => true,
						"sortoption" => true,
						"filteroption" => true,
						"type" => "string",
						"maxlen" => 40,
						"minlen" => 3,
						"indexlink" => true)
					,
					"item_data" => array(
						"webserviceparam" => true,
						"type" => "string",
						"control" => "textarea",
						"maxlen" => 2000,
						"tableheader" => "",
						"hideinlist" => true,
						"hideinview" => false,
						"indexlink" => false)
					,
					"creation_datetime" => array(
						"webserviceparam" => true,
						"sortoption" => true,
						"type" => "datetime",
						"formlabel" => "",
						"readonly" => true,
						"tab" => "tab1")	
					,
					"lastchange_datetime" => array(
						"webserviceparam" => true,
						"sortoption" => true,
						"type" => "datetime",
						"readonly" => true,
						"tab" => "tab2")
				)		
			);
		
		return $arrayEntityDefinition;
	}

	
