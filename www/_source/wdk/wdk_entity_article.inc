<?php

	require_once(GetWDKDir()."wdk_entitydefinitions.inc");
	
	define("ARTICLE_MAXLEN",ENTITY_STRINGLENGTH_MAX);
			
	function GetArticleStatusEnum()
	{
		$arrayStatus = array(
			0 => "DRAFT",
			1 => "ACTIVE",
			2 => "INACTIVE"
			);
		return $arrayStatus;
	}
	
	function GetArticleTypeEnum()
	{
		$arrayStatus = array(
			0 => "STATIC",
			1 => "NEWS",
			2 => "GLOSSARY",
			3 => "BLOG",
			4 => "POST",
			5 => "COMMENT",
			6 => "BOOK",
			7 => "TEASER"
			);
		return $arrayStatus;
	}	
	
	function GetArticleTextFormatEnum()
	{
		$arrayFormat = array(
			0 => "PLAIN",
			1 => "HTML",
			2 => "WIKI",
			3 => "BBCODE"
			);
		return $arrayFormat;
	}		
		
	function GetEntityDefinitionArticle()
	{
		$arrayEntityDefinition = array(
			"indexattribute" => "articleversion_id",
			"default" => true,
			"indexstate" => "view", 
			"userfriendlyattribute" => "title", 
			"blocksize" => 10,
			"maxblocksize" => 100,
			"dbtablename" => "CMS-Article",
			"noitemsfoundmessage" => "?TID_ARTICLE_INFO_NOITEMSFOUND?",
			"webservice" => array(
				"name" => "cms/article",
				"itemtag" => "ARTICLE",
				"newitemidtag" => "NEW_ARTICLEVERSION_ID" 
				,
				"commands" => array(
					"list" => array(),
					"get" => array(
						"additionalparams" => array("name","title","language","country","type","update_lastaccess")),
					"add" => array(
						"name" => "set"),
					"set" => array(),
					"delete" => array(
						"additionalparams" => array("name","title"))
					)
				)
			,
			"sorting" => array(
				"defaultattribute" => "name",
				"defaultorder" => "desc")
			,
			"filteroptions" => array(
				"submitbuttontext" => "?TID_ARTICLE_BUTTON_FILTER?",
				"reset" => 1,
				"reseticon" => "undo",
				"resettooltip" => "?TID_ARTICLE_TOOLTIP_RESETFILTERS?",
				"formstyle" => "horizontal",
				"formid" => "",
				"filters" => array(
					"filterbytype" => array(
						"webservicefilteroption" => "type",
						"label" => "?TID_ARTICLE_LABEL_FILTERBYTYPE?",
						"type" => "select")
					,
					"filterbystatus" => array(
						"webservicefilteroption" => "status",
						"label" => "?TID_ARTICLE_LABEL_FILTERBYSTATUS?",
						"type" => "select")
					,
					"filterbylanguage" => array(
						"webservicefilteroption" => "language",
						"label" => "?TID_ARTICLE_LABEL_FILTERBYLANGUAGE?",
						"type" => "select")
					,
					"filterbycountry" => array(
						"webservicefilteroption" => "country",
						"label" => "?TID_ARTICLE_LABEL_FILTERBYCOUNTRY?",
						"type" => "select")
					,
					"searchforname" => array(
						"webservicefilteroption" => "name",
						"label" => "?TID_ARTICLE_LABEL_SEARCHFORNAME?",
						"type" => "search",
						"separator" => true)
					,
					"searchfortitle" => array(
						"webservicefilteroption" => "title",
						"label" => "?TID_ARTICLE_LABEL_SEARCHFORTITLE?",
						"type" => "search")
					,
					"searchforcontent" => array(
						"webservicefilteroption" => "content",
						"label" => "?TID_ARTICLE_LABEL_SEARCHFORCONTENT?",
						"type" => "search")
					,
					"filterbytextformat" => array(
						"webservicefilteroption" => "textformat",
						"label" => "?TID_ARTICLE_LABEL_FILTERBYTEXTFORMAT?",
						"type" => "select")
					,
					
					)
				)
			,
			"states" => array(
				"list",
				"add",
				"delete",
				"view",
				"modify",
				"export",
				"import"				
				)
			,
			"tasks" => array(
				"view" => array(
					"state" => "view",
					"icon" => "view"
				),
				"modify" => array(
					"state" => "modify",
					"icon" => "modify"
				),
				"delete" => array(
					"state" => "delete",
					"icon" => "delete"
				)
			)
			,
			"tabs" => array(
				"timing" => "?TID_ARTICLE_TAB_TIMING?",
				"history" => "?TID_ARTICLE_TAB_HISTORY?"
				)
			,
			"attributes" => array( 
				"articleversion_id" => array(
					"webserviceparam" => true,
					"type" => "numericid",
					"sortoption" => true,
					"readonly" => true,
					"indexlink" => true,
					"sharecolumn" => true
					)
				,
				"name" => array(
					"webserviceparam" => true,
					"type" => "string", 
					"minlen" => 1,
					"maxlen" => 128,
					"charset" => CHARSET_TEXTID,
					"sortoption" => true,
					"filteroption" => true,
					"readonly" => false
					)
				,
				"type" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"enum" => GetArticleTypeEnum(),
					"default" => "STATIC",
					"control" => "select",
					"hidenooption" => true,
					"type" => "enum",
					"sharecolumn" => true
					)
				,					
				"textformat" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"enum" => GetArticleTextFormatEnum(),
					"default" => "PLAIN",
					"control" => "select",
					"type" => "enum",
					"hidenooption" => true)
				,  
				"reference_articleversion_id" => array(
					"webserviceparam" => true,
					"type" => "numericid",
					"filteroption" => true,
					"readonly" => false
					)
				,
				"creation_datetime" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"sortoption" => true,
					"type" => "datetime",
					"tab" => "history",
					"sharecolumn" => true)
				,
				"lastaccess_datetime" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"sortoption" => true,
					"type" => "datetime",
					"tab" => "history",
					"sharecolumn" => true)
				,
				"lastchange_datetime" => array(
					"webserviceparam" => true,
					"readonly" => true,
					"sortoption" => true,
					"type" => "datetime",
					"tab" => "history")
				,
				"status" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"enum" => GetArticleStatusEnum(),
					"default" => "DRAFT",
					"control" => "select",
					"type" => "enum",
					"sharecolumn" => true)
				,					
				"author_user_id" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"readonlyfrontend" => true,
					"type" => "numericid")
				,
				"publication_start_datetime" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"filteroption" => true,
					"sharecolumn" => true,
					"type" => "datetime",
					"tab" => "timing")
				,
				"publication_end_datetime" => array(
					"webserviceparam" => true,
					"sortoption" => true,
					"filteroption" => true,
					"type" => "datetime",
					"tab" => "timing"/*,
					"default" => DATETIME_INFINITE*/)
				,
				"language" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"type" => "language",
					"sharecolumn" => true)
				,
				"country" => array(
					"webserviceparam" => true,
					"filteroption" => true,
					"minlen" => 0,
					"maxlen" => 3,
					"type" => "string",
					"control" => "select")
				,
				"title" => array(
					"webserviceparam" => true,
					"readonly" => false,
					"filteroption" => true,
					"minlen" => 1,
					"maxlen" => 128,
					"type" => "string",
					"hideinlist" => false)
				,
				"content" => array(
					"webserviceparam" => true,
					"readonly" => false,
					"filteroption" => true,
					"minlen" => 0,
					"maxlen" => ARTICLE_MAXLEN,
					"type" => "string",
					"hideinlist" => true,
					"hideinview" => true,
					"control" => "textarea")
			)		
		);
		
		return $arrayEntityDefinition;
	}

	
