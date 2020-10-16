<?php

	require_once("wdk_hash.inc");

	function IsMcryptSupported()
	{
		return function_exists("mcrypt_module_open");
	}

	function MakeInitVector($strAlgorithm = "rijndael-256")
	{
		if (!IsMcryptSupported())
		{
			return false;
		}
		$ea = new CErrorAmnesty();
		$ea->Start();
		$resource = mcrypt_module_open($strAlgorithm,"",MCRYPT_MODE_CBC,"");
		if ($resource == false)
		{
			return false;
		}
		$initvector = mcrypt_create_iv(mcrypt_enc_get_iv_size($resource), MCRYPT_RAND);
		mcrypt_module_close($resource);
		return $initvector;
	}

	function Encrypt($strData,$strPassword,$initvector,$strAlgorithm = "rijndael-256", $strHashMethod="sha1")
	{
		if (!IsMcryptSupported())
		{
			return false;
		}
		$ea = new CErrorAmnesty();
		$ea->Start();
		$resource = mcrypt_module_open($strAlgorithm,"",MCRYPT_MODE_CBC,"");
		if ($resource == false)
		{
			return false;
		}
		$nKeySize = mcrypt_enc_get_key_size($resource);
		if ($strHashMethod != "sha1")
		{
			$strPassword = MakeHash($strPassword,$initvector,$strHashMethod);
		}
		$key = StringSection(sha1($strPassword,TRUE), 0, $nKeySize);
		mcrypt_generic_init($resource, $key, $initvector);
		$strEncryprted = mcrypt_generic($resource,$strData."X");
		mcrypt_generic_deinit($resource);
		return $strEncryprted;	
	}

	function Decrypt($strEncrypted,$strPassword,$initvector,$strAlgorithm = "rijndael-256", $strHashMethod="sha1")
	{
		if (!IsMcryptSupported())
		{
			return false;
		}
		$ea = new CErrorAmnesty();
		$ea->Start();
		$resource = mcrypt_module_open($strAlgorithm,"",MCRYPT_MODE_CBC,"");
		if ($resource == false)
		{
			return false;
		}
		$nInitVectorSize = mcrypt_enc_get_iv_size($resource);
		if (strlen($initvector) != $nInitVectorSize)
		{
			return false;	
		}
		$nKeySize = mcrypt_enc_get_key_size($resource);
		if ($strHashMethod != "sha1")
		{
			$strPassword = MakeHash($strPassword,$initvector,$strHashMethod);
		}		
		$key = StringSection(sha1($strPassword,TRUE), 0, $nKeySize);
		$result = mcrypt_generic_init($resource, $key, $initvector);
		if( ($result < 0) || ($result === false))
		{
			return false;
		}
		$data = mdecrypt_generic($resource,$strEncrypted);
		mcrypt_generic_deinit($resource);
		$strData = trim($data,"\0");
		$strData = StringSection($strData,0,-1);
		return $strData;	
	}		


 