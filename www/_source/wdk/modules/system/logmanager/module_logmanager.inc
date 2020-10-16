<?php

	require_once(GetWDKDir()."wdk_module_entitymanager.inc");
	require_once(GetWDKDir()."wdk_entity_log.inc");
	
	class CLogManagerModule extends CEntityManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"system/logmanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$arrayEntityDefinitions = array(
				"log" => GetEntityDefinitionLog()
			);

			$this->SetEntityDefinitions($arrayEntityDefinitions);
			
			return true;
		}
		
	
		function CallbackRenderModule($strState)
		{
			$this->Trace("CallbackRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "help":
					return $this->StateHelp();
				case "":
					return $this->StateList("log");
				default:
					return parent::CallbackRenderModule($strState);
			}
		}
		
		function CallbackPrepareFilterSelectOptions($strEntity,$strFilter)
		{
			$arrayOptions = array();
			if ($strEntity == "log")
			{
				if ($strFilter == "filterbyseverity")
				{
					$arrayOptions = array();
					$arrayOptions[""] = "?TID_OPTION_ALL?";
					$arraySeverity = GetEventSeverities();
					foreach ($arraySeverity as $strSeverity)
					{
						if ($strSeverity != "TESTWHITELIST" && $strSeverity != "TESTBLACKLIST")
						{
							$arrayOptions[$strSeverity] = $strSeverity;
						}
					}
				}
			}
			return $arrayOptions;
		}		
		
		function CallbackPrepareItemDataView($strEntity,$strIndex,&$arrayItem)
		{
			if ($strEntity == "log")
			{
				$strDetails = HtmlDecode(chop(ArrayGetValue($arrayItem,"event_details")));
				if ($strDetails != "")
				{
					$bUseTable = true;
					$arrayDetails = ParseURLParameters($strDetails);
					if (is_array($arrayDetails))
					{
						if (ArrayCount($arrayDetails) == 1)
						{
							foreach ($arrayDetails as $strKey => $strValue)
							{
								if ($strValue == "")
								{
									$bUseTable = false;
								}
							}
						}
						if ($bUseTable == true)
						{
							$arrayTableDetails = array();
							foreach ($arrayDetails as $strKey => $strValue)
							{
								$arrayRow = array();
								$arrayRow[] = $strKey;
								$arrayRow[] = $strValue;
								$arrayTableDetails[] = $arrayRow;
							}
							$table = new CTableElement($this->GetWebsite(),"default");
							$arrayItem["event_details"] = $table->RenderTable($arrayTableDetails,false,true);
						}
					}
				}
			}
			parent::CallbackPrepareItemDataView($strEntity,$strIndex,$arrayItem);
		}
	}

