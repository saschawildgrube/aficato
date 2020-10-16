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
		
		function CallbackInitParameterDefinition(&$paramDef)
		{
			parent::CallbackInitParameterDefinition($paramDef);
		}
		
		function CallbackInitCommandDefinition(&$commandDef)
		{
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
			parent::CallbackCommand($strCommand);
		}
		
		function CallbackCheckParameter($strCommand,$strParamName,$strParamValue)
		{
			return true;	
		}

		function CallbackLog($strEventSeverity,$strEventID, $strDetails)
		{
			$this->CallbackLog_LogWebservice($strEventSeverity,$strEventID, $strDetails);
		}
		
		function CallbackCommandAddPrepare($strEntity,&$arrayFields)
		{
			if ($strEntity == "demoitem")
			{
				$arrayFields["CREATION_DATETIME"] = RenderDateTimeNow();
				$arrayFields["LASTCHANGE_DATETIME"] = $arrayFields["CREATION_DATETIME"];
			}
			return parent::CallbackCommandAddPrepare($strEntity,$arrayFields);				
		}

		function CallbackCommandSetPrepare($strEntity,&$arrayFields)
		{
			if ($strEntity == "demoitem")
			{
				$arrayFields["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
			}
			return parent::CallbackCommandSetPrepare($strEntity,$arrayFields);
		}


	}
	

