<?php

	require_once("wdk_hash.inc");
	require_once(__DIR__."/phpseclib/Crypt/Base.php");
	require_once(__DIR__."/phpseclib/Crypt/Rijndael.php");

	function GetSymmetricBlockSize($strAlgorithm)
	{
		switch ($strAlgorithm)
		{
			case "rijndael-128": return 16;
			case "rijndael-192": return 24;
			case "rijndael-256": return 32;
		}
		return false;
	}

	function MakeSymmetricCipher($strPassword,$initvector,$strAlgorithm,$strHashMethod)
	{
		$nBlockSize = GetSymmetricBlockSize($strAlgorithm);
		if (($nBlockSize === false) || (strlen($initvector) != $nBlockSize))
		{
			return false;
		}
		if ($strHashMethod != "sha1")
		{
			$strPassword = MakeHash($strPassword,$initvector,$strHashMethod);
		}
		// Preserve the original StringSection call, including its UTF-8 semantics.
		$key = StringSection(sha1($strPassword,TRUE), 0, 32);
		$cipher = new \phpseclib\Crypt\Rijndael(\phpseclib\Crypt\Base::MODE_CBC);
		$cipher->setPreferredEngine(\phpseclib\Crypt\Base::ENGINE_INTERNAL);
		$cipher->setBlockLength($nBlockSize * 8);
		// mcrypt_generic_init rounds a 20-byte SHA-1 key up to 24 bytes.
		$cipher->setKey(str_pad($key,24,"\0"));
		$cipher->setIV($initvector);
		$cipher->disablePadding();
		return $cipher;
	}

	function IsMcryptSupported()
	{
		return function_exists("mcrypt_module_open");
	}

	function MakeInitVector($strAlgorithm = "rijndael-256")
	{
		$nBlockSize = GetSymmetricBlockSize($strAlgorithm);
		if ($nBlockSize !== false)
		{
			try
			{
				return random_bytes($nBlockSize);
			}
			catch (\Exception $exception)
			{
				return false;
			}
		}
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
		$nBlockSize = GetSymmetricBlockSize($strAlgorithm);
		if ($nBlockSize !== false)
		{
			$cipher = MakeSymmetricCipher($strPassword,$initvector,$strAlgorithm,$strHashMethod);
			if ($cipher === false)
			{
				return false;
			}
			// mcrypt uses zero padding, without an extra block for aligned input.
			$strData .= "X";
			$strData .= str_repeat("\0",($nBlockSize - strlen($strData) % $nBlockSize) % $nBlockSize);
			return $cipher->encrypt($strData);
		}
		// Retain support for other historical algorithms when mcrypt is installed.
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
		$nBlockSize = GetSymmetricBlockSize($strAlgorithm);
		if ($nBlockSize !== false)
		{
			$cipher = MakeSymmetricCipher($strPassword,$initvector,$strAlgorithm,$strHashMethod);
			if (($cipher === false) || ($strEncrypted === ""))
			{
				return false;
			}
			// Match mcrypt's zero extension of incomplete ciphertext blocks.
			$strEncrypted .= str_repeat("\0",($nBlockSize - strlen($strEncrypted) % $nBlockSize) % $nBlockSize);
			$data = $cipher->decrypt($strEncrypted);
			// Keep legacy trimming and marker removal, even for leading NULs.
			return StringSection(trim($data,"\0"),0,-1);
		}
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


 