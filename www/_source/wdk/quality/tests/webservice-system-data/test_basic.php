<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	
	class CTest extends CUnitTest
	{
		
		private $m_strWebservice;
		private $m_strItemID;
		
		function __construct()
		{
			$arrayConfig = array();;
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			$this->m_strWebservice = "system/data";
			parent::__construct("Web service system/data basic functionality",$arrayConfig);
		}
		
		function OnInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(false);
			return parent::OnInit();
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);
			
			
	
			
			/*
			
			0. Cleanup
			
			1. Get Data by name
			2. Set data by name
			3. Get Data by name
			4. Set data by name
			5. Get Data by name
			6. Delete Data by name
			7. Get Data by name
		
			8. Get Data by name
			9. Set data by name
			10. Get Data by id
			11. Set data by id
			12. Get Data by id
			13. Delete Data by id
			14. Get Data by id
			
			
			*/
			

			
			$strDataName = "test";
			$strDataContent = "This is a test!";
			$strDataContent2 = "The Data has changed!";
		
		
			$this->Trace("0. DELETE DATA BY NAME (CLEANUP)");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");
		
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = "idtest";
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");
		
		
			
			$this->Trace("1. GET DATA BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$arrayResult = $consumer->GetResultArray();
			$strData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strData."\"");
		
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Data item \"".$strDataName."\" should not exist.");		
				return;	
			}
			$this->Trace("");
		
			
			$this->Trace("2. SET DATA BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["data_content"] = $strDataContent; 
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
			
			
			$this->Trace("3. GET DATA BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace($consumer->GetServiceOutput());		
			$strData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			
			$this->Trace("Data: \"".$strData."\"");
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			if ($strData != $strDataContent)
			{
				return;	
			}
		
		
		
			$this->Trace("4. SET DATA BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["data_content"] = $strDataContent2; 
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			
			$this->Trace("5. GET DATA BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strData."\"");
			if ($strData != $strDataContent2)
			{
				$this->Trace("Setting the data has failed");
				return;	
			}
			$this->Trace("");
		
		
		
			$this->Trace("6. DELETE DATA BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");
				return;	
			}
			$this->Trace("");
		
			
			$this->Trace("7. GET DATA BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["command"] = "get";

			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strData."\"");
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Data \"$strDataName\" should not exist!");
				return;	
			}
			$this->Trace("");
			
			
			
			
			
			$strDataName = "idtest";
			$strDataContent = "This is a test with ID!";
			$strDataContent2 = "The Data has changed (still a test with ID)!";
		
			
			$this->Trace("8. GET DATA BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Data with name \"idtest\" should not exist!");
				return;	
			}
			$this->Trace("");
			
			
			
		
		
			
			$this->Trace("9. SET DATA BY NAME");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_name"] = $strDataName;
			$arrayParams["data_content"] = $strDataContent; 
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");
				return;	
			}
		
			$strData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strData."\"");
		
			$strDataID = $consumer->GetResultValue("NEW_DATA_ID");
			$this->Trace("Data ID: \"".$strDataID."\"");
			$this->Trace("");
		
			
			
			$this->Trace("10. GET DATA BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_id"] = $strDataID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);		
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
			$strData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strData."\"");
			$this->Trace("");
		
			$this->Trace("11. SET DATA BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_id"] = $strDataID;
			$arrayParams["data_content"] = $strDataContent2; 
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
			
			$this->Trace("12. GET DATA BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_id"] = $strDataID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);		
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strData."\"");
			$this->Trace("");
		
		
			$this->Trace("13. DELETE DATA BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_id"] = $strDataID;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
			
			$this->Trace("14. GET DATA BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["data_id"] = $strDataID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Error: Data with id \"".$strDataID."\" should not exist anymore.");		
				return;	
			}
			$strData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strData."\"");
			
			
					
			$this->SetResult(true);
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
					
			return true;
		}
		
		
	}
	
	

		
