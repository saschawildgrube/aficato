<?php

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");
	require_once(GetWDKDir()."wdk_entity_demoitem.inc");
		
   class CDatabaseDemoWebService extends CDatabaseEntityWebService
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

			$arrayEntityDefinition = array(
				"demoitem" => GetEntityDefinitionDemoitem()
				);
			
			
			
			parent::__construct(
				"demo/databasedemo",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinition);
		}
		
		function OnInitParameterDefinition(&$paramDef)
		{
			parent::OnInitParameterDefinition($paramDef);
		}
		
		function OnInitCommandDefinition(&$commandDef)
		{
			parent::OnInitCommandDefinition($commandDef);
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
			parent::OnCommand($strCommand);
		}
		
		function OnCheckParameter($strCommand,$strParamName,$strParamValue)
		{
			return true;	
		}

		function OnLog($strEventSeverity,$strEventID, $strDetails)
		{
			$this->OnLog_LogWebservice($strEventSeverity,$strEventID, $strDetails);
		}
		
		function OnCommandAddPrepare($strEntity,&$arrayFields)
		{
			if ($strEntity == "demoitem")
			{
				$arrayFields["CREATION_DATETIME"] = RenderDateTimeNow();
				$arrayFields["LASTCHANGE_DATETIME"] = $arrayFields["CREATION_DATETIME"];
			}
			return parent::OnCommandAddPrepare($strEntity,$arrayFields);				
		}

		function OnCommandSetPrepare($strEntity,&$arrayFields)
		{
			if ($strEntity == "demoitem")
			{
				$arrayFields["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
			}
			return parent::OnCommandSetPrepare($strEntity,$arrayFields);
		}


	}
	

