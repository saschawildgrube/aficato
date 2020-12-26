<?php

	function MakeHash($payload,$strSeed='',$strMethod='sha1')
	{
		$strPayload = '';
		if (is_array($payload))
		{
			ksort($payload);
			foreach ($payload as $value)
			{
				if (is_array($value))
				{
					$strPayload .= MakeHash($value,$strSeed,$strMethod);
				}
				else
				{
					$strPayload .= ''.$value;
				}
			}
		}
		else
		{
			$strPayload = ''.$payload;	
		}
		if ($strMethod == 'bcrypt7')
		{
			return sha1(crypt($strPayload,'$2a$07$'.StringSection(base64_encode(sha1($strSeed)),22)));
		}
		if ($strMethod == 'bcrypt12')   
		{
			return sha1(crypt($strPayload,'$2a$12$'.StringSection(base64_encode(sha1($strSeed)),22)));
		}
		return sha1($strSeed.$strPayload);
	}
		
	function MakePasswordHashSeed()
	{
		return sha1('PASSWORDHASHSEED'.microtime(true).rand());	
	}
	

	function MakePasswordHash($strPassword,$strPasswordHashSeed)
	{
		return MakeHash($strPassword,$strPasswordHashSeed,'bcrypt12');
	}	
 

	function VerifyPassword($strPassword,$strPasswordHash,$strPasswordHashSeed)
	{
		if (MakeHash($strPassword,$strPasswordHashSeed,'sha1') == $strPasswordHash)
		{
			return true;	
		}	
		if (MakeHash($strPassword,$strPasswordHashSeed,'bcrypt7') == $strPasswordHash)
		{
			return true;	
		}	
		if (MakeHash($strPassword,$strPasswordHashSeed,'bcrypt12') == $strPasswordHash)
		{
			return true;	
		}	
		return false;
	}



