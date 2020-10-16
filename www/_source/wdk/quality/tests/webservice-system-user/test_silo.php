<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");
	require_once(GetWDKDir()."wdk_hash.inc");

	/*
	
	1. Delete / Create new user
	2. Set by name
	3. Get by name
	4. Authenticate User by password
	5. Authenticate User by password hash
	6. Authenticate User to provoke an error
	7. Get user and check last authentication date/time
	8. Set data by name
	9. Get data by name
	10. Rename by name
	11. Get by id
	
	
	*/


	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		
		private $m_strUserID;
		private $m_nSiloID;
		
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/user",$arrayConfig);
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();

			$this->RequireWebservice("system/user");
			
			$this->m_strWebservice = "system/user";
			$this->m_nSiloID = "99999";
					
			$this->SetVerbose(true);
			//$this->SetActive(false);
			
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$consumer = new CWebServiceConsumerWebApplication($this);

			$strUserName = "test";
			$strUserName2 = "test-renamed";

			$strDataName1 = "data1";
			$strDataContent1 = "This is a test";
			$strDataName2 = "data2";
			$strDataContent2 = "This is another test";

		
			$this->Trace("1A. DELETE USER:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");
		
		
			$this->Trace("1B. ADD USER:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;			
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["password"] = "changeme";
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
		
			
			$this->Trace("2. SET USER:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["passwordhash_seed"] = MakePasswordHashSeed();
			$arrayParams["passwordhash"] = MakePasswordHash("1",$arrayParams["passwordhash_seed"]);
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$this->Trace("3. GET USER:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strPasswordHash = $consumer->GetResultValue("USER","PASSWORDHASH");
			$strPasswordHashSeed = $consumer->GetResultValue("USER","PASSWORDHASH_SEED");
		
			$bMatch = VerifyPassword("1",$strPasswordHash,$strPasswordHashSeed);
		
			$this->Trace((($bMatch)?("MATCH!"):("MATCH FAILED!")));
			$this->Trace("");
		
			$this->Trace("4. AUTHENTICATE USER BY PASSWORD:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "authenticate";
			$arrayParams["authenticationpayload"] = "1";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("5. AUTHENTICATE USER BY PASSWORDHASH:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "authenticate";
			$arrayParams["authenticationpayload"] = MakePasswordHash("1",$strPasswordHashSeed);
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams); 
			$this->Trace($consumer->GetServiceOutput()) . "</pre>";
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
					
			$this->Trace("6. AUTHENTICATE USER / PROVOKE AN ERROR:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "authenticate";
			$arrayParams["authenticationpayload"] = "bogus";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "USER_AUTHENTICATION_FAILED")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\" should be USER_AUTHENTICATION_FAILED");
				return;	
			}
			$this->Trace("");

			$this->Trace("7. GET USER and check last authentication");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			
			$strDateTimeLastAuthentication = $consumer->GetResultValue("USER","LASTAUTHENTICATION_DATETIME");
			$timeLastAuthentication = GetTime($strDateTimeLastAuthentication);
			$timeNow = GetTimeNow();
			
			$this->Trace("Last Authentication: $strDateTimeLastAuthentication ($timeLastAuthentication)");
			$this->Trace("Now                : ".RenderDateTime($timeNow)." ($timeNow)");
			
			if ( ($timeNow - $timeLastAuthentication) > 10)
			{
				$this->Trace("Error: Time difference is bigger than 10 seconds.");
				return;	
			}
			
			
			
			$this->Trace("8. SET USER DATA:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["data_name"] = $strDataName1;
			$arrayParams["data_content"] = $strDataContent1;
			$arrayParams["command"] = "setdata";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$this->Trace("9. GET USER DATA:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["data_name"] = $strDataName1;
			$arrayParams["command"] = "getdata";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);		
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strDataContentResult = $consumer->GetResultValue("DATA","DATA_CONTENT");
			if ($strDataContent1 != $strDataContentResult)
			{
				$this->Trace("Unexpected or missing data");
				return;	
			}
			$this->Trace("");
			
			
			
			
			$this->Trace("10. RENAME USER BY NAME:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName;
			$arrayParams["new_user_name"] = $strUserName2;
			$arrayParams["command"] = "rename";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("11. GET USER:");
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
			if ($strUserNameCheck != $strUserName2)	
			{
				$this->Trace("\"$strUserNameCheck\" != \"$strUserName2\"");		
				return;	
			}
			$this->Trace("");			





			$this->Trace("DELETE USER:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;
			$arrayParams["user_name"] = $strUserName2;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
			}		

			$this->Trace("GET USER:");
			$arrayParams = array();
			$arrayParams["silo_id"] = $this->m_nSiloID;			
			$arrayParams["user_name"] = $strUserName2;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Result unexpected. User sholuld no longer exist");		
				return;	
			}
			$this->Trace("");			



			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			

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
			
			return true;
		}
		
		
	}
	
	

		
