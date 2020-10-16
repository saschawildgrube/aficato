<?php

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");
	require_once(GetWDKDir()."wdk_entity_entitlement.inc");
		
   class CEntitlementWebService extends CDatabaseEntityWebService
	{
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = WDK_VERSION; 
			$arrayMeta["author"] = "Sascha Wildgrube";
			
			$arrayEntityDefinitions = array(
				"entitlement" => GetEntityDefinitionEntitlement()
				);
						
			parent::__construct(
				"system/entitlement",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinitions
				);
		}
		
		function CallbackInitParameterDefinition(&$paramDef)
		{
			$paramDef->AddParameterDefinition_userid();
			parent::CallbackInitParameterDefinition($paramDef);	
		}
		
		function CallbackInitCommandDefinition(&$commandDef)
		{
			//$commandDef->AddCommandDefinition("get",array("user_id"));
			//$commandDef->AddCommandDefinition("set",array("user_id","privilege_id"));
			//$commandDef->AddCommandDefinition("delete",array("user_id","privilege_id"));
			$commandDef->AddCommandDefinition("deleteall",array("user_id"));
			parent::CallbackInitCommandDefinition($commandDef);
		}
		
		function CallbackInit()
		{
			if ($this->DatabaseConnect() != true)
			{
				return false;
			}			
			return parent::CallbackInit();
		}		
		
		
		function CallbackCommand($strCommand)
		{
			switch ($strCommand)
			{
			case "deleteall":
				$this->CommandDeleteAll();
				return;

			default:
				parent::CallbackCommand($strCommand);
				return;
			}
		}

		function CallbackLog($strEventSeverity, $strEventID, $strDetails)
		{
			$this->CallbackLog_LogWebservice($strEventSeverity, $strEventID, $strDetails);
		}	

		
		function CheckParamsAndMakeSQLCondition_UserID()
		{
			$strSQLCondition = "`USER_ID` = ".MakeSQL_Helper_Value($this->GetParam("user_id"));
			return $strSQLCondition;
		}

	
		function CommandGet($strEntity)
		{
			$this->Trace("CommandGet");
			if ($strEntity != "entitlement")
			{
				return parent::CommandGet($strEntity);	
			}
			
			$strSQLCondition = $this->CheckParamsAndMakeSQLCondition_UserID();
			if ($strSQLCondition == "")
			{
				return;	
			}
			
			$arrayParams = array();
			$arrayParams["blocksize"] = "1000";
			$arrayParams["offset"] = "0";
			$arrayParams["filter1"] = "user_id";
			$arrayParams["filter1_value"] = $this->GetParam("user_id");
			$arrayParams["filter1_operator"] = "=";
			$arrayParams["sort1"] = "privilege_id";
			$arrayParams["sort1_order"] = "asc";
			$this->SetAllParams($arrayParams);
			return parent::CommandList("entitlement");
		}
		
				
		function CommandSet($strEntity)
		{
			$this->Trace("CommandSet");
			
			if ($strEntity != "entitlement")
			{
				return parent::CommandSet($strEntity);	
			}
			
			
			/*
			
			1. Check if entitlement exists
			2. if so: update it
			3. if not create it
						
			*/
			
			/*
			if ($this->CheckParam("user_id") != true)
			{
				return;
			}
			*/
			$strUserID = $this->GetParam("user_id");
			
			/*
			if ($this->CheckParam("privilege_id") != true)
			{
				return;
			}
			*/
			$strPrivilegeID = StringUpperCase($this->GetParam("privilege_id"));
			
			$strSQLCondition = "`USER_ID` = ".MakeSQL_Helper_Value($strUserID)." AND `PRIVILEGE_ID` = ".MakeSQL_Helper_Value($strPrivilegeID);
			
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName("entitlement"),
				"*",
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				//$this->SetError("USER_EXISTS");
				return;
			}
			
			$arrayData = $this->m_database->GetData();
			$nRows = ArrayCount($arrayData);
			
			$arrayFields = array();
			$arrayFields["USER_ID"] = $strUserID;
			$arrayFields["PRIVILEGE_ID"] = $strPrivilegeID;

			
			if ($nRows == 0)
			{
				// Entitlement does not exist	
				
				$strSQL = MakeSQL_Insert(
					$this->GetDatabaseTableName("entitlement"),
					$arrayFields);
				
				
				if ($this->DatabaseQuery($strSQL) != true)
				{
					return;
				}
				
				$this->SystemLog("INFORMATIONAL","PRIVILEGE_SET","",array("userid" => $strUserID, "privilege" => $strPrivilegeID));
				return;
			}
			
			if ($nRows == 1)
			{
				// Entitlement exists
				
				$row = ArrayGetValue($arrayData,0);
				$arrayFields["ENTITLEMENT_ID"] = $row["ENTITLEMENT_ID"];

				$strSQL = MakeSQL_Replace(
					$this->GetDatabaseTableName("entitlement"),
					$arrayFields);
				

				if ($this->DatabaseQuery($strSQL) != true)
				{
					return;
				}
				
				$this->SystemLog("INFORMATIONAL","PRIVILEGE_UPDATE","",array("userid"=>$strUserID,"privilege"=>$strPrivilegeID));
				return;
			}
			
			$this->SetError("ENTITLEMENT_AMBIGUOUS");
			return;	
		}
		
	
	
		function CommandDelete($strEntity)
		{
			$this->Trace("CommandDelete");
			if ($strEntity != "entitlement")
			{
				return parent::CommandDelete($strEntity);	
			}

			
			
			/*
			if ($this->CheckParam("user_id") != true)
			{
				return;
			}
			*/
			$strUserID = $this->GetParam("user_id");

/*
			if ($this->CheckParam("privilegeid") != true)
			{
				return;
			}
			*/
			$strPrivilegeID = $this->GetParam("privilege_id");

			$strSQL = MakeSQL_Delete(
				$this->GetDatabaseTableName("entitlement"),
				"`USER_ID` = '$strUserID' AND `PRIVILEGE_ID` = '$strPrivilegeID'");

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
		
			return;	
		}


		function CommandDeleteAll()
		{
			$this->Trace("CommandDeleteAll");

			$strUserID = $this->GetParam("user_id");

			$strSQL = MakeSQL_Delete(
				$this->GetDatabaseTableName("entitlement"),
				"`USER_ID` = '$strUserID'");

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			return;	
		}


	}
	

