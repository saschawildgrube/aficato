<?php

	function IsValidCookieName($strCookie)
	{
		if (StringCheckCharSet($strCookie,CHARSET_COOKIE) == false)
		{
			return false;	
		}
		if (StringCheckCharSet(StringSection($strCookie,0,1),CHARSET_NUMERIC) == true)
		{
			return false;	
		}
		return true;
	}
	



