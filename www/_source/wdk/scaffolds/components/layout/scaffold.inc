<?php

	class CMyScaffolder extends CScaffolder
	{
		
		function OnPrepareForm(CFormDefinition &$formDef)
		{

			$formDef->AddControlText(
 				"ASSEMBLYID",
 				$this->GetVarName("assemblyid"),
 				$this->GetTID("CONTROL_ASSEMBLYID"),
 				$this->GetVar("assemblyid"),
		   	40,
 				40);

			$formDef->AddControlText(
 				"LAYOUTID",
 				$this->GetVarName("layoutid"),
 				$this->GetTID("CONTROL_LAYOUTID"),
 				$this->GetVar("layoutid"),
		   	20,
 				20);

		}
		
		function OnGetParams(&$arrayParams)
		{
			$arrayParams["a"] = StringLowerCase($this->GetVar("assemblyid"));
			$arrayParams["LayoutID"] = $this->GetVar("layoutid");
			$arrayParams["layoutid"] = StringLowerCase($arrayParams["LayoutID"]);
			$arrayParams["LAYOUTID"] = StringUpperCase($arrayParams["LayoutID"]);
		} 
		
		
		function OnCheckParams($arrayParams,&$arrayErrors)
		{
			$strLayoutID = $arrayParams["LayoutID"];
			if (StringCheckCamelCase($strLayoutID) == false)
			{
				$arrayErrors["LAYOUTID"]	= $this->GetTID("ERROR_LAYOUTID_INVALID");
			}

			$strAssemblyID = $arrayParams["a"];
			if (IsValidAssemblyID($strAssemblyID) == false)
			{
				$arrayErrors["ASSEMBLYID"]	= $this->GetTID("ERROR_ASSEMBLYID_INVALID");
			}
			
		}

	}
	
