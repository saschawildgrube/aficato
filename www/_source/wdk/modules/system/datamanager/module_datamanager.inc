<?php

	require_once(GetWDKDir()."wdk_module_entitymanager.inc");
	require_once(GetWDKDir()."wdk_entity_dataitem.inc");
	
	class CDataManagerModule extends CEntityManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"system/datamanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$arrayEntityDefinitions = array(
				"dataitem" => GetEntityDefinitionDataitem()
			);

			$this->SetEntityDefinitions($arrayEntityDefinitions);
			
			return true;
		}
		

		
		function CallbackRenderModule($strState)
		{
			$this->Trace("CallbackRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "":
					return $this->StateList("dataitem");
				default:
					return parent::CallbackRenderModule($strState);
			}
		}
		


		function CallbackCheckInputValues($strItem,$bAdd,$arrayItem)
		{
			$arrayErrors = parent::CallbackCheckInputValues($strItem,$bAdd,$arrayItem);
			return $arrayErrors;
		}
		
		function CallbackPrepareWebserviceParams($strEntity,&$arrayParams)
		{
			if ($strEntity == "dataitem")
			{
				$strCommand = ArrayGetValue($arrayParams,"command");
				if ($strCommand == "add")
				{
					$arrayParams["command"] = "set";	
				}	
				
			}
		}
		
		function CallbackPrepareItemDataList($strEntity,&$arrayItems,$strSortOption,$strSortOrder)
		{
			if ($strEntity == "dataitem")
			{
				foreach ($arrayItems as &$arrayItem)
				{
					$arrayItem["data_content"] = StringCutOff($arrayItem["data_content"],80);
				}
			}
			
			parent::CallbackPrepareItemDataList($strEntity,$arrayItems,$strSortOption,$strSortOrder);
		}
		
		function CallbackPrepareItemDataView($strEntity,$strIndex,&$arrayItem)
		{
			if ($strEntity == "dataitem")
			{
				$arrayItem["data_content"] = StringWordWrap($arrayItem["data_content"],90,"<br/>",true);
			}
			
			parent::CallbackPrepareItemDataView($strEntity,$strIndex,$arrayItem);
		}
		
	}
	
