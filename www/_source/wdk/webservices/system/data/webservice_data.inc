<?php

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");
	require_once(GetWDKDir()."wdk_entity_dataitem.inc");
		
   class CDataWebService extends CDatabaseEntityWebService 
	{
		private $m_arrayTableFields;
		private $m_nMaxContentSize;
				
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = WDK_VERSION; 
			$arrayMeta["author"] = "Sascha Wildgrube";
			
			$this->m_nMaxContentSize = 65535;
			$nMaxContentSize = intval(ArrayGetValue($arrayConfig,"max_content_size"));
			if ($nMaxContentSize > 0)
			{
				$this->m_nMaxContentSize = $nMaxContentSize;
			}

			$arrayEntityDefinitions = array(
				"dataitem" => GetEntityDefinitionDataItem()
				);
				
			$arrayEntityDefinitions["dataitem"]["attributes"]["data_content"]["maxlen"] = $this->m_nMaxContentSize;

			parent::__construct(
				"system/data",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinitions);
		}
		
		function OnInitParameterDefinition(&$paramDef)
		{
			return parent::OnInitParameterDefinition($paramDef);
		}
		
		function OnInitCommandDefinition(&$commandDef)
		{
			$commandDef->AddCommandDefinition("deletecontext",array("data_context_type","data_context_id"));
			return parent::OnInitCommandDefinition($commandDef);
		}
		
		function OnInit()
		{
			if ($this->DatabaseConnect() != true)
			{
				return false;
			}			
			return parent::OnInit();
		}		
		
		function OnCommand($strCommand)
		{
			switch ($strCommand)
			{
			case "deletecontext":
				$this->CommandDeleteContext();
				return;

			default:
				parent::OnCommand($strCommand);
				return;
			}
		}
		
		function MakeIndexSQLCondition($strEntity)
		{
			if ($strEntity != "dataitem")
			{
				return parent::MakeIndexSQLCondition($strEntity);
			}

			$this->Trace("CheckParamsAndMakeSQLCondition");
	
			$strSQLCondition = "";
			$strName = $this->GetParam("data_name");
			$strID = $this->GetParam("data_id");
			$strContextType = $this->GetParam("data_context_type");
			$strContextID = $this->GetParam("data_context_id");

			$this->Trace("CheckParamsAndMakeSQLCondition: strName = \"$strName\"");
			$this->Trace("CheckParamsAndMakeSQLCondition: strID = \"$strID\"");
			$this->Trace("CheckParamsAndMakeSQLCondition: strContextType = \"$strContextType\"");
			$this->Trace("CheckParamsAndMakeSQLCondition: strContextID = \"$strContextID\"");

			if (($strName == "") && ($strID ==""))
			{
				$this->SetError("DATA_IDENTIFIER_MISSING");
				return "";	
			}
			if ($strID != "")			
			{
				return $strSQLCondition = $this->MakeSQL_Helper_Field("DATA_ID")." = ".MakeSQL_Helper_Value($strID);
			}
			if (($strContextType == "") && ($strContextID != ""))
			{
				$this->SetError("DATA_CONTEXTID_WITHOUT_CONTEXTTYPE");
				return "";	
			}
			if (($strContextType != "") && ($strContextID == ""))
			{
				$this->SetError("DATA_CONTEXTTYPE_WITHOUT_CONTEXTID");
				return "";	
			}
			$strSQLCondition =
				$this->MakeSQL_Helper_Field("DATA_NAME")." = ".MakeSQL_Helper_Value($strName).
				" AND ".$this->MakeSQL_Helper_Field("DATA_CONTEXT_TYPE")." = ".MakeSQL_Helper_Value($strContextType).
				" AND ".$this->MakeSQL_Helper_Field("DATA_CONTEXT_ID")." = ".MakeSQL_Helper_Value($strContextID);
			return $strSQLCondition;
		}


		function MakeSQLCondition_GroupByContext()
		{
			$this->Trace("MakeSQLCondition_GroupByContext");
	
			$strSQLCondition = "";
			$strContextType = $this->GetParam("data_context_type");
			$strContextID = $this->GetParam("data_context_id");

			$this->Trace("CheckParamsAndMakeSQLCondition: strContextType = \"$strContextType\"");
			$this->Trace("CheckParamsAndMakeSQLCondition: strContextID = \"$strContextID\"");

			if (($strContextType == "") && ($strContextID != ""))
			{
				$this->SetError("DATA_CONTEXTID_WITHOUT_CONTEXTTYPE");
				return "";	
			}
			if (($strContextType != "") && ($strContextID == ""))
			{
				$this->SetError("DATA_CONTEXTTYPE_WITHOUT_CONTEXTID");
				return "";	
			}

			$strSQLCondition .= 
				$this->MakeSQL_Helper_Field("DATA_CONTEXT_TYPE")." = ".MakeSQL_Helper_Value($strContextType).
				" AND ".$this->MakeSQL_Helper_Field("DATA_CONTEXT_ID")." = ".MakeSQL_Helper_Value($strContextID);
				
			return $strSQLCondition;
		}

	

		function CommandDeleteContext()
		{
			$this->Trace("CommandDeleteContext");
			
			$this->m_bLegacyHack = true;
			$strSQLCondition = $this->MakeSQLCondition_GroupByContext();

			if ($strSQLCondition == "")
			{
				return;	
			}

			$strSQL = MakeSQL_Delete(
				$this->GetDatabaseTableName("dataitem"),
				$strSQLCondition);

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
		
			return;	
		}


		function CommandSet($strEntity)
		{
			if ($strEntity != "dataitem")
			{
				return parent::CommandSet($strEntity);	
			}
			
			$this->Trace("CommandSet");

			$this->m_bLegacyHack = true;			
			$strSQLCondition = $this->MakeIndexSQLCondition($strEntity);
			if ($strSQLCondition == "")
			{
				return;	
			}
			
			$strSQL = MakeSQL_Select(
				$this->GetDatabaseTableName($strEntity),
				"*",
				$strSQLCondition . " LIMIT 1");

			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			$arrayData = $this->m_database->GetData();
			
			$arrayFields = array();
			// Check and set the different values
			$arrayFields["DATA_CONTENT"] = $this->GetParam("data_content");
			$arrayFields["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
			
			if (ArrayCount($arrayData) == 0)
			{
				$this->Trace("Entry does not exist. Set CREATION_DATETIME.");
				$arrayFields["CREATION_DATETIME"] = RenderDateTimeNow();
/*
				if ($this->CheckParam("dataname") != true)
				{
					return;
				}
				*/
				$arrayFields["DATA_NAME"] = $this->GetParam("data_name");
				$arrayFields["DATA_CONTEXT_TYPE"] = $this->GetParam("data_context_type");
				$arrayFields["DATA_CONTEXT_ID"] = $this->GetParam("data_context_id");
				$strSQL = MakeSQL_Insert(
					$this->GetDatabaseTableName($strEntity),
					$arrayFields);

				if ($this->DatabaseQuery($strSQL) != true)
				{
					return;
				}
				$this->SetResultValue(
					"".$this->m_database->GetLastInsertID(),
					"NEW_DATA_ID");
			}
			else
			{
				$this->Trace("Entry does exist.");
				
				if ($this->GetParam("data_name") != "")
				{
					$arrayFields["DATA_NAME"] = $this->GetParam("data_name");
				}
				if ($this->GetParam("data_context_type") != "")
				{
					$arrayFields["DATA_CONTEXT_TYPE"] = $this->GetParam("data_context_type");
				}
				if ($this->GetParam("data_context_id") != "")
				{
					$arrayFields["DATA_CONTEXT_ID"] = $this->GetParam("data_context_id");
				}

				
				$strSQL = MakeSQL_Update(
					$this->GetDatabaseTableName($strEntity),
					$arrayFields,
					$strSQLCondition);
	
				if ($this->DatabaseQuery($strSQL) != true)
				{
					if ($this->GetError() == "DATABASE_DUPLICATE_ENTRY")
					{
						$this->SetError("DATAITEM_UNIQUE_CONSTRAINT",true);
					}
					return;
				}
			}
			return;
		}




	}
	

