<?php

	
	define("SESSION_MODE_NONE",							0);
	define("SESSION_MODE_LOCAL_COOKIE",			1);
	define("SESSION_MODE_SERVER_URL",				2);
	define("SESSION_MODE_SERVER_COOKIE",		3);
	define("SESSION_MODE_SERVER_COMBINED",	4);
	
	
	function IsValidSessionMode($nSessionMode) 
	{
		if (!is_int($nSessionMode))
		{
			return false;	
		}
		if ($nSessionMode < SESSION_MODE_NONE)	
		{
			return false;
		}
		if ($nSessionMode > SESSION_MODE_SERVER_COMBINED)
		{
			return false;
		}
		return true;
	}
	
	function IsSessionModeLocal($nSessionMode)
	{
		if ($nSessionMode == SESSION_MODE_LOCAL_COOKIE) 
		{
			return true;	
		}
		return false;
	}

	function IsSessionModeServer($nSessionMode)
	{
		if (		($nSessionMode == SESSION_MODE_SERVER_COOKIE)
				||	($nSessionMode == SESSION_MODE_SERVER_URL)
				||	($nSessionMode == SESSION_MODE_SERVER_COMBINED))
		{
			return true;	
		}
		return false;
	}

	

