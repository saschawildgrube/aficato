<?php



	class CCommandDefinition
	{
		private $m_arrayCommandDefs;
		
		function __contruct()
		{
			$this->m_arrayCommandDefs = array();	
		}
		
		function GetCommandDefs()
		{
			return $this->m_arrayCommandDefs;	
		} 
		
		function AddCommandDefinition(
			$strName,
			$arrayParams)
		{
			$arrayCommandDef = array();
			
			$strName = StringLowerCase($strName);
			
			$arrayCommandDef["parameter"] = $arrayParams;
			
			$this->m_arrayCommandDefs[$strName] = $arrayCommandDef;
		}


		function AddCommandDefinition_install()
		{
			$arrayParams = array();
			$this->AddCommandDefinition(
				"install",
				$arrayParams);
		}
		
		function AddCommandDefinition_list(
			$nFilterOptions = 0,
			$nSortOptions = 0,
			$strPrefix = "",
			$arrayAdditionalParameter = array())
		{
			if ($strPrefix != "")
			{
				$strPrefixUnderscore = $strPrefix . "_";
			}
			else
			{
				$strPrefixUnderscore = "";
			}
			$arrayParams = array("offset","blocksize");
			for ($nIndex = 1; $nIndex <= $nFilterOptions; $nIndex++)
			{
				array_push($arrayParams,$strPrefixUnderscore."filter".$nIndex);
				array_push($arrayParams,$strPrefixUnderscore."filter".$nIndex."_operator");
				array_push($arrayParams,$strPrefixUnderscore."filter".$nIndex."_value");
			}
			for ($nIndex = 1; $nIndex <= $nSortOptions; $nIndex++)
			{
				array_push($arrayParams,$strPrefixUnderscore."sort".$nIndex);
				array_push($arrayParams,$strPrefixUnderscore."sort".$nIndex."_order");
			}
			if (is_array($arrayAdditionalParameter))
			{
				$arrayParams = ArrayMerge($arrayParams,$arrayAdditionalParameter);
			}
			
			$this->AddCommandDefinition(
				$strPrefix."list",
				$arrayParams);
			 
		}

		function AddCommandDefinition_help()
		{
			$arrayParams = array();
			$this->AddCommandDefinition(
				"help",
				$arrayParams);
		}

		function AddCommandDefinition_selfcheck()
		{
			$arrayParams = array();
			$this->AddCommandDefinition(
				"selfcheck",
				$arrayParams);
		}

		function CheckCommand($strCommand,$arrayParams,$arrayDefaultParams)
		{
			if (!is_array($arrayDefaultParams))
			{
				$arrayDefaultParams = array();
			}
			if ($strCommand == "")
			{
				return 'COMMAND_MISSING';	
			}
			if (!ArrayKeyExists($this->m_arrayCommandDefs,$strCommand))
			{
				return 'COMMAND_UNKNOWN';	
			}
			
			$arrayCommandParams = ArrayGetValue($this->m_arrayCommandDefs[$strCommand],"parameter");
			if (!is_array($arrayCommandParams))
			{
				$arrayCommandParams = array();
			}
			
			foreach ($arrayParams as $strParamName => $strParamValue)
			{
				if (!ArrayValueExists($arrayDefaultParams,$strParamName))
				{
					if (!ArrayValueExists($arrayCommandParams,$strParamName))
					{
						return 'PARAMETER_'.StringUpperCase($strParamName).'_MISMATCH';
					}
				}
			}
			return '';
		}
		
	}



