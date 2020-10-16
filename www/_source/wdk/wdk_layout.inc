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
			parent::__construct($website, 'Layout '.$strLayoutID);
						
			// Init values
			$this->m_strLayoutID = $strLayoutID;
		}
		
		final
		function Init()
		{
			return $this->CallbackInit();
		}
		
		function CallbackInit()
		{
			return true;
		}


	}

