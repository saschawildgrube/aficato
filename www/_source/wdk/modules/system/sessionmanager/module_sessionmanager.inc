<?php

	require_once(GetWDKDir()."wdk_module_entitymanager.inc");
	require_once(GetWDKDir()."wdk_entity_session.inc");

/*	
// owner id must be supplied 
$arrayParams["ownerid"] = $this->GetModuleVar("ownerid");				
*/
	
	
	class CSessionManagerModule extends CEntityManagerModule
	{
	
		function __construct(
			CWebSite $website,
			$strInstanceID = "",
			$arrayConfig = array())
		{
			parent::__construct(
				$website,
				"system/sessionmanager",
				$strInstanceID,
				$arrayConfig);
		}
		
		function OnInit()
		{
			$arrayEntityDefinitions = array(
				"session" => GetEntityDefinitionSession()
			);
	
			$this->SetEntityDefinitions($arrayEntityDefinitions);
			
			return parent::OnInit();
		}
		
		function OnPrepareItemDataView($strEntity,$strIndex,&$arrayItem)
		{
			parent::OnPrepareItemDataView($strEntity,$strIndex,$arrayItem);
			if ($strEntity == "session")
			{
				// Display the user name (instead of the user id)
				$strUserID = ArrayGetValue($arrayItem,"user_id_authentication");
				$arrayItem["user_id_authentication"] = "";
				if (intval($strUserID) != 0)
				{
					$webservice = new CWebserviceConsumerWebApplication($this->GetWebsite());
					$arrayParams["command"] = "get";
					$arrayParams["user_id"] = $strUserID;
					$webservice->ConsumeWebService("system/user",$arrayParams);
					$strError = $webservice->GetError();
					if ($strError == "")
					{
						$arrayItem["user_id_authentication"] = $webservice->GetResultValue("USER","USER_NAME");
					}
				}
				
				// Nicely display the session payload
				$strData = $arrayItem["data"];
				$arrayData = ArrayUnpack($strData);
				$arrayTable = ArrayTransformToTable($arrayData);
				if (is_array($arrayTable))
				{
					$table = new CTableElement($this->GetWebsite(),"default");
					$arrayItem["data"] = $table->RenderTable($arrayTable,false,true);
				}
				 
				// Mark the current session
				$strSessionID = $strIndex;
				if ($this->GetWebsite()->GetSessionID() == $strSessionID)
				{
					$this->Trace("Current Session identified");
					$this->IncludeElement("format");
					$format = new CFormatElement($this->GetWebsite(),"strong");
					foreach ($arrayItem as $strKey => &$strValue)
					{
						if ($strKey != "TASKS" && $strKey != "INDEX")
						{
							$strValue = $format->RenderFormat($strValue);
						}
					}
				}
			}
		}
		
		function OnPrepareToolbar()
		{
			$arrayParams = array();
			$arrayParams[$this->GetModuleVarNameState()] = "sessioncleanup";
			$strLink = $this->GetWebsite()->MakeURL($arrayParams);
			$this->AddTool($strLink, "delete", "?TID_SESSION_TOOL_SESSIONCLEANUP?");
			parent::OnPrepareToolbar();	
		}
		
		function OnRenderModule($strState)
		{
			$this->Trace("OnRenderModule(\"$strState\")");
			switch ($strState)
			{
				case "sessioncleanup":
					return $this->StateSessionCleanup();
				case "":
					return $this->StateList("session");
				default:
					return parent::OnRenderModule($strState);
			}
		}
		
		function StateSessionCleanup()
		{
			$strWebservice = ArrayGetValue($this->GetEntityDefinition("session"),"webservice","name");
			
			$consumer = new CWebServiceConsumerWebApplication($this->GetWebsite());
			$arrayParams = array();
			$arrayParams["command"] = "cleanup";
			$consumer->ConsumeWebService($strWebservice,$arrayParams);
			$strError = $consumer->GetError();
			if ($strError == "")
			{
				$this->SetInfoMessageCodeEntity("SESSION","CLEANUP_SUCCESS");
			}
			else
			{
				$this->SetErrorMessageCode($strError);	
			}
			
			return $this->StateList("session");	
		}
		
		function OnItemDeleted($strEntity,$strItemIndex,$arrayItem)
		{
			parent::OnItemDeleted($strEntity,$strItemIndex,$arrayItem);
			$strSessionID = ArrayGetValue($arrayItem,"session_id");
			if ($this->GetWebsite()->GetSessionID() == $strSessionID)
			{
				$this->GetWebsite()->UserLogout();
				$this->GetWebsite()->SessionDestroy();
			}
		}				
		
		
	}

