<?php

	require_once("wdk_datacontainer.inc");
	require_once("wdk_arraytextfile.inc");


	function IsValidConfigID($strConfigID)
	{
		if ($strConfigID == "")
		{
			return false;	
		}	
		if (StringCheckCharSet($strConfigID,CHARSET_ID) == false)
		{
			return false;	
		}
		if (StringSection($strConfigID,0,1) == "-")
		{
			return false;	
		}
		if (StringSection($strConfigID,-1,1) == "-")
		{
			return false;	
		}
		return true;
	}

	class CConfig extends CDataContainer
	{
		private $m_arrayConfigStoreLocations;
		private $m_arrayLoadedConfigStores;
		private $m_arraySupportedConfigStoreTypes;

		function __construct()
		{
			parent::__construct();
			$this->m_arrayLoadedConfigStores = array();
			$this->m_arrayConfigStoreLocations = array();
			//$this->m_arraySupportedConfigStoreTypes = array("cfg","csv","xml");
			$this->m_arraySupportedConfigStoreTypes = array("cfg");
		}
		
		function AddConfigStoreLocation($strNewConfigStoreLocation)
		{
			array_push($this->m_arrayConfigStoreLocations,$strNewConfigStoreLocation);
			
		}
		function GetLoadedConfigStores()
		{
			return $this->m_arrayLoadedConfigStores;
		}
		
		function LoadConfig($strConfigID)
		{
			if (IsValidConfigID($strConfigID) == false)
			{
				return false;
			}
			foreach ($this->m_arrayConfigStoreLocations as $strLocation)
			{
				foreach ($this->m_arraySupportedConfigStoreTypes as $strStoreType)	
				{
					$arrayNewConfigValues = array();
					
					$strConfigFile = $strLocation.$strConfigID.".".$strStoreType;
					switch ($strStoreType)
					{
						case "cfg":
							$arrayNewConfigValues = LoadNamedArrayFromTextFile($strConfigFile);
							if (is_array($arrayNewConfigValues))
							{
								array_push($this->m_arrayLoadedConfigStores,$strConfigID);
								$arrayNewConfigValues = array_change_key_case($arrayNewConfigValues,CASE_LOWER);
								$this->MergeDataArray($arrayNewConfigValues);
								return true;
							}
							break;
						default:
							FatalError("Unsupported config store type \"$strStoreType\".");
							break;	
					}
				}
			}
			return false;
		}
		
	}



