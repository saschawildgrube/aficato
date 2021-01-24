<?php

	function IsValidLayoutID($strLayoutID)
	{
		if ($strLayoutID == '')
		{
			return false;
		}
		if (StringCheckCharSet($strLayoutID,CHARSET_ID) == false)
		{
			return false;	
		}	
		return true;
	}

	class CLayout extends CWebsiteSatellite
	{
		private $m_strLayoutID;
		
		function __construct(
			CWebSite $website,
			$strLayoutID)
		{
			parent::__construct($website, $strLayoutID);
						
			// Init values
			$this->m_strLayoutID = $strLayoutID;
		}
		
		function GetSatelliteType()
		{
			return 'Layout';	
		}
		
		
		final
		function Init()
		{
			return $this->OnInit();
		}
		
		function OnInit()
		{
			return true;
		}


	}

