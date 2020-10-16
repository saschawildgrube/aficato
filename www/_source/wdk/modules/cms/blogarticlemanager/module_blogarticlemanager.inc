<?php

	require_once(GetWDKDir()."modules/cms/articlemanager/module_articlemanager.inc");
	
	class CBlogArticleManagerModule extends CArticleManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"blog".$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$bResult = parent::CallbackInit();
			if ($bResult == false)
			{
				return false;	
			}			
			$arrayEntityDefinitions = $this->GetEntityDefinitions();

			unset($arrayEntityDefinitions["article"]["filteroptions"]["filters"]["filterbytype"]);
			//unset($arrayEntityDefinitions["article"]["filteroptions"]["filters"]["filterbycountry"]);
			//unset($arrayEntityDefinitions["article"]["filteroptions"]["filters"]["filterbylanguage"]);
			unset($arrayEntityDefinitions["article"]["filteroptions"]["filters"]["searchforname"]);
			//unset($arrayEntityDefinitions["article"]["filteroptions"]["filters"]["filterbytextformat"]);
			
			$arrayEntityDefinitions["article"]["attributes"]["type"]["hideinlist"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["type"]["hideinview"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["type"]["readonly"] = true;


			$arrayEntityDefinitions["article"]["sorting"]["defaultattribute"] = "creation_datetime";

			$arrayEntityDefinitions["article"]["attributes"]["articleversion_id"]["indexlink"] = false;
			
			$arrayEntityDefinitions["article"]["attributes"]["title"]["indexlink"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["title"]["sortoption"] = true;
			
			/*
			$arrayEntityDefinitions["article"]["attributes"]["textformat"]["hideinlist"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["textformat"]["hideinview"] = true;
			//$arrayEntityDefinitions["article"]["attributes"]["textformat"]["readonlyfrontend"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["textformat"]["default"] = "WIKI";
			$arrayEntityDefinitions["article"]["attributes"]["textformat"]["enum"] = array("WIKI");
			*/

			$arrayEntityDefinitions["article"]["attributes"]["name"]["readonly"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["name"]["hideinlist"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["name"]["hideinview"] = true;
 
			$arrayEntityDefinitions["article"]["attributes"]["reference_articleversion_id"]["readonlyfrontend"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["reference_articleversion_id"]["hideinlist"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["reference_articleversion_id"]["hideinview"] = true;
			
			$arrayEntityDefinitions["article"]["attributes"]["articleversion_id"]["hideinlist"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["articleversion_id"]["hideinview"] = true;
			
			
			/*
			$arrayEntityDefinitions["article"]["attributes"]["language"]["hideinlist"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["language"]["hideinview"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["language"]["readonlyfrontend"] = true;

			$arrayEntityDefinitions["article"]["attributes"]["country"]["hideinlist"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["country"]["hideinview"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["country"]["readonlyfrontend"] = true;
			*/

			$this->SetEntityDefinitions($arrayEntityDefinitions);
			
			return true;
		}
		
		function CallbackPrepareWebserviceParams($strEntity,&$arrayParams)
		{
			if ($strEntity == "article")
			{
				if (ArrayGetValue($arrayParams,"command") == "set")
				{
					$arrayParams["type"] = "BLOG";	
				}
				if (ArrayGetValue($arrayParams,"command") == "add")
				{
					$arrayParams["type"] = "BLOG";	
				}
				if (ArrayGetValue($arrayParams,"command") == "list")
				{
					$arrayParams["filter20"] = "type";
					$arrayParams["filter20_operator"] = "=";	
					$arrayParams["filter20_value"] = "BLOG";
				}
							
			}
			parent::CallbackPrepareWebserviceParams($strEntity,$arrayParams);
		}

		
		function CallbackCheckConditionForItem($strEntity,$arrayItem,$strState)
		{
			if ($strEntity == "article")	
			{
				$strType = ArrayGetValue($arrayItem,"type");
				if ($strType != "BLOG")
				{
					return $this->StateList($strEntity);
				}
			}
		}
		
	}

