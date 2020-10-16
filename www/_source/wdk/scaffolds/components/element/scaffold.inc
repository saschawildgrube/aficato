<?php

	class CMyScaffolder extends CScaffolder
	{
		
		function CallbackPrepareForm(CFormDefinition &$formDef)
		{

			$formDef->AddControlText(
 				'ASSEMBLYID',
 				$this->GetVarName('assemblyid'),
 				$this->GetTID('CONTROL_ASSEMBLYID'),
 				$this->GetVar('assemblyid'),
		   	40,
 				40);

			$formDef->AddControlText(
 				'ELEMENTID',
 				$this->GetVarName('elementid'),
 				$this->GetTID('CONTROL_ELEMENTID'),
 				$this->GetVar('elementid'),
		   	40,
 				40);

		}
		
		function CallbackGetParams(&$arrayParams)
		{
			$arrayParams['a'] = StringLowerCase($this->GetVar('assemblyid'));
			$arrayParams['ElementId'] = $this->GetVar('elementid');
			$arrayParams['elementid'] = StringLowerCase($arrayParams['ElementId']);
			$arrayParams['ELEMENTID'] = StringUpperCase($arrayParams['ElementId']);
		} 
		
		
		function CallbackCheckParams($arrayParams,&$arrayErrors)
		{
			$strElementId = $arrayParams['ElementId'];
			if (StringCheckCamelCase($strElementId) == false)
			{
				$arrayErrors['ELEMENTID']	= $this->GetTID('ERROR_ELEMENTID_INVALID');
			}

			$strAssemblyID = $arrayParams['a'];
			if (IsValidAssemblyID($strAssemblyID) == false)
			{
				$arrayErrors['ASSEMBLYID']	= $this->GetTID('ERROR_ASSEMBLYID_INVALID');
			}
			
		}

	}
	
