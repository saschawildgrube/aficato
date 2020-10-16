<?php

	require_once(GetWDKDir()."wdk_shell.inc");
	require_once(GetWDKDir()."wdk_random.inc");
	require_once(GetWDKDir()."wdk_hash.inc");
	require_once(GetWDKDir()."wdk_mail.inc");

	interface IAsymmetricEncryptionEngine
	{
		public
		function SelfTest();
		
		public
		function CreateKeyPair(
			$strRealName,
			$strComment,
			$strEmail,
			$strPassword,
			&$strPrivateKey,
			&$strPublicKey);
		
		public
		function Encrypt(
			$strPayload,
			$strPublicKey,
			&$strEncryptedPayload);
		
		public
		function Decrypt(
			$strEncryptedPayload,
			$strPrivateKey,
			$strPassword,
			&$strPayload);
		
	}
	





	/*
	
		Some sources that were helpful while developing the gpg wrapper:
		http://www.alt-php-faq.org/local/65/
		http://cvs.gnupg.org/cgi-bin/viewcvs.cgi/trunk/doc/DETAILS?rev=5109&root=GnuPG&view=markup
		http://devzone.zend.com/article/1265

	*/

	class CAsymmetricEncryptionEngineGPG
		extends CTraceLog
		implements IAsymmetricEncryptionEngine
	{
		private $m_strEnginePath;
		private $m_strWorkingDir;
		private $m_strRandomSeedFileOption;
				
		function __construct(
			$strEnginePath,
			$strWorkingDir)
		{
			parent::__construct();
			CheckSafeMode(true);
			$this->m_strEnginePath = $strEnginePath;
			$this->m_strWorkingDir = $strWorkingDir;
			//$this->m_strRandomSeedFileOption = "";
			$this->m_strRandomSeedFileOption = "--no-random-seed-file";
		}
		
		function Trace($strTrace)
		{
			parent::Trace($strTrace);
		}
		
		function SelfTest()
		{
			if (IsDirectoryReadWriteAccess($this->m_strWorkingDir) == false)
			{
				$this->Trace("SelfTest: IsDirectoryReadWriteAccess(\"$this->m_strWorkingDir\") returned false.");
				return false;	
			}
			
			
			$arrayResult = array();
			$bResult = ShellExecute(
				$this->m_strEnginePath,
				array(
					"--help",
					$this->m_strRandomSeedFileOption),
				array(),
				"",
				$arrayResult);
			if ($bResult == false)
			{
				$this->Trace("SelfTest: ShellExecute returned false.");
				return false;	
			}
			$bSupportedVersion = false;
			
			$arrayVersions = array(
				"gpg (GnuPG) 1.4.2",
				"gpg (GnuPG) 1.4.5",
				"gpg (GnuPG) 1.4.10",
				"gpg (GnuPG) 2.0.4-svn0");
			
			
			
			foreach ($arrayVersions as $strVersion)
			{
				if (FindString($arrayResult["stdout"],$strVersion) != -1)
				{
					$bSupportedVersion = true;
				}				
			}	
				
			if ($bSupportedVersion == false)
			{
				$this->Trace("SelfTest: gpg version could not be determined.");
				return false;
			}
			return true;	
		}
		
		
		function CreateKeyPair(
			$strRealName,
			$strComment,
			$strEmail,
			$strPassword,
			&$strPrivateKey,
			&$strPublicKey)
		{
			$strFileNameCore = MakeNumericToken(10);
		
			$strKeyID = "";
			$bResult = $this->GPG_CreateKeyPair(
				$strFileNameCore,
				$strRealName,
				$strComment,
				$strEmail,
				$strPassword,
				$strKeyID);
			if ($bResult != true)
			{
				$this->Trace("CreateKeyPair: GPG_CreateKeyPair returned false.");
				$this->GPG_DeleteFiles($strFileNameCore);
				return false;	
			}
			
			$strPrivateKey = "";
			$strPublicKey = "";
			$bResult = $this->GPG_ExportKeyPair(
				$strFileNameCore,
				$strKeyID,
				$strPrivateKey,
				$strPublicKey);
				
			$this->GPG_DeleteFiles($strFileNameCore);
			
			if ($bResult != true)
			{
				$this->Trace("CreateKeyPair: GPG_ExportKeyPair returned false.");
				return false;	
			}
			
			if ($strPrivateKey == "")
			{
				$this->Trace("CreateKeyPair: GPG_ExportKeyPair returned empty private key.");
				return false;	
			}
			if ($strPublicKey == "")
			{
				$this->Trace("CreateKeyPair: GPG_ExportKeyPair returned empty public key.");
				return false;	
			}
			$this->Trace("CreateKeyPair: Successfully completed.");
			return true;
		}
		
		function Encrypt(
			$strPayload,
			$strPublicKey,
			&$strEncryptedPayload)
		{
			$strFileNameCore = MakeNumericToken(10);
			$strPublicKeyID = "";
			$bResult = $this->GPG_ImportKey(
				$strFileNameCore,
				$strPublicKey,
				$strPublicKeyID);
			if ($bResult != true)
			{
				$this->Trace("Encrypt: GPG_ImportKey returned false.");
				return false;	
			}
			
			$bResult = $this->GPG_Encrypt(
				$strFileNameCore,
				$strPayload,
				$strPublicKeyID,
				$strEncryptedPayload);
			
			$this->GPG_DeleteFiles(
				$strFileNameCore);
				
			if ($bResult != true)
			{
				$this->Trace("Encrypt: GPG_Encrypt returned false.");
				return false;	
			}

			return true;
		}
		
		function Decrypt(
			$strEncryptedPayload,
			$strPrivateKey,
			$strPassword,
			&$strPayload)
		{
			$strPayload = "";
			
			$strFileNameCore = MakeNumericToken(10);
			$strPrivateKeyID = "";
			
			// 1. Import private key to key ring
			$bResult = $this->GPG_ImportKey(
				$strFileNameCore,
				$strPrivateKey,
				$strPrivateKeyID);
			if ($bResult != true)
			{
				$this->GPG_DeleteFiles($strFileNameCore);
				$this->Trace("Decrypt: GPG_ImportKey returned false.");
				return false;	
			}
				
			// 2. Decrypt the message using the private key
			$bResult = $this->GPG_Decrypt(
				$strFileNameCore,
				$strEncryptedPayload,
				$strPrivateKeyID,
				$strPassword,
				$strPayload);

			// 3. Delete the key file
			$this->GPG_DeleteFiles($strFileNameCore);
				
			if ($bResult == false)
			{
				$this->Trace("Decrypt: GPG_Decrypt returned false.");
				return false;	
			}
			$this->Trace("Decrypt: Successfully completed.");
			return true;	
		}
			
		function GPG_ShellExecute(
			$arrayParams,
			$strStdin,
			&$arrayResult)
		{
			
			$this->Trace("GPG_ShellExecute");
			foreach ($arrayParams as $strParam) 
			{
				$this->Trace("GPG_ShellExecute: param: ".$strParam);	
			}
			
			$arrayResult = array();
			$bResult = ShellExecute(
				$this->m_strEnginePath,
				$arrayParams,
				array("GNUPGHOME" => $this->m_strWorkingDir), 
				$strStdin,
				$arrayResult);
		
			$this->Trace("GPG_ShellExecute: Command:");
			$this->Trace($arrayResult["command"]);
			$this->Trace("GPG_ShellExecute: Result:");
			$this->Trace(RenderBool($bResult));
			$this->Trace("GPG_ShellExecute: stdout:");
			$this->Trace($arrayResult["stdout"]);
			$this->Trace("GPG_ShellExecute: stderr:");
			$this->Trace($arrayResult["stderr"]);
				
			return $bResult;
		}

		protected
		function GPG_MakeFilePathUnattendedKeyGenConfig($strFileNameCore)
		{
			return $this->m_strWorkingDir . $strFileNameCore . "_keygen.cfg";	
		}

		protected		
		function GPG_MakeFilePathPrivateKeyRing($strFileNameCore)
		{
			return $this->m_strWorkingDir . $strFileNameCore . "_secring.gpg";	
		}
		
		protected
		function GPG_MakeFilePathPublicKeyRing($strFileNameCore)
		{
			return $this->m_strWorkingDir . $strFileNameCore . "_pubring.gpg";	
		}
		
		protected
		function GPG_MakeFilePathTrustDB($strFileNameCore)
		{
			return $this->m_strWorkingDir . $strFileNameCore . "_trustdb.gpg";	
		}		


		protected
		function GPG_MakeFilePathImport($strFileNameCore)
		{
			return $this->m_strWorkingDir . $strFileNameCore . "_import.txt";	
		}
		
		protected
		function GPG_MakeFilePathMessage($strFileNameCore)
		{
			return $this->m_strWorkingDir . $strFileNameCore . "_message.txt";	
		}		
		
		protected
		function GPG_DeleteFiles($strFileNameCore)
		{
			$strFilePathPrivateKeyRing = $this->GPG_MakeFilePathPrivateKeyRing($strFileNameCore);
			$strFilePathPublicKeyRing = $this->GPG_MakeFilePathPublicKeyRing($strFileNameCore);
			$strFilePathTrustDB = $this->GPG_MakeFilePathTrustDB($strFileNameCore);
			DeleteFile($strFilePathPrivateKeyRing,true);
			DeleteFile($strFilePathPublicKeyRing,true);
			DeleteFile($strFilePathPublicKeyRing."~",true);
			DeleteFile($strFilePathTrustDB,true);
			if (	IsFile($strFilePathPrivateKeyRing,true)
				|| IsFile($strFilePathPublicKeyRing,true)
				|| IsFile($strFilePathPublicKeyRing."~",true)
				|| IsFile($strFilePathTrustDB,true))
			{
				$this->Trace("GPG_DeleteFiles: One or more files still exist!");
				return false;		
			}
			$this->Trace("GPG_DeleteFiles: Successfully completed.");
			return true;
		}
		
		protected
		function GPG_CreateKeyPair(
				$strFileNameCore,
				$strRealName,
				$strComment,
				$strEmail,
				$strPassword,
				&$strKeyID)
		{
			if (StringLength($strRealName) < 5)
			{
				$this->Trace("GPG_CreateKeyPair: real name must be at least 5 characters long!");
				return false;	
			}
			if ($strEmail != "")
			{
				if (!IsValidEmail($strEmail))
				{
					$this->Trace("GPG_CreateKeyPair: invalid email: \"$strEmail\"");
					return false;	
				}	
			}
			if ($strPassword == "")
			{
				$this->Trace("GPG_CreateKeyPair: Password missing.");
				return false;	
			}
			
			$strFilePath = "";
			
			$bResult = $this->GPG_MakeUnattendedKeyGenFile(
				$strFileNameCore,
				$strRealName,
				$strComment,
				$strEmail,
				$strPassword,
				$strFilePath);
			if ($bResult != true)
			{
				$this->Trace("GPG_CreateKeyPair: GPG_MakeUnattendedKeyGenFile returned false");
				return false;	
			}
			
			
			
			$bResult = $this->GPG_ShellExecute(
				array(
					"--batch",
					"--no-default-keyring",
					$this->m_strRandomSeedFileOption,
					"--keyring",
					$this->GPG_MakeFilePathPublicKeyRing($strFileNameCore),
					"--secret-keyring",
					$this->GPG_MakeFilePathPrivateKeyRing($strFileNameCore),
					"--trustdb",
					$this->GPG_MakeFilePathTrustDB($strFileNameCore),
					"--gen-key",
					$strFilePath
					),
				"",
				$arrayResult);

			DeleteFile($strFilePath,true);
				
			if ($bResult == false)
			{
				$this->Trace("GPG_CreateKeyPair: GPG_ShellExecute returned false.");
				return false;
			}
			
			// "public and secret key created and signed."
			$strOutput = $arrayResult["stderr"];
			$nPos = FindString($strOutput,"gpg: key ");
			if ($nPos == -1)
			{
				$this->Trace("GPG_CreateKeyPair: Key ID not found in shell output.");	
				return false;
			}
			
			$strKeyID = StringSection($strOutput,$nPos+9,8);
			$this->Trace("GPG_CreateKeyPair: Key ID: \"$strKeyID\""); 
			
			if ($this->GPG_CheckKeyID($strKeyID) != true)
			{
				$this->Trace("GPG_CreateKeyPair: Invalid key id.");	
				return false;	
			}
			$this->Trace("GPG_CreateKeyPair: Sucessfully completed.");	
			return true;
		}
		
		protected
		function GPG_ExportKeyPair(
			$strFileNameCore,
			$strKeyID,
			&$strPrivateKey,
			&$strPublicKey)
		{
			
			
			$bResult = $this->GPG_ShellExecute(
				array(
					"--batch",
					"--armor",
					"--no-default-keyring",
					$this->m_strRandomSeedFileOption,
					"--keyring",
					$this->GPG_MakeFilePathPublicKeyRing($strFileNameCore),
					"--secret-keyring",
					$this->GPG_MakeFilePathPrivateKeyRing($strFileNameCore),
					"--trustdb",
					$this->GPG_MakeFilePathTrustDB($strFileNameCore),
					"--export-secret-key",
					$strKeyID
					),
				"",
				$arrayResult);
				
			if ($bResult != true)
			{
				$this->Trace("GPG_ExportKeyPair: GPG_ShellExecute returned false.");
				return false;	
			}
				
			$strPrivateKeyTemp = $arrayResult["stdout"];
			if ($strPrivateKeyTemp == "")
			{
				$this->Trace("GPG_ExportKeyPair: stdout was empty!.");
				return false;
			}
			if (FindString($strPrivateKeyTemp,"-----BEGIN PGP PRIVATE KEY BLOCK-----") == -1)
			{
				$this->Trace("GPG_ExportKeyPair: \"-----BEGIN PGP PRIVATE KEY BLOCK-----\" not found.");				
				return false;
			}
			if (FindString($strPrivateKeyTemp,"-----END PGP PRIVATE KEY BLOCK-----") == -1)
			{
				$this->Trace("GPG_ExportKeyPair: \"-----END PGP PRIVATE KEY BLOCK-----\" not found.");				
				return false;
			}
			$strPrivateKey = $strPrivateKeyTemp;
			
			$bResult = $this->GPG_ShellExecute(
				array(
					"--batch",
					"--armor",
					"--no-default-keyring",
					$this->m_strRandomSeedFileOption,
					"--keyring",
					$this->GPG_MakeFilePathPublicKeyRing($strFileNameCore),
					"--secret-keyring",
					$this->GPG_MakeFilePathPrivateKeyRing($strFileNameCore),
					"--trustdb",
					$this->GPG_MakeFilePathTrustDB($strFileNameCore),
					"--export",
					$strKeyID
					),
				"",
				$arrayResult);
				
			if ($bResult != true)
			{
				$this->Trace("GPG_ExportKeyPair: GPG_ShellExecute returned false.");
				return false;	
			}
				
			$strPublicKeyTemp = $arrayResult["stdout"]; 
			if ($strPublicKeyTemp == "")
			{
				$this->Trace("GPG_ExportKeyPair: stdout was empty!.");
				return false;
			}
			if (FindString($strPublicKeyTemp,"-----BEGIN PGP PUBLIC KEY BLOCK-----") == -1)
			{
				$this->Trace("GPG_ExportKeyPair: \"-----BEGIN PGP PUBLIC KEY BLOCK-----\" not found.");				
				return false;
			}
			if (FindString($strPublicKeyTemp,"-----END PGP PUBLIC KEY BLOCK-----") == -1)
			{
				$this->Trace("GPG_ExportKeyPair: \"-----END PGP PUBLIC KEY BLOCK-----\" not found.");				
				return false;
			}
			$strPublicKey = $strPublicKeyTemp;

			$this->Trace("GPG_ExportKeyPair: Sucessfully completed.");	
			return true;
		}
		
		protected
		function GPG_ImportKey(
				$strFileNameCore,
				$strKey,
				&$strKeyID)
		{
			$strFilePath = $this->GPG_MakeFilePathImport($strFileNameCore);
			
			$this->Trace("GPG_ImportKey: GPG_MakeFilePathImport returned: $strFilePath");
			
			$strCommand = "";
			$strCommand .= "cat >$strFilePath <<EOF\n"; 
     		$strCommand .= "$strKey\n" ;
			$strCommand .= "EOF\n" ;
			
			$bResult = ShellExecute(
				$strCommand,
				array(),
				array(), 
				"",
				$arrayResult);
				
			$this->Trace($arrayResult);
				
			if ($bResult != true)
			{
				$this->Trace("GPG_ImportKey: ShellExecute (cat) returned false.");
				return false;	
			}
			if(!IsFile($strFilePath,true))
			{
				$this->Trace("GPG_ImportKey: \"$strFilePath\" is not a file.");
				return false;			
			}


			$bResult = $this->GPG_ShellExecute(
				array(
					"--batch",
					"--no-default-keyring",
					$this->m_strRandomSeedFileOption,
					"--keyring",
					$this->GPG_MakeFilePathPublicKeyRing($strFileNameCore),
					"--secret-keyring",
					$this->GPG_MakeFilePathPrivateKeyRing($strFileNameCore),
					"--trustdb",
					$this->GPG_MakeFilePathTrustDB($strFileNameCore),
					"--import",
					$strFilePath
					),
				"",	
				$arrayResult);
				
			DeleteFile($strFilePath,true);
		
			if ($bResult != true)
			{
				$this->Trace("GPG_ImportKey: ShellExecute returned false.");
				return false;	
			}
			
			$strOutput = $arrayResult["stderr"];
			
			$nPos = FindString($strOutput,"gpg: key ");
			if ($nPos==-1)
			{
				$this->Trace("GPG_ImportKey: Key ID not found");
				return false;	
			}
			
			$strKeyIDTemp = StringSection($strOutput,$nPos+9,8);
			
			$this->Trace("GPG_ImportKey: Key ID: \"$strKeyIDTemp\"");
			if ($this->GPG_CheckKeyID($strKeyIDTemp) != true)
			{
				$this->Trace("GPG_ImportKey: Invalid key id.");	
				return false;	
			}

			$strKeyID = $strKeyIDTemp;
			
			$this->Trace("GPG_ImportKey: Sucessfully completed.");	
			return true;	
		}
		
		
		protected
		function GPG_MakeUnattendedKeyGenFile(
			$strFileName,
			$strRealName,
			$strComment,
			$strEmail,
			$strPassword,
			&$strFilePath)
		{
			$strFilePath = $this->GPG_MakeFilePathUnattendedKeyGenConfig($strFileName);
			
			$this->Trace("GPG_MakeUnattendedKeyGenFile: GPG_MakeFilePathUnattendedKeyGenConfig returned: $strFilePath");
			
			$strCommand = "";
			$strCommand .="cat >$strFilePath <<EOF\n"; 
     		$strCommand .="%ech"."o Generating a key\n" ;
     		$strCommand .="Key-Type: DSA\n" ;
     		$strCommand .="Key-Length: 1024\n"; 
     		$strCommand .="Subkey-Type: ELG-E\n"; 
     		$strCommand .="Subkey-Length: 1024\n" ;
     		$strCommand .="Name-Real: $strRealName\n";
     		if ($strComment!="") 
     		{
     			$strCommand .="Name-Comment: $strComment\n" ;
     		}
     		$strCommand .="Name-Email: $strEmail\n" ;
     		$strCommand .="Expire-Date: 0\n" ;
     		$strCommand .="Passphrase: $strPassword\n";
     		$strCommand .="%commit\n";
			$strCommand .="EOF\n" ;
			
			$bResult = ShellExecute(
				$strCommand,
				array(),
				array(), 
				"",
				$arrayResult);
				
			$this->Trace($arrayResult);
				
			if ($bResult != true)
			{
				$this->Trace("GPG_MakeUnattendedKeyGenFile: ShellExecute (cat) returned false.");
				return false;	
			}
			if(!IsFile($strFilePath,true))
			{
				$this->Trace("GPG_MakeUnattendedKeyGenFile: \"$strFilePath\" is not a file.");
				return false;			
			}
			return true;		
		}

		protected
		function GPG_MakeMessageFile(
			$strFileName,
			$strEncryptedPayload,
			&$strFilePath)
		{
			$strFilePath = $this->GPG_MakeFilePathMessage($strFileName);
			
			$this->Trace("GPG_MakeMessageFile: GPG_MakeFilePathMessage returned: $strFilePath");
			
			$strCommand = "";
			$strCommand .="cat >$strFilePath <<EOF\n"; 
     		$strCommand .= $strEncryptedPayload."\n" ;
			$strCommand .="EOF\n" ;
			
			$bResult = ShellExecute(
				$strCommand,
				array(),
				array(), 
				"",
				$arrayResult);
				
			$this->Trace($arrayResult);
				
			if ($bResult != true)
			{
				$this->Trace("GPG_MakeMessageFile: ShellExecute (cat) returned false.");
				return false;	
			}
			if(!IsFile($strFilePath,true))
			{
				$this->Trace("GPG_MakeMessageFile: \"$strFilePath\" is not a file.");
				return false;			
			}
			return true;		
		}
	
	
		protected
		function GPG_Encrypt(
			$strFileNameCore,
			$strPayload,
			$strPublicKeyUserID,
			&$strEncryptedPayload)
		{

			$arrayResult = array();
			//$strPayload = escapeshellarg(chop($strPayload));
			//$strPayload = escapeshellarg($strPayload);
			$strPayload = ShellEscapeArgumentString($strPayload);
			$bResult = ShellExecute(
				"ech"."o $strPayload| ".$this->m_strEnginePath,
				array(
					"--batch",
					"--armor",
					"--always-trust",
					"--no-default-keyring",
					$this->m_strRandomSeedFileOption,
					"--keyring",
					$this->GPG_MakeFilePathPublicKeyRing($strFileNameCore),
					"--secret-keyring",
					$this->GPG_MakeFilePathPrivateKeyRing($strFileNameCore),
					"--trustdb",
					$this->GPG_MakeFilePathTrustDB($strFileNameCore),
					"--recipient",
					$strPublicKeyUserID,
					"--encrypt"
					),
				array("GNUPGHOME" => $this->m_strWorkingDir), 
				"",
				$arrayResult);

			$this->Trace("ShellExecute: Command:");
			$this->Trace($arrayResult["command"]);
			$this->Trace("ShellExecute: Result:");
			$this->Trace(RenderBool($bResult));
			$this->Trace("ShellExecute: stdout:");
			$this->Trace($arrayResult["stdout"]);
			$this->Trace("ShellExecute: stderr:");
			$this->Trace($arrayResult["stderr"]);

				
			if ($bResult != true)
			{
				$this->Trace("GPG_Encrypt: ShellExecute returned false.");
				return false;	
			}
			
			$strOutput = $arrayResult["stdout"];
			
			if ($this->GPG_IsValidMessage($strOutput) != true)
			{
				$this->Trace("GPG_Encrypt: Invalid message block.");
				return false;
			}
			
			$strEncryptedPayload = $strOutput;

			$this->Trace("GPG_Encrypt: Completed successfully");
			return true;
		}
		
		protected
		function GPG_Decrypt(
			$strFileNameCore,
			$strEncryptedPayload,
			$strPrivateKeyID,
			$strPassword,
			&$strPayload)
		{
			
			$this->GPG_MakeMessageFile(
				$strFileNameCore,
				$strEncryptedPayload,
				$strFilePath);
			
			$arrayResult = array();
			
			//$strPassword = escapeshellarg($strPassword);
			$strPassword = ShellEscapeArgumentString($strPassword);

			$bResult = ShellExecute(
				"ech"."o $strPassword|".$this->m_strEnginePath,
				array(
					"--batch",
					"--no-default-keyring",
					$this->m_strRandomSeedFileOption,
					"--quiet",
					"--passphrase-fd 0",
					"--keyring",
					$this->GPG_MakeFilePathPublicKeyRing($strFileNameCore),
					"--secret-keyring",
					$this->GPG_MakeFilePathPrivateKeyRing($strFileNameCore),
					"--trustdb",
					$this->GPG_MakeFilePathTrustDB($strFileNameCore),
					"--decrypt",
					$strFilePath
					),
				array("GNUPGHOME" => $this->m_strWorkingDir), 
				"",
				$arrayResult);

			$this->Trace("ShellExecute: Command:");
			$this->Trace($arrayResult["command"]);
			$this->Trace("ShellExecute: Result:");
			$this->Trace(RenderBool($bResult));
			$this->Trace("ShellExecute: stdout:");
			$this->Trace($arrayResult["stdout"]);
			$this->Trace("ShellExecute: stderr:");
			$this->Trace($arrayResult["stderr"]);

			DeleteFile($strFilePath,true);
				
			if ($bResult != true)
			{
				$this->Trace("GPG_Decrypt: ShellExecute returned false.");
				return false;	
			}
			
			//$strOutput = chop($arrayResult["stdout"]);
			$strOutput = chop($arrayResult["stdout"],"\n\r\0");
			//$strOutput = chop($arrayResult["stdout"],"\0");
			
			if ($strOutput == "")
			{
				$this->Trace("GPG_Decrypt: Decrypted message is empty.");
				return false;
			}
			
			$strPayload = $strOutput;

			$this->Trace("GPG_Decrypt: Completed successfully");
			return true;
		}		
		
		protected
		function GPG_CheckKeyID($strKeyID)
		{
			if (StringLength($strKeyID) != 8)
			{
				return false;
			}
			if (StringCheckCharSet($strKeyID,CHARSET_HEX) != true)
			{
				return false;
			}
			return true;
		}
		
		protected
		function GPG_IsValidPublicKey($strPublicKey)
		{
			return true;	
		}

		protected
		function GPG_IsValidPrivateKey($strPrivateKey)
		{
			return true;	
		}
		
		protected
		function GPG_IsValidMessage($strMessage)
		{
			if ($strMessage == "")
			{
				return false;
			}
			
			$nPosStart = FindString($strMessage,"-----BEGIN PGP MESSAGE-----");
			if ($nPosStart == -1)
			{
				return false;	
			}

			$nPosEnd = FindString($strMessage,"-----END PGP MESSAGE-----");
			if ($nPosEnd == -1)
			{
				return false;	
			}
			
			if ($nPosStart > $nPosEnd)
			{
				return false;
			}

			return true;	
		}		
	}
	
