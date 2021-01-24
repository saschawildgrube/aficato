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
		
		function OnInit()
		{
			$arrayEntityDefinitions = array(
				"dataitem" => GetEntityDefinitionDataitem()
			);

			$this->SetEntityDefinitions($arrayEntityDefinitions);
			
			return parent::OnInit();
		}
		

		
		function OnRenderModule($strState)
		{
			$this->Trace("OnRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "":
					return $this->StateList("dataitem");
				default:
					return parent::OnRenderModule($strState);
			}
		}
		


		function OnCheckInputValues($strItem,$bAdd,$arrayItem)
		{
			$arrayErrors = parent::OnCheckInputValues($strItem,$bAdd,$arrayItem);
			return $arrayErrors;
		}
		
		function OnPrepareWebserviceParams($strEntity,&$arrayParams)
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
		
		function OnPrepareItemDataList($strEntity,&$arrayItems,$strSortOption,$strSortOrder)
		{
			if ($strEntity == "dataitem")
			{
				foreach ($arrayItems as &$arrayItem)
				{
					$arrayItem["data_content"] = StringCutOff($arrayItem["data_content"],80);
				}
			}
			
			parent::OnPrepareItemDataList($strEntity,$arrayItems,$strSortOption,$strSortOrder);
		}
		
		function OnPrepareItemDataView($strEntity,$strIndex,&$arrayItem)
		{
			if ($strEntity == "dataitem")
			{
				$arrayItem["data_content"] = StringWordWrap($arrayItem["data_content"],90,"<br/>",true);
			}
			
			parent::OnPrepareItemDataView($strEntity,$strIndex,$arrayItem);
		}
		
	}
	
