<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	1. Delete test user(s)
	2. Add test user with email validation
	3. Get test user (and check for validation status and user email)
	4. Attempt to authenticate the user (without prior validation)
	5. Validate user (by applying an incorrect token)
	6. Validate user (by applying the token)
	7. Get test user (and check for validation status and user email)
	8. Attempt to authenticate
	CLEANUP
	* Delete Test User

	*/

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_strUserID;
		
		function __construct()
		{
			$this->m_strWebservice = "system/user";			
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();			
			parent::__construct(
				"Web service system/user Email Validation Sign-In",
				$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);			 
			return parent::CallbackInit();;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$consumer = new CWebServiceConsumerWebApplication($this);

			$strUserName = "test-signin";
			$strEmail = "test@websitedevkit.com";
			$strPassword = "secret";
		
			$this->Trace("1. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");


		
			$this->Trace("2. ADD USER (WITH EMAIL TO BE VALIDATED):");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["password"] = $strPassword;
			$arrayParams["user_email"] = $strEmail;
			$arrayParams["user_email_validation"] = 1; // activate email validation
			$arrayParams["user_email_validation_duration"] = 60; // sixty seconds to validate the email
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->m_strUserID = $consumer->GetResultValue("NEW_USER_ID");
			$this->Trace("New USERID = ".$this->m_strUserID);
			if ($this->m_strUserID == "")	
			{
				$this->Trace("Error: New user ID is empty!");		
				return;	
			}
			$this->m_strValidationToken = $consumer->GetResultValue("VALIDATION_TOKEN");
			$this->Trace("VALIDATION_TOKEN = ".$this->m_strValidationToken);
			if ($this->m_strValidationToken == "")	
			{
				$this->Trace("Error: Validation token is empty!");		
				return;	
			}			
			$this->Trace("");
		
		
			$this->Trace("3. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName)	
			{
				$this->Trace("Error: \"$strUserNameCheck\" != \"$strUserName\"");		
				return;	
			}
			$this->Trace("");		
			$strUserEmailCheck = $consumer->GetResultValue("USER","USER_EMAIL");
			if ($strUserEmailCheck != "")	
			{
				$this->Trace("Error: User email is \"$strUserEmailCheck\", but it must be empty - since not yet validated");		
				return;	
			}
			$this->Trace("");		
			$strValidationTokenCheck = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");
			if ($strValidationTokenCheck != $this->m_strValidationToken)	
			{
				$this->Trace("Error: Validation Token  \"$strValidationTokenCheck\" != \"".$this->m_strValidationToken."\"");		
				return;	
			}
			$strValidationStatus = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_STATUS");
			$this->Trace("USER_EMAIL_VALIDATION_STATUS = \"$strValidationStatus\"");
			if ($strValidationStatus != "VALIDATION_SIGNIN")	
			{
				$this->Trace("Error: Validation Status is not VALIDATION_SIGNIN");
				return;	
			}			
			$this->Trace("");		
		
		
			$this->Trace("4. AUTHENTICATE VIA PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["authenticationpayload"] = $strPassword;
			$arrayParams["command"] = "authenticate";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USER_EMAIL_NOT_VALIDATED")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\" - expected is USER_EMAIL_NOT_VALIDATED");		
				return;	
			}
			$this->Trace("");			

		
		
			$this->Trace("5. VALIDATE USER (VIA INVALID TOKEN):");
			$arrayParams = array();
			$arrayParams["user_email_validation_token"] = "1234567890";
			$arrayParams["command"] = "validateemail";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "TOKEN_NOT_FOUND")	
			{
				$this->Trace("Error: Expected error = \"TOKEN_NOT_FOUND\" but the service returned error = \"".$consumer->GetError()."\"");
				return;	
			}
			$this->Trace("");			
		
		
			$this->Trace("6. VALIDATE USER (VIA CORRECT TOKEN):");
			$arrayParams = array();
			$arrayParams["user_email_validation_token"] = $this->m_strValidationToken;
			$arrayParams["command"] = "validateemail";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName)	
			{
				$this->Trace("Error: \"$strUserNameCheck\" != \"$strUserName\"");		
				return;	
			}			
			$this->Trace("");			
		
		
			$this->Trace("7. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserNameCheck = $consumer->GetResultValue("USER","USER_NAME");
			if ($strUserNameCheck != $strUserName)	
			{
				$this->Trace("Error: \"$strUserNameCheck\" != \"$strUserName\"");		
				return;	
			}
			$this->Trace("");		
			$strUserEmailCheck = $consumer->GetResultValue("USER","USER_EMAIL");
			if ($strUserEmailCheck != $strEmail)	
			{
				$this->Trace("Error: User email is \"$strUserEmailCheck\", but it must be \"".$strEmail."\"");		
				return;	
			}
			$this->Trace("");		
			$strValidationTokenCheck = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");
			if ($strValidationTokenCheck != "")	
			{
				$this->Trace("Error: Validation token should be empty by now");		
				return;	
			}
			$strValidationStatus = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_STATUS");
			$this->Trace("USER_EMAIL_VALIDATION_STATUS = \"$strValidationStatus\"");
			if ($strValidationStatus != "VALIDATED")	
			{
				$this->Trace("Error: Validation Status is not VALIDATED");
				return;	
			}			
			$this->Trace("");		
		
		
		
			$this->Trace("8. AUTHENTICATE VIA PASSWORD:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["authenticationpayload"] = $strPassword;
			$arrayParams["command"] = "authenticate";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");			
		
		
		
			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			if ($this->m_strUserID != "")
			$this->Trace("DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}		
	
			
			return true;
		}
		
		
	}
	
	

		
