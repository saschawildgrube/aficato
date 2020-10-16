<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	require_once(GetWDKDir()."wdk_random.inc");


	/*
	
	0. Cleanup
	1. Create new user
	2. Set data
	3. Get data
	
	4. Delete data
	5. Get data
	
	6. Set data1 and data2
	7. List Data
	8. Delete User (and internally delete all data)
	9. Check if data is left

	*/

	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		private $m_strWebserviceData;
		
		private $m_strUserName;
		private $m_strUserID;
		
		function __construct()
		{
			$this->m_strWebservice = "system/user";
			$this->m_strWebserviceData = "system/data";
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/user Data Features",$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->RequireWebservice($this->m_strWebserviceData);
			$this->m_strUserName = "test-data";
			$this->SetVerbose(true);
			return parent::CallbackInit();
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);


		
		
			
			
			$strDataName1 = "data1";
			$strDataContent1 = "This is a test";
			$strDataName2 = "data2";
			$strDataContent2 = "This is another test";
		
			$this->Trace("0. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");
		
		
			$this->Trace("1. ADD USER:");
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
			$this->m_strUserID = $consumer->GetResultValue("NEW_USER_ID");
			$this->Trace("New USERID = ".$this->m_strUserID);
			if ($this->m_strUserID == "")	
			{
				$this->Trace("Error: New user ID is empty!");		
				return;	
			}
		
			
			$this->Trace("2. SET USER DATA:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["data_name"] = $strDataName1;
			$arrayParams["data_content"] = $strDataContent1;
			$arrayParams["command"] = "setdata";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$this->Trace("3. GET USER DATA:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
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
		
		
		
			$this->Trace("4. DELETE USER DATA:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["data_name"] = $strDataName1;
			$arrayParams["command"] = "deletedata";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("5. GET USER DATA:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["data_name"] = $strDataName1;
			$arrayParams["command"] = "getdata";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strData."\"");
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Data \"$strDataName\" should not exist!");
				return;	
			}
			$this->Trace("");

		
			$this->Trace("6. SET USER DATA1 and DATA2 (by username):");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["data_name"] = $strDataName1;
			$arrayParams["data_content"] = $strDataContent1;
			$arrayParams["command"] = "setdata";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayParams = array();
			$arrayParams["user_name"] = $this->m_strUserName;
			$arrayParams["data_name"] = $strDataName2;
			$arrayParams["data_content"] = $strDataContent2;
			$arrayParams["command"] = "setdata";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("7. LIST USER DATA:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "listdata";
			$arrayParams["blocksize"] = 100;
			$arrayParams["offset"] = 0;
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayList = $consumer->GetResultList();
			$this->Trace($arrayList);
			if (ArrayCount($arrayList) != 2)	
			{
				$this->Trace("Data is missing!");		
				return;	
			}
			$this->Trace("");
		
			
		
			$this->Trace("8. DELETE USER:");
			$arrayParams = array();
			$arrayParams["user_id"] = $this->m_strUserID;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
		
			$this->Trace("9. CHECK FOR DATA REMAINDERS:");
			$arrayParams = array();
			//$arrayParams["trace"] = "1";
			//$arrayParams["contexttype"] = "USER";
			//$arrayParams["contextid"] = $strUserID;
			$arrayParams["filter1"] = "data_context_type";
			$arrayParams["filter1_value"] = "USER";
			$arrayParams["filter2"] = "data_context_id";
			$arrayParams["filter2_value"] = $this->m_strUserID;
			$arrayParams["blocksize"] = "100";
			$arrayParams["offset"] = "0";
			$arrayParams["command"] = "list";
			$consumer->ConsumeWebService($this->m_strWebserviceData,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
			$arrayList = $consumer->GetResultList();
			$this->Trace($arrayList);
			if (ArrayCount($arrayList) != 0)	
			{
				$this->Trace("Error: there are still data elements associated to the deleted user!");		
				return;	
			}
			
					
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
/*			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return false;	
			}		
*/
			
			return true;
		}
		
		
	}
	
	

		
