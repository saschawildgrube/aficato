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
		
		function CallbackInit()
		{
			$arrayEntityDefinitions = array(
				"demoitem" => GetEntityDefinitionDemoitem()
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
					return $this->StateList("demoitem");
				default:
					return parent::CallbackRenderModule($strState);
			}
		}
		


		function CallbackCheckInputValues($strEntity,$bAdd,$arrayItem)
		{
			$arrayErrors = parent::CallbackCheckInputValues($strEntity,$bAdd,$arrayItem);
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
	

