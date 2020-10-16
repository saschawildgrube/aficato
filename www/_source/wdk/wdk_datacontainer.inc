<?php

	require_once("wdk_error.inc");

	class CDataContainer
	{
		private $m_arrayData;
		
		function __construct()
		{
			$this->m_arrayData = array();
		}
		
		function GetDataArray()
		{
			return $this->m_arrayData;	
		}
		
		function SetDataArray($arrayData)
		{
			if (is_array($arrayData) == true)
			{
				$this->m_arrayData = $arrayData;
			}
		}

		function MergeDataArray($arrayData)
		{
			$this->m_arrayData = ArrayMerge($this->m_arrayData,$arrayData);
		}

		
		function GetValue($key1,$key2="",$key3="",$key4="",$key5="",$key6="",$key7="")
		{
			return ArrayGetValue(
				$this->m_arrayData,
				$key1,$key2,$key3,$key4,$key5,$key6,$key7);	
		}
		
		function SetValue($value,$key1,$key2="",$key3="",$key4="",$key5="",$key6="",$key7="")
		{
			ArraySetValue($this->m_arrayData,$value,$key1,$key2,$key3,$key4,$key5,$key6,$key7);
		}
		
		function Unserialize($strSerialized)
		{
			$this->m_arrayData = ArrayUnpack($strSerialized);
			if (!is_array($this->m_arrayData))
			{
				$this->m_arrayData = array();
				return false;
			}
			return true;
		}

		function Serialize()
		{
			$strSerialized = ArrayPack($this->m_arrayData);   
			return $strSerialized;
		}
		
	}



