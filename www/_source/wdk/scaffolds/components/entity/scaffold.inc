<?php

	require_once(GetWDKDir()."wdk_mysql.inc");

	class CMyScaffolder extends CScaffolder
	{
		
		function OnPrepareForm(CFormDefinition &$formDef)
		{
			$formDef->AddControlText(
 				"ENTITYNAME",
 				$this->GetVarName("entityname"),
 				$this->GetTID("CONTROL_ENTITYNAME"),
 				$this->GetVar("entityname"),
		   	20,
 				20);

				
			$formDef->AddControlText(
 				"GROUPNAME",
 				$this->GetVarName("groupname"),
 				$this->GetTID("CONTROL_GROUPNAME"),
 				$this->GetVar("groupname"),
		   	40,
 				40); 				

			$formDef->AddControlStatic(
 				"DBTABLENAME",""); 	

			$formDef->AddControlText(
 				"ASSEMBLYID",
 				$this->GetVarName("assemblyid"),
 				$this->GetTID("CONTROL_ASSEMBLYID"),
 				$this->GetVar("assemblyid"),
		   	40,
 				40);


			$formDef->AddControlText(
 				"AUTHOR",
 				$this->GetVarName("author"),
 				$this->GetTID("CONTROL_AUTHOR"),
 				$this->GetVar("author"),
		   	40,
 				40);

			
			
			
			
		}
		
		function OnGetParams(&$arrayParams)
		{
			
			$arrayParams["a"] = StringLowerCase($this->GetVar("assemblyid"));
			$arrayParams["EntityName"] = $this->GetVar("entityname");
			$arrayParams["entityname"] = StringLowerCase($arrayParams["EntityName"]);
			$arrayParams["ENTITYNAME"] = StringUpperCase($arrayParams["EntityName"]);
			$arrayParams["moduleid"] = StringLowerCase($arrayParams["entityname"])."manager";
			$arrayParams["MODULEID"] = StringUpperCase($arrayParams["moduleid"]);
			$arrayParams["ModuleID"] = $arrayParams["EntityName"]."Manager"; 
			$arrayParams["GroupName"] = $this->GetVar("groupname");
			$arrayParams["groupname"] = StringLowerCase($arrayParams["GroupName"]);
			$arrayParams["GROUPNAME"] = StringUpperCase($arrayParams["GroupName"]);
			$arrayParams["modulegroup"] = $arrayParams["groupname"];
			$arrayParams["ws1"] = $arrayParams["groupname"];
			$arrayParams["ws2"] = $arrayParams["entityname"];
			$arrayParams["webservicename"] = $arrayParams["ws1"]."/".$arrayParams["ws2"];
			$arrayParams["Author"] = $this->GetVar("author");
			$arrayParams["accesscode"] = MakeNumericToken(40);
			$arrayParams["dbtablename"] = $arrayParams["GroupName"]."-".$arrayParams["EntityName"];
			 
			/*			
				$arrayScaffoldParams["ModuleID"] = $this->GetModuleVar($strScaffoldID."_moduleid");
				$arrayScaffoldParams["MODULEID"] = StringUpperCase($this->GetModuleVar($strScaffoldID."_moduleid"));
				$arrayScaffoldParams["defaultstate"] = StringLowerCase($this->GetModuleVar($strScaffoldID."_defaultstate"));
				$arrayScaffoldParams["DefaultState"] = $this->GetModuleVar($strScaffoldID."_defaultstate");
				$arrayScaffoldParams["modulepath"] = $arrayScaffoldParams["modulegroup"]."/".$arrayScaffoldParams["moduleid"];
			}
			*/
		}
		
		
		function OnCheckParams($arrayParams,&$arrayErrors)
		{
			$strEntityName = $arrayParams["EntityName"];
			if (StringCheckCamelCase($strEntityName) == false)
			{
				$arrayErrors["ENTITYNAME"]	= $this->GetTID("ERROR_ENTITYNAME_INVALID");
			}

			$strGroupName = $arrayParams["GroupName"];
			if (StringCheckCamelCase($strGroupName) == false)
			{
				$arrayErrors["GROUPNAME"]	= $this->GetTID("ERROR_GROUPNAME_INVALID");
			}

			$strAssemblyID = $arrayParams["a"];
			if (IsValidAssemblyID($strAssemblyID) == false)
			{
				$arrayErrors["ASSEMBLYID"]	= $this->GetTID("ERROR_ASSEMBLYID_INVALID");
			}
			
		}

	}
	
