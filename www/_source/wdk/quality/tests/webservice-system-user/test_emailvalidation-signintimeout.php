<?php

	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	Test the timeout function of an initial user validation process
	
	1. Delete test user(s)
	2. Add test user with email validation (and allow 1 second for validation)
	3. Get test user (and check for validation status and user email)
	4. Wait a second
	5. Cleanup
	6. Get test user (and check for validation status and user email)	-> should fail
	7. Validate email (by applying the token) -> should fail
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
				"Web service system/user Email Validation Sign-In Timeout",
				$arrayConfig);
		}
		
		function OnInit()
		{
			$this->RequireWebservice($this->m_strWebservice);			 
			return parent::OnInit();
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			$consumer = new CWebServiceConsumerWebApplication($this);

			$strUserName = "test-signin-timeout";
			$strEmail = "test@websitedevkit.com";
			$strPassword = "secret";
		
			$this->Trace("1. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");


			$this->Trace("2. ADD TEST USER WITH EMAIL VALIDATION:");
			$arrayParams = array();
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["password"] = $strPassword;
			$arrayParams["user_email"] = $strEmail;
			$arrayParams["user_email_validation"] = 1; // activate email validation
			$arrayParams["user_email_validation_duration"] = 1; // one second to validate the email
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
			$this->Trace("VALIDATION_TOKEN = \"".$this->m_strValidationToken."\"");
			if ($this->m_strValidationToken == "")	
			{
				$this->Trace("Error: Validation token should not be empty!");		
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
			$strUserEmailCheck = $consumer->GetResultValue("USER","USER_EMAIL");
			if ($strUserEmailCheck != "")	
			{
				$this->Trace("Error: User email is \"$strUserEmailCheck\", but it must be empty.");		
				return;	
			}
			$strValidationTokenCheck = $consumer->GetResultValue("USER","USER_EMAIL_VALIDATION_TOKEN");
			if ($strValidationTokenCheck != $this->m_strValidationToken)	
			{
				$this->Trace("Error: Validation Token  \"$strValidationTokenCheck\" != \"$this->m_strValidationToken\"");		
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
		
			$this->Trace("4. WAIT A SECOND");
			sleep(2);
			$this->Trace("");
		
		
			$this->Trace("5. CLEANUP:");
			$arrayParams = array();
			$arrayParams["command"] = "cleanup";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}				
			$this->Trace("");





			$this->Trace("6. GET USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "ITEM_NOT_FOUND")	
			{
				$this->Trace("Error: Result is \"".$consumer->GetError()."\" but it should be ITEM_NOT_FOUND");		
				return;	
			}
			$this->Trace("");		


			$this->Trace("7. VALIDATE EMAIL:");
			$arrayParams = array();
			$arrayParams["user_email_validation_token"] = $this->m_strValidationToken;
			$arrayParams["command"] = "validateemail";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "TOKEN_NOT_FOUND")	
			{
				$this->Trace("Error: Result is \"".$consumer->GetError()."\" but it should be TOKEN_NOT_FOUND");		
				return;	
			}
			$this->Trace("");			


		
			$this->SetResult(true);
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			if ($this->m_strUserID != "")
			{
				$this->Trace("DELETE USER:");
				$arrayParams = array();
				$arrayParams["user_id"] = $this->m_strUserID;
				$arrayParams["command"] = "delete";
				$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
				/*if ($consumer->GetError() != "")	
				{
					$this->Trace("Error: \"".$consumer->GetError()."\"");		
					return false;	
				}	
				*/	
			}
	
			
			return true;
		}
		
		
	}
	
	

		
