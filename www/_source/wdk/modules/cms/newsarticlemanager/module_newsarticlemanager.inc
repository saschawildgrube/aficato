<?php

	require_once(GetWDKDir()."modules/cms/articlemanager/module_articlemanager.inc");
	
	class CNewsArticleManagerModule extends CArticleManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"news".$strInstanceID,
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
			$arrayEntityDefinitions["article"]["attributes"]["type"]["hideinlist"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["type"]["hideinview"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["type"]["readonly"] = true;
			$arrayEntityDefinitions["article"]["attributes"]["name"]["readonly"] = true;
			
			$this->SetEntityDefinitions($arrayEntityDefinitions);
			
			return true;
		}
		
		function CallbackPrepareWebserviceParams($strEntity,&$arrayParams)
		{
			if ($strEntity == "article")
			{
				if (ArrayGetValue($arrayParams,"command") == "set")
				{
					$arrayParams["type"] = "NEWS";	
				}
				if (ArrayGetValue($arrayParams,"command") == "list")
				{
					$arrayParams["filter20"] = "type";
					$arrayParams["filter20_operator"] = "=";	
					$arrayParams["filter20_value"] = "NEWS";
				}
							
			}
			parent::CallbackPrepareWebserviceParams($strEntity,$arrayParams);
		}

		
		function CallbackCheckConditionForItem($strEntity,$arrayItem,$strState)
		{
			if ($strEntity == "article")	
			{
				$strType = ArrayGetValue($arrayItem,"type");
				if ($strType != "NEWS")
				{
					return $this->StateList($strEntity);
				}
			}
		}
		
	}

