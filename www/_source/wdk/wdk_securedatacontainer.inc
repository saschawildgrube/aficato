<?php

	require_once("wdk_datacontainer.inc");
	require_once("wdk_encryption-symmetric.inc");

	class CSecureDataContainer extends CDataContainer
	{
		private $m_arrayHeader;
		
		function __construct()
		{
			$this->m_arrayHeader = array();
			parent::__construct();
		}
		
		function SetHashMethod($strHashMethod)
		{
			ArraySetValue($this->m_arrayHeader,$strHashMethod,"hashmethod");
		}
		function GetHashMethod()
		{
			return ArrayGetValue($this->m_arrayHeader,"hashmethod");
		}		
		
		function SetAlgorithm($strAlgorithm)
		{
			ArraySetValue($this->m_arrayHeader,$strAlgorithm,"algorithm");
		}
		function GetAlgorithm()
		{
			return ArrayGetValue($this->m_arrayHeader,"algorithm");
		}
		
		function SetInitVector($iv)
		{
			ArraySetValue($this->m_arrayHeader,base64_encode($iv),"initvector");
		}
		function GetInitVector()
		{
			return base64_decode(ArrayGetValue($this->m_arrayHeader,"initvector"));
		}
		
		function GetHeaderArray()
		{
			return $this->m_arrayHeader;	
		}
		

		
		function SerializeEncrypt($strPassword)
		{
			if (IsMcryptSupported() == false)
			{
				return false;
			}			

			if ($this->GetHashMethod() == "")
			{
				$this->SetHashMethod("bcrypt7");
			}
			if ($this->GetAlgorithm() == "")
			{
				$this->SetAlgorithm("rijndael-256");
			}
			if ($this->GetInitVector() == "")
			{
				$iv = MakeInitVector($this->GetAlgorithm());
				if ($iv == false)
				{
					return false;
				}
				$this->SetInitVector($iv);
			}

			$strSerialized = $this->Serialize();
			$strEncrypted = Encrypt(
				$strSerialized,
				$strPassword,
				$this->GetInitVector(),
				$this->GetAlgorithm(),
				$this->GetHashMethod());
			
			ArraySetValue($this->m_arrayHeader,base64_encode($strEncrypted),"payload");

			return base64_encode(serialize($this->m_arrayHeader));
		}

		
		function UnserializeDecrypt($strSerialized,$strPassword)
		{
			if (IsMcryptSupported() == false)
			{
				return false;
			}
						
			if ($strSerialized == "")
			{
				return false;	
			}
			$this->m_arrayData = array();
			$ea = new CErrorAmnesty();
			$ea->Start();
			$this->m_arrayHeader = unserialize(base64_decode($strSerialized));
			$ea->End();
			if (!is_array($this->m_arrayHeader))
			{
				return false;
			}
			if ($this->GetHashMethod() == "")
			{
				$this->SetHashMethod("sha1"); // For legacy support
			}
			if ($this->GetAlgorithm() == "")
			{
				return false;
			}
			if ($this->GetInitVector() == "")
			{
				return false;
			}

			$this->m_arrayData = array();
			$strEncrypted = base64_decode(ArrayGetValue($this->m_arrayHeader,"payload"));
			if ($strEncrypted == "")
			{
				return false;
			}

			$strDecrypted = Decrypt(
				$strEncrypted,
				$strPassword,
				$this->GetInitVector(),
				$this->GetAlgorithm(),
				$this->GetHashMethod());
				
			if ($strDecrypted == false)
			{
				return false;	
			}
	
			$bResult = $this->Unserialize($strDecrypted);
			if ($bResult != true)
			{
				return false;	
			}
			return true;
		}


		
	}



