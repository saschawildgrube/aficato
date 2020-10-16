<?php

	function MakeHash($payload,$strSeed='',$strHashMode='sha1')
	{
		$strPayload = '';
		if (is_array($payload))
		{
			ksort($payload);
			foreach ($payload as $value)
			{
				$strPayload .= "$value";
			}
		}
		else
		{
			$strPayload = "$payload";	
		}
		if ($strHashMode == 'bcrypt7')
		{
			return sha1(crypt($strPayload,"$2a$07$".StringSection(base64_encode(sha1($strSeed)),22)));
		}
		if ($strHashMode == 'bcrypt12')   
		{
			return sha1(crypt($strPayload,"$2a$12$".StringSection(base64_encode(sha1($strSeed)),22)));
		}
		return sha1($strSeed.$strPayload);
	}
		
	function MakePasswordHashSeed()
	{
		return sha1('PASSWORDHASHSEED'.microtime(true).rand());	
	}
	

	function MakePasswordHash($strPassword,$strPasswordHashSeed)
	{
		return MakeHash($strPassword,$strPasswordHashSeed,"bcrypt12");
	}	
 

	function VerifyPassword($strPassword,$strPasswordHash,$strPasswordHashSeed)
	{
		if (MakeHash($strPassword,$strPasswordHashSeed,"sha1") == $strPasswordHash)
		{
			return true;	
		}	
		if (MakeHash($strPassword,$strPasswordHashSeed,"bcrypt7") == $strPasswordHash)
		{
			return true;	
		}	
		if (MakeHash($strPassword,$strPasswordHashSeed,"bcrypt12") == $strPasswordHash)
		{
			return true;	
		}	
		return false;
	}



