<?php

	class CMyScaffolder extends CScaffolder
	{
		
		function CallbackPrepareForm(CFormDefinition &$formDef)
		{

			$formDef->AddControlText(
 				"ASSEMBLYID",
 				$this->GetVarName("assemblyid"),
 				$this->GetTID("CONTROL_ASSEMBLYID"),
 				$this->GetVar("assemblyid"),
		   	40,
 				40);

			$formDef->AddControlText(
 				"MODULEGROUP",
 				$this->GetVarName("modulegroup"),
 				$this->GetTID("CONTROL_MODULEGROUP"),
 				$this->GetVar("modulegroup"),
		   	40,
 				40); 				

			$formDef->AddControlText(
 				"MODULEID",
 				$this->GetVarName("moduleid"),
 				$this->GetTID("CONTROL_MODULEID"),
 				$this->GetVar("moduleid"),
		   	20,
 				20);

		}
		
		function CallbackGetParams(&$arrayParams)
		{
			$arrayParams["a"] = StringLowerCase($this->GetVar("assemblyid"));
			$arrayParams["ModuleID"] = $this->GetVar("moduleid");
			$arrayParams["moduleid"] = StringLowerCase($arrayParams["ModuleID"]);
			$arrayParams["MODULEID"] = StringUpperCase($arrayParams["ModuleID"]);
			$arrayParams["ModuleGroup"] = $this->GetVar("modulegroup");
			$arrayParams["modulegroup"] = StringLowerCase($arrayParams["ModuleGroup"]);
			$arrayParams["MODULEGROUP"] = StringUpperCase($arrayParams["ModuleGroup"]);
		} 
		
		
		function CallbackCheckParams($arrayParams,&$arrayErrors)
		{
			$strModuleID = $arrayParams["ModuleID"];
			if (StringCheckCamelCase($strModuleID) == false)
			{
				$arrayErrors["MODULEID"]	= $this->GetTID("ERROR_MODULEID_INVALID");
			}

/*
			$strGroupName = $arrayParams["GroupName"];
			if (StringCheckCamelCase($strGroupName) == false)
			{
				$arrayErrors["GROUPNAME"]	= $this->GetTID("ERROR_GROUPNAME_INVALID");
			}
*/

			$strAssemblyID = $arrayParams["a"];
			if (IsValidAssemblyID($strAssemblyID) == false)
			{
				$arrayErrors["ASSEMBLYID"]	= $this->GetTID("ERROR_ASSEMBLYID_INVALID");
			}
			
		}

	}
	
