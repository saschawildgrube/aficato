<?php

	require_once("wdk_website_dynamic.inc");
	require_once("wdk_securedatacontainer.inc");
	
	

   abstract class CEncryptionDynamicWebSite extends CDynamicWebSite
	{
		private $m_sdc;
	
		function __construct(
			$strRootURL,
			$strSourceDir,
			$strWWWDir,
			$arrayMeta,
			$arrayConfig,
			$arrayParams,
			$strSessionOwnerID,
			$arrayObfuscationItems = array()
			)
		{
			$this->m_sdc = new CSecureDataContainer();
			
			parent::__construct(
				$strRootURL,
				$strSourceDir,
				$strWWWDir,
				$arrayMeta,
				$arrayConfig,
				$arrayParams,
				$strSessionOwnerID,
				$arrayObfuscationItems);
		}
		
	
		function OnInit()
		{
			if (IsMcryptSupported() == false)
			{
				$this->Error("mcrypt is not supported!");	
			}
			$this->LoadSecretPayload();
			return parent::OnInit();
		}
		

		
		function LoadSecretPayload()
		{
			$arraySlices = array();
			$arraySlices[0] = $this->GetVar("secretpayload");
			$arraySlices[1] = $this->GetVar("secretpayload_session");
			
			if ($arraySlices[0] == "")
			{
				$this->Trace("LoadSecretPayload: secretpayload is empty");
				return false;	
			}
			if ($arraySlices[1] == "")
			{
				$this->Trace("LoadSecretPayload: secretpayload_session is empty");				
				return false;
			}
			
			$strGlued = StringGlue($arraySlices);
			if ($strGlued == "")
			{
				return false;	
			}
			
			$bResult = $this->m_sdc->UnserializeDecrypt($strGlued,"1");
			if (!$bResult)
			{
				$this->Trace("LoadSecretPayload: Failed to decrypt payload");
				return false;	
			}

			$strSecretPayload = $this->m_sdc->GetValue("payload");
			return $strSecretPayload;
		}


		function StoreSecretPayload($strSecretPayload)
		{
			$this->Trace("StoreSecretPayload");
			$this->Trace("StoreSecretPayload: \"$strSecretPayload\"");
			if ($strSecretPayload == "")
			{
				$this->SetVar("secretpayload","");
				$this->SetVar("secretpayload_session","");
				return;	
			}
			$this->m_sdc->SetValue($strSecretPayload,"payload");
			
			$strSerialized = $this->m_sdc->SerializeEncrypt("1"); 
			
			$arraySlices = StringSlice($strSerialized,2);
			
			$this->SetVar("secretpayload",$arraySlices[0]);
			$this->SetVar("secretpayload_session",$arraySlices[1]);
		}

		function GetSessionAwareArrayVars()
		{
			$arrayVarsSession = parent::GetSessionAwareArrayVars();
			$strSecretPayload = $this->GetVar("secretpayload");
			if ($strSecretPayload != "")
			{
				$arrayVarsSession["secretpayload"] = $strSecretPayload;
			}
	   	return $arrayVarsSession;
		}

		function OnSessionWrite($strSessionID, $arrayData)
		{
			$this->Trace("OnSessionWrite");
			$arrayData = ArrayRemoveKey($arrayData,"secretpayload");
			return parent::OnSessionWrite($strSessionID, $arrayData);
		}
	
		function OnUserLogin($strUserName,$strAuthenticationPayload)	
		{
			$strUserID = parent::OnUserLogin($strUserName,$strAuthenticationPayload);
			if ($strUserID != "")
			{
				$this->StoreSecretPayload($strAuthenticationPayload);
			}
			return $strUserID;
		}
		
		function OnUserLogout()
		{
			$this->StoreSecretPayload("");
			return parent::OnUserLogout();
		}			
		
		function OnUserChangePassword($strOldPassword,$strNewPassword)
		{
			$bResult = parent::OnUserChangePassword($strOldPassword,$strNewPassword);
			if ($bResult == false)
			{
				return false;
			}			
			$this->StoreSecretPayload($strNewPassword);			
			return true;		
		}
	}

