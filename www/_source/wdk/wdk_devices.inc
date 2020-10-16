<?php

	function GetDeviceType()
	{
		// from https://github.com/serbanghita/Mobile-Detect:
		require_once(GetWDKDir()."3rdparty/mobiledetect/mobile_detect.php");  
			
		$mobiledetect = new Mobile_Detect;
		if ($mobiledetect->isTablet() == true)
		{
			return "tablet";	
		}
		if ($mobiledetect->isMobile() == true)
		{
			return "mobile";	
		}
		return "computer";
	}
	
	function IsDeviceMobile()
	{
		if (GetDeviceType() == "mobile")
		{
			return true;
		}
		return false;
	}

	function IsDeviceTablet()
	{
		if (GetDeviceType() == "tablet")
		{
			return true;
		}
		return false;
	}
	
	function IsDeviceComputer()
	{
		if (GetDeviceType() == "computer")
		{
			return true;
		}
		return false;
	}

	function IsDeviceWatch()
	{
		if (GetDeviceType() == "watch")
		{
			return true;
		}
		return false;
	}

	function IsDeviceTV()
	{
		if (GetDeviceType() == "tv")
		{
			return true;
		}
		return false;
	}


