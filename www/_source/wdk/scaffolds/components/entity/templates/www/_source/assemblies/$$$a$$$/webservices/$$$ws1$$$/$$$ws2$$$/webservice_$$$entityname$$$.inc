<?php

	require_once(GetWDKDir()."wdk_webservice_database_entity.inc");  
	require_once(GetSourceDir()."assemblies/$$$a$$$/entity_$$$entityname$$$.inc");
		
   class C$$$EntityName$$$WebService extends CDatabaseEntityWebService
	{
		function __construct(
			$strSourceDir,
			$strServiceDir,
			$arrayConfig,
			$arrayParams)
		{
			$arrayMeta = array();
			$arrayMeta["version"] = "1.0"; 
			$arrayMeta["author"] = "$$$Author$$$";

			$arrayEntityDefinition = array(
				"$$$entityname$$$" => GetEntityDefinition$$$EntityName$$$()
				);
				
				
			parent::__construct(
				"$$$webservicename$$$",
				$strSourceDir,
				$strServiceDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$arrayEntityDefinition);
				
		}
		
		function OnInit()
		{
			$bResult = parent::OnInit();
			if ($bResult == false)
			{
				return false;
			}

			/*
				The following code establishes the connection to the configured
				database.
				
				You may not want to do this in all cases. So feel free to wrap this
				with additional conditions.
			*/
			if ($this->DatabaseConnect() != true)
			{
				return false;
			}			
			
			/*
				Do additional command independend initialization here!
			*/
			
			return true;
		}
		
		function OnInitCommandDefinition(&$commandDef)
		{
			/*
				Add more command definitions here!
				"helloworld" doesn't really do anything. It just shows how it works.
				Remove this part if not needed!
			*/
			$commandDef->AddCommandDefinition(
				"helloworld",
				array(
					"param1",
					"param2"
					)
				);
			
			/*
				You should consider to support a "cleanup" command to remove
				unused data rows after a grace period (or moving them to a
				separate graveyard table)
				Think of inactive users, closed tickets, canceled contracts,
				or completed projects of past years...
			*/
			$commandDef->AddCommandDefinition(
				"cleanup",
				array()
				);				
				
			parent::OnInitCommandDefinition($commandDef);
		}

		function OnInitParameterDefinition(&$paramDef)
		{
			/*
				param1 and param2 are used by the "helloworld" command.
			*/
			$paramDef->AddParameterDefinitionInteger("param1",0,10);
			$paramDef->AddParameterDefinitionString("param2",2,20,"1234567890abcdefABCDEF");
			
			/*
				update_lastaccess tells the "get" command if it sould update the 
				LASTACCESS_DATETIME value in the database.
			*/
			$paramDef->AddParameterDefinitionBoolean("update_lastaccess");
			
			/*
				Please note: parameter definitions are global for the web service.
				If two or more commands share the same parameter, they also share the
				parameter definition.
			*/
			parent::OnInitParameterDefinition($paramDef);
			
		}

		function OnCheckParameter($strCommand,$strParamName,$strParamValue)
		{
			/*
				Implement specific checks for parameters here!
			*/
			if ($strParamName == "param1")
			{
				if (intval($strParamValue) == 5)
				{
					return false;
				}
			}
			/*
				You may want to let the default checks take place before or after your own.
			*/
			$bResult = parent::OnCheckParameter($strCommand,$strParamName,$strParamValue);	
			if ($bResult == true)
			{
				if ($strParamName == "param2")
				{
					if (StringSection($strVaramValue,0,2) == "00")
					{
						return false;	
					}
				}
			}
			return $bResult;
		}		
	
		function OnCommand($strCommand)
		{
			switch ($strCommand)
			{
			/*
				Dispatch commands to functions within the web service implementation
				class!
			*/
			case "helloworld":
				$this->CommandHelloWorld();
				return;

			case "cleanup":
				$this->CommandCleanUp();
				return;
				
			default:
				parent::OnCommand($strCommand);
				return;
			}
		}

		function CommandHelloWorld()
		{
			/*
				This is our demo command implementation function.
			*/
			$this->SetResultValue("WORLD","HELLO");
			/*
				Please note: The string "WORLD" is the value of the tag named "HELLO".
				Check the definiton of the SetResultValue function for clarification!
			*/
			return;
		}




		function CommandAdd($strEntity)
		{
			/*
				Create your own implementation of the "add" command...
			*/
			if ($strEntity == "$$$entityname$$$")
			{
				/*
					...or just call the default implementation
				*/
				parent::CommandAdd("$$$entityname$$$");
			}
			else
			{
				parent::CommandAdd($strEntity);
			}
		}
		
		function OnCommandAddPrepare($strEntity,&$arrayItem)
		{
			if ($strEntity == "$$$entityname$$$")
			{
				/*
					Use this function to preset attribute values when
					adding new items!
				*/
				$arrayItem["CREATION_DATETIME"] = RenderDateTimeNow();
				$arrayItem["LASTCHANGE_DATETIME"] = $arrayItem["CREATION_DATETIME"];
			}
			return parent::OnCommandAddPrepare($strEntity,$arrayItem);		
		}
		
		function OnCommandAddComplete($strEntity,$arrayItem,$strNewItemID)
		{
			/*
				This function is called after a new item has been added and thus
				the database created it's new id.
			*/
			return true;				
		}

		function MakeIndexSQLCondition($strEntity)
		{
			if ($strEntity == "$$$entityname$$$")
			{
				/*
					The default implemenation uses the index attribute to identify
					an item in the database.
					
					Feel free to create new SQL conditions that meet your demand.
				*/
				return parent::MakeIndexSQLCondition($strEntity);
			}
			else
			{
				return parent::MakeIndexSQLCondition($strEntity);
			}
		}

		function CommandGet($strEntity)
		{
			/*
				Create your own implementation of the "get" command...
			*/
			if ($strEntity == "$$$entityname$$$")
			{
				/*
					...or just call the default implementation
				*/
				parent::CommandGet("$$$entityname$$$");
			}
			else
			{
				parent::CommandGet($strEntity);	
			}
		}

		function OnCommandGetPrepare($strEntity)
		{
			/*
				Invoked before the get command receives data from the database...
				
				Return false to prevent the "get" command from actually requesting the 
				database.
			*/
			return parent::OnCommandGetPrepare($strEntity);				
		}
	

		function OnCommandGetComplete($strEntity,&$arrayItem,$strContext)
		{
			/*
				This function is invoked after data has been received
				from the database.
				The strContext parameter is either set to "get" or "list" depending
				on the command that was called.
				If the "list" command is executed, the function is invoked
				for each single item that is going to be returned by the command.
			*/
			if ($strEntity == "$$$entityname$$$")
			{
				/*
					This example implements the logic of updating the lastaccess_datetime
					attribute if the update_lastaccess parameter is set and the
					context is the "get" command - i.e. it will not update the attribute
					if the "list" command is invoked.
				*/
				if ($strContext == "get")
				{
					if ($this->GetParam("update_lastaccess") == true)
					{
						$arrayItem["LASTACCESS_DATETIME"] = RenderDateTimeNow();
						$str$$$EntityName$$$ID = $arrayItem["$$$ENTITYNAME$$$_ID"];
						$strSQL = MakeSQL_Update(
							$this->GetDatabaseTableName("$$$entityname$$$"),
							array(
								"LASTACCESS_DATETIME" => $arrayItem["LASTACCESS_DATETIME"]
								),
							"`$$$ENTITYNAME$$$_ID` = ".MakeSQL_Helper_Value($str$$$EntityName$$$ID)
							);
						$this->DatabaseQuery($strSQL);
					}
				}
			}
			return parent::OnCommandGetComplete($strEntity,$arrayItem,$strContext);
		}		
		
		
		
		
				
		function CommandSet($strEntity)
		{
			/*
				Create your own implementation of the "set" command...
			*/
			if ($strEntity == "$$$entityname$$$")
			{
				/*
					...or just call the default implementation
				*/
				parent::CommandSet("$$$entityname$$$");
			}
			else
			{
				parent::CommandSet($strEntity);
			}
		}
		
		function OnCommandSetPrepare($strEntity,&$arrayItem)
		{
			/*
				This function is invoked before new data is
				used to update an existing item in the database.
				Use it to make modifications to the attributes before being
				updated!
			*/
			if ($strEntity == "$$$entityname$$$")
			{			
				$arrayItem["LASTCHANGE_DATETIME"] = RenderDateTimeNow();
			}
			return parent::OnCommandSetPrepare($strEntity,$arrayItem);
		}  
		
		function OnCommandSetComplete($strEntity,$arrayItem)
		{
			/*
				This function is called after attribute data has been updated.
			*/
			return parent::OnCommandSetComplete($strEntity,$arrayItem);
		}  

		
		function CommandDelete($strEntity)
		{
			if ($strEntity == "$$$entityname$$$")
			{
				parent::CommandDelete("$$$entityname$$$");
			}
			else
			{
				parent::CommandDelete($strEntity);
			}
		}
			
		
		function CommandCleanUp()
		{
			$this->Trace("CommandCleanUp");
			
			/*
				This implements a simple clean up function.
				You may want to implement more sophisticated logic here (or remove
				it at all).
			*/
			
			$strTable = $this->GetDatabaseTableName("$$$entityname$$$");
			
			// the last access should be more than 30 days in the past
			$strDateTimeThreshold = RenderDateTime(GetTimeAddDays(GetTimeNow(),-30));
				
			$strSQL = MakeSQL_Delete(
				$strTable,
				"`LASTACCESS_DATETIME` < ".MakeSQL_Helper_Value($strDateTimeThreshold)." AND `ACTIVE` = '0'");
	
			if ($this->DatabaseQuery($strSQL) != true)
			{
				return;
			}
			
			return;
		}		
		
		function CommandSelfCheck()
		{
			parent::CommandSelfCheck();
			if ($this->GetError() != "")
			{
				return;	
			}
			$this->Trace("C$$$EntityName$$$Webservice::CommandSelfCheck()");
			/*
				Add your specific checks here!
				The command function should set an error if a condition is met that
				prevents the web service from operating properly.
				For example you may want to check a specific php version or other aspects
				of the server environment.
			*/
		}

		function CommandInstall()
		{
			parent::CommandInstall();	

			/*
				Add code here that is needed once to set up the web service.
				The parent implementation will create the database table
				according to the configuration in the entity definition.
			*/

			/*
				This will create a scheduled job that runs the "cleanup"
				command using the system/scheduler web service.
				This requires that the system/scheduler web service is configured
				for use by this web service.
			*/
			$arrayParams = array();
			$arrayParams["command"] = "cleanup";
			$this->Install_AddScheduledJob(
				"$$$EntityName$$$Cleanup",
				$arrayParams,
				"0",
				"*",
				"*",
				"*",
				"*",
				20);
			
			
		}

	
		


	}

