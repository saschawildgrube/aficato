<?php
	
	require_once(GetWDKDir()."wdk_encryption-symmetric.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK Symmetric Encryption and Decryption");
		}
		

		function CallbackInit()
		{
			$this->SetResult(true);
			if (!IsMcryptSupported())
			{
				$this->Trace("To install mcrypt support:");
				$this->Trace("sudo apt-get install php5-mcrypt");
				$this->Trace("sudo /etc/init.d/apache2 restart");
				$this->Trace("");
				$this->SetActive(false);
			}
			return parent::CallbackInit();	
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

		function CallbackTest()
		{
			parent::CallbackTest();
			
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
	
	

		
