<?php

	require_once(GetWDKDir()."wdk_module_entitymanager.inc");
	require_once(GetWDKDir()."wdk_entity_demoitem.inc");
	
	class CDemoItemManagerModule extends CEntityManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"demo/demoitemmanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$arrayEntityDefinitions = array(
				"demoitem" => GetEntityDefinitionDemoitem()
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
					return $this->StateList("demoitem");
				default:
					return parent::OnRenderModule($strState);
			}
		}
		


		function OnCheckInputValues($strEntity,$bAdd,$arrayItem)
		{
			$arrayErrors = parent::OnCheckInputValues($strEntity,$bAdd,$arrayItem);
			if ($strEntity == "demoitem")
			{
				$strName = $arrayItem["item_name"];
				if ($strName == "")
				{
					$arrayErrors["item_name"] = "?TID_DEMOITEM_ERROR_NAMEMISSING?";
				}
			}
			return $arrayErrors; 
		}


		
	}
	

