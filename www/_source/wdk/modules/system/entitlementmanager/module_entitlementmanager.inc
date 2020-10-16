<?php

	require_once(GetWDKDir()."wdk_module_entitymanager.inc");
	require_once(GetWDKDir()."wdk_entity_entitlement.inc");
	
	class CEntitlementManagerModule extends CEntityManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"system/entitlementmanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$arrayEntityDefinitions = array(
				"entitlement" => GetEntityDefinitionEntitlement()
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
					return $this->StateList("entitlement");
				default:
					return parent::CallbackRenderModule($strState);
			}
		}
		
		function CallbackPrepareWebserviceParams($strEntity,&$arrayParams)
		{
			if ($strEntity == "entitlement")
			{
				$strCommand = $arrayParams["command"];
				if ($strCommand == "list")
				{
					$arrayParams = array();
					$arrayParams["command"] = "get";
					$arrayParams["user_id"] = $this->GetModuleConfig("user_id");
				}
			}
		}

		
	}
	

