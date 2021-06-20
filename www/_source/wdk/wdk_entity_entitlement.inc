<?php
	
	
	function GetEntityDefinitionEntitlement()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "entitlement_id",
			"default" => true,
			"userfriendlyattribute" => "privilege_id", 
			"maxblocksize" => 1000,
			"noitemsfoundmessage" => "?TID_ENTITLEMENT_INFO_NOENTITLEMENTSFOUND?",
			"dbtablename" => "System-Entitlement",
			"webservice" => array(
				"name" => "system/entitlement",
				"itemtag" => "ENTITLEMENT",
				"commands" => array(
					"list" => array(),
					"get" => array(
						"additionalparams" => array("user_id")
						),
					"set" => array(),
					"delete" => array(
						"additionalparams" => array("user_id","privilege_id")
						)
					)
				)
			,
			"sorting" => array(
				"defaultattribute" => "privilege_id",
				"defaultorder" => "asc")
			,
			/*
			"filteroptions" => array(
				"filters" => array(
					"user_id" => array(
						"webservicefilteroption" => "user_id",
						"type" => "search")
					)
				)
			,
			*/
			/*
			"states" => array(
				"add",
				"view",
				"modify",
				"delete")
			,
			*/
			/*
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
			*/
			"attributes" => array( 
				"user_id" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "numericid",
					"hideinlist" => true
					)
				,
				"privilege_id" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "string",
					"maxlen" => 50)
				,
				"entitlement_id" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"sortoption" => true,
					"type" => "numericid",
					"readonly" => true)
			)		
		);
		
		return $arrayEntityDefinition;
	}

	
