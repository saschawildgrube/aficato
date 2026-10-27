<?php
	
	require_once(GetWDKDir()."wdk_encryption-symmetric.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK Symmetric Encryption and Decryption");
		}
		

		function OnInit()
		{
			$this->SetResult(true);
			return parent::OnInit();	
		}
		
		
		function TestCase_EncryptDecrypt($strPayload,$strPassword,$bWrongPassword = false, $strHashMethod="bcrypt7")
		{
			$this->Trace("TestCase_EncryptDecrypt BEGIN");
			
			$stopwatch = new CStopWatch();
			
			$stopwatch->Start();
			$initvector = MakeInitVector();
			$stopwatch->Stop();
			$this->Trace("MakeInitVector() speed (seconds): ".RenderNumber($stopwatch->GetSeconds(),3));
			$stopwatch->Reset();
			
			$this->Trace("strPayload = \"$strPayload\"");
			$this->Trace("strPassword = \"$strPassword\"");
			$this->Trace("initvector = \"".RenderHex($initvector)."\"");
			$this->Trace("Hash method = \"".$strHashMethod."\"");
			
			$stopwatch->Start();
			$strEncrypted = Encrypt($strPayload,$strPassword,$initvector,"rijndael-256",$strHashMethod);
			$stopwatch->Stop();
			$this->Trace("Encryption speed (seconds): ".RenderNumber($stopwatch->GetSeconds(),3));
			$stopwatch->Reset();
			
			$this->Trace("strEncrypted = \"".RenderHex($strEncrypted)."\"");
			
			
			$stopwatch->Start();
			$strDecrypted = Decrypt(
				$strEncrypted,
				($bWrongPassword)?("WRONG"):($strPassword),
				$initvector,
				"rijndael-256",
				$strHashMethod);
			$stopwatch->Stop();
			$this->Trace("Decryption speed (seconds): ".RenderNumber($stopwatch->GetSeconds(),3));
			$stopwatch->Reset();
						
			$this->Trace("strPayload   = \"$strPayload\"");
			//$this->Trace("strPayload   = \"".RenderHex($strPayload)."\"");
			if ($bWrongPassword  == false)
			{
				$this->Trace("strDecrypted = \"$strDecrypted\"");
			}
			else
			{
				$this->Trace("strDecrypted (hex) = \"".RenderHex($strDecrypted)."\"");
			}

			if ($bWrongPassword  == false)
			{
				if ($strPayload != $strDecrypted) 
				{
					$this->Trace("TESTCASE FAILED"); 
					$this->SetResult(false);
				}
			}
			else
			{
				if ($strPayload == $strDecrypted) 
				{
					$this->Trace("TESTCASE FAILED"); 
					$this->SetResult(false);
				}
				
			}
			$this->ProfilingTrace();

			$this->Trace("TestCase_EncryptDecrypt END");
			$this->Trace("");
				
		}

		function CheckCompatibility($bResult,$strDescription)
		{
			if (!$bResult)
			{
				$this->Trace("COMPATIBILITY FAILED: ".$strDescription);
				$this->SetResult(false);
			}
		}

		function TestCase_LegacyCompatibility()
		{
			$bMcrypt = IsMcryptSupported();
			if (!$bMcrypt)
			{
				$this->Trace("Legacy cross-check skipped: run this test also on a host with mcrypt.");
			}
			foreach (array("rijndael-128"=>16,"rijndael-192"=>24,"rijndael-256"=>32) as $strAlgorithm=>$nBlockSize)
			{
				$iv = substr(str_repeat("0123456789abcdef",2),0,$nBlockSize);
				$this->CheckCompatibility(strlen(MakeInitVector($strAlgorithm)) === $nBlockSize,"IV size");
				$this->CheckCompatibility(Encrypt("data","secret","short",$strAlgorithm) === false,"Invalid encryption IV");
				$this->CheckCompatibility(Decrypt("data","secret","short",$strAlgorithm) === false,"Invalid decryption IV");
				$payloads = array("","X","a\0b\0","\0leading","Gr".chr(195).chr(188).chr(195).chr(159)."e",
					str_repeat("a",$nBlockSize-1),str_repeat("a",$nBlockSize),
					str_repeat("a",$nBlockSize+1),str_repeat("a",2*$nBlockSize-1),
					str_repeat("a",2*$nBlockSize),str_repeat("a",1000));
				foreach (array("sha1","bcrypt7","bcrypt12") as $strHashMethod)
				{
					foreach ($payloads as $strPayload)
					{
						$strPassword = "secret";
						$strEncrypted = Encrypt($strPayload,$strPassword,$iv,$strAlgorithm,$strHashMethod);
						$strExpected = StringSection(trim($strPayload."X","\0"),0,-1);
						$this->CheckCompatibility(
							Decrypt($strEncrypted,$strPassword,$iv,$strAlgorithm,$strHashMethod) === $strExpected,
							$strAlgorithm." ".$strHashMethod." round trip, length ".strlen($strPayload));
						if (!$bMcrypt)
						{
							continue;
						}
						// Use the original mcrypt API and original WDK key derivation.
						$ea = new CErrorAmnesty();
						$ea->Start();
						$resource = mcrypt_module_open($strAlgorithm,"",MCRYPT_MODE_CBC,"");
						$password = ($strHashMethod == "sha1") ? $strPassword : MakeHash($strPassword,$iv,$strHashMethod);
						$key = StringSection(sha1($password,TRUE),0,mcrypt_enc_get_key_size($resource));
						mcrypt_generic_init($resource,$key,$iv);
						$strLegacy = mcrypt_generic($resource,$strPayload."X");
						mcrypt_generic_deinit($resource);
						$this->CheckCompatibility($strEncrypted === $strLegacy,"Identical ciphertext: ".$strAlgorithm." ".$strHashMethod);
						$this->CheckCompatibility(
							Decrypt($strLegacy,$strPassword,$iv,$strAlgorithm,$strHashMethod) === $strExpected,
							"Old ciphertext read by new implementation");
						mcrypt_generic_init($resource,$key,$iv);
						$strLegacyPlain = StringSection(trim(mdecrypt_generic($resource,$strEncrypted),"\0"),0,-1);
						mcrypt_generic_deinit($resource);
						mcrypt_module_close($resource);
						$this->CheckCompatibility($strLegacyPlain === $strExpected,"New ciphertext read by old implementation");
					}
				}
			}
		}

		function OnTest()
		{
			parent::OnTest();
			$this->TestCase_LegacyCompatibility();
			
			$this->TestCase_EncryptDecrypt(
				"This is some highly confidential text.",
				"secret");

			$this->TestCase_EncryptDecrypt(
				"This is some highly confidential text.",
				"secret",
				false,
				"sha1");


			$this->TestCase_EncryptDecrypt(
				StringRepeat("1234567890",100),
				"secret");

			$this->TestCase_EncryptDecrypt(
				StringRepeat("1234567890",100),
				"secret",
				true);
			
		}
		
		
	}
	
	

		
