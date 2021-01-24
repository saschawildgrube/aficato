<?php
	
	require_once(GetWDKDir()."wdk_encryption-asymmetric.inc");
		
	class CEncryptAsymmetricUnitTest extends CUnitTest
	{
		public $m_aee;
		function __construct()
		{
			$this->m_aee = NULL;
			parent::__construct("WDK Asymmetric Encryption and Decryption");
		}
		
		function OnInit()
		{
			parent::OnInit();
			if (CheckSafeMode())
			{
				$this->Trace("Safe Mode is ON. So Shell* functions are not expected to work!");
				$this->SetActive(false);
			}
			if (GetGpgPath() == "")
			{
				$this->Trace("GetGpgPath() returned \"\". So the use of gpg is not configured.");
				$this->SetActive(false);
			}
			
			$this->m_aee = new CAsymmetricEncryptionEngineGPG(
				GetGpgPath(),
				GetTempDir().".gnupg/"
				);
			
			if ($this->m_aee->SelfTest() == false)
			{
				$this->Trace("Engine Self test failed");
				$this->Trace($this->m_aee->GetTraceLog());
				$this->SetResult(false);
				return false;	
			} 		
			
			$this->SetResult(true);
			return true;
		}


		function TestCase_EncryptAsymmetric(
			$strPayload)
		{
			$this->Trace("");
			$this->Trace("TestCase_EncryptAsymmetric");
			$this->Trace("Payload:");
			$this->Trace("\"$strPayload\"");


			$strRealName = "John Connor";
			$strComment = "Test";
			$strEmail = "john.connor@example.com";
			$strPassword = "1";

			$strPrivateKey = "";
			$strPublicKey = "";
			
			//$this->Trace("Original Payload: \"$strPayload\"");
			//$this->Trace("Chopped Payload: \"".chop($strPayload)."\"");
			//$this->Trace("Escaped Payload: \"".escapeshellarg($strPayload)."\"");
			//$this->Trace("Escaped Payload: \"".ShellEscapeArgumentString($strPayload)."\"");
			
			$bResult = $this->m_aee->CreateKeyPair(
				$strRealName,
				$strComment,	
				$strEmail,
				$strPassword,
				$strPrivateKey,
				$strPublicKey);
				
			$this->Trace("CreateKeyPair returned ".RenderBool($bResult));
			if ($bResult == false)
			{
				$this->Trace("Creating the key pair failed!");
				$this->SetResult(false);
				$this->Trace("Testcase FAILED!");	
				return;
			}
			if ($strPrivateKey == "")
			{
				$this->Trace("strPrivateKey is empty!");
				$this->SetResult(false);
				$this->Trace("Testcase FAILED!");	
				return;	
			}
			if ($strPublicKey == "")
			{
				$this->Trace("strPublicKey is empty!");
				$this->SetResult(false);
				$this->Trace("Testcase FAILED!");	
				return;	
			}

/*			
			$this->Trace("PrivateKey:");
			$this->Trace($strPrivateKey);
			$this->Trace("PublicKey:");
			$this->Trace($strPublicKey);
*/ 
			
			$strEncryptedPayload = "";
			$bResult = $this->m_aee->Encrypt(
				$strPayload,
				$strPublicKey,
				$strEncryptedPayload);

			$this->Trace("Encrypt returned ".RenderBool($bResult));
			if ($bResult == false)
			{
				$this->Trace("Encryption failed!");
				$this->SetResult(false);
				$this->Trace("Testcase FAILED!");	
				return;	
			}
			if ($strEncryptedPayload == "")
			{
				$this->Trace("strEncryptedPayload is empty!");
				$this->SetResult(false);
				$this->Trace("Testcase FAILED!");	
				return;	
			}
			
/*
			$this->Trace("EncryptedPayload:");
			$this->Trace($strEncryptedPayload);			
*/

			$strDecryptedMessage = "";
			$bResult = $this->m_aee->Decrypt(
				$strEncryptedPayload,
				$strPrivateKey,
				$strPassword,
				$strDecryptedPayload);

			$this->Trace("Decrypt returned ".RenderBool($bResult));
			if ($bResult == false)
			{
				$this->Trace("Decryption failed!");
				$this->SetResult(false);
				$this->Trace("Testcase FAILED!");	
				return;	
			}
			$this->Trace("Decrypted Payload:");
			$this->Trace("\"$strDecryptedPayload\"");
			
			if ($strDecryptedPayload != $strPayload)
			{
				$this->Trace("strDecryptedPayload does not match strPayload");
				$this->SetResult(false);
				$this->Trace("Testcase FAILED!");
				return;	
			}


			$this->Trace("Testcase PASSED!");
			$this->Trace("");
		}

		function OnCleanup()
		{
			if (is_object($this->m_aee) && ($this->GetVerbose() == true))
			{
				$this->Trace($this->m_aee->GetTraceLog());
			}
			return parent::OnCleanup();
		}		
	}
	
	

		
