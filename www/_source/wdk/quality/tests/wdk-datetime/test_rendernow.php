<?php
	
	require_once(GetWDKDir()."wdk_datetime.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test RenderDateTimeNow and RenderTimeNow");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(false);
			date_default_timezone_set("UTC");
			return true;
		}

		function OnTest()
		{
			parent::OnTest();
			
			$strValue = RenderDateTimeNow();
			$this->Trace("RenderDateTimeNow() = \"$strValue\"");
			if (IsValidDateTime($strValue) != true)
			{
				$this->Trace("IsValidDateTime(\"$strValue\") returns false");
				return;
			}
			
			$strValue = RenderDateNow();
			$this->Trace("RenderDateNow() = \"$strValue\"");
			$strValue = $strValue . " 00:00:00";
			if (IsValidDateTime($strValue) != true)
			{
				$this->Trace("IsValidDateTime(\"$strValue\") returns false");
				return;
			}
			
			$strValue = RenderTimeNow();
			$this->Trace("RenderTimeNow() = \"$strValue\"");
			$strValue = "2001-09-11 ".$strValue;
			if (IsValidDateTime($strValue) != true)
			{
				$this->Trace("IsValidDateTime(\"$strValue\") returns false");
				return;
			}		
			$this->SetResult(true);	
		}
		
		
	}
	
	

		
