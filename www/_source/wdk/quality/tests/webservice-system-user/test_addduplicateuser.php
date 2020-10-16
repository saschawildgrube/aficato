<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	1. Delete test user
	2. Add test user
	3. Add test user 2nd try
	4. Delete test user
	
	*/

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		
		private $m_strUserName;
		private $m_strUserID;
		
		function __construct()
		{
			$this->m_strWebservice = "system/user";			
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/user Add duplicate user",$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(false);
			$this->m_strUserName = "test-addduplicateuser";
			return parent::CallbackInit();
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$consumer = new CWebServiceConsumerWebApplication($this);


		
			$this->Trace("1. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			/*if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			*/
		
		
		
			$this->Trace("2. ADD USER 1ST:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["password"] = "changeme";
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strUserID = $consumer->GetResultValue("NEW_USER_ID");
			$this->Trace("New USERID = ".$strUserID);
			if ($strUserID == "")	
			{
				$this->Trace("Error: New user ID is empty!");		
				return;	
			}
		
		
			$this->Trace("3. ADD USER 2ND:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["password"] = "changeme";
			$arrayParams["command"] = "add";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USER_EXISTS")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}

			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			$this->Trace("4. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}		
			
			return true;
		}
		
		
	}
	
	

		
