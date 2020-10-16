<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	0. deletecontext
	
	1. Get data1 by name and context
	2. Set data1 by name and context
	3. Get data1 by name and context

	4. Set data1 by id
	5. Get data1 by id
		
	6. Set data2 by name and context
	7. Get data2 by name and context

	8. Set data3 by name and context
	9. Get data3 by name and context
	10. Delete data3 by name and context
	11. Get data3 by name and context

	12. Set data3 by name and context
	13. Get data3 by id
	14. Delete data3 by id
	15. Get data3 by id

	16. List data by context
	17. deletecontext
	18. List data by context
	
	*/
	
	class CTest extends CUnitTest
	{
		
		private $m_strWebservice;
		private $m_strItemID;
		
		function __construct()
		{
			$this->m_strWebservice = "system/data";
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/data Context",$arrayConfig);
		}
		
		function CallbackInit()
		{
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(false);
			return parent::CallbackInit();;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);


	
			
			$strDataContent1 = "This is a test!";
			$strDataContent2 = "This is another test!";
			$strDataContent3 = "Yet another test!";
			
			$strDataContextType = "TEST";
			$strDataContextID = "TEST1";
		
			$strDataName1 = "test1";
			$strDataName2 = "test2";
			$strDataName3 = "test3";
		
		
			$this->Trace("0. DELETE DATA BY CONTEXT (CLEANUP)");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "deletecontext";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$this->Trace("");
		
			
			$this->Trace("1. GET DATA1 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName1;
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$arrayResult = $consumer->GetResultArray();
			$strResultData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strResultData."\"");
		
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Data item \"$strDataContextType/$strDataContextID/$strDataName1\" should not exist.");
				return;	
			}
			$this->Trace("");
		
			
			$this->Trace("2. SET DATA1 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName1;
			$arrayParams["data_content"] = $strDataContent1; 
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
			
			
			$this->Trace("3. GET DATA1 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName1;
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strDataID1 = $consumer->GetResultValue("DATA","DATA_ID");
			$strResultData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$strResultDataContextType = $consumer->GetResultValue("DATA","DATA_CONTEXT_TYPE");
			$strResultDataContextID = $consumer->GetResultValue("DATA","DATA_CONTEXT_ID");
			
			$this->Trace("Data: \"".$strResultData."\"");
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			if ($strResultData != $strDataContent1)
			{
				$this->Trace("Unexpected Data!");
				return;	
			}
		
		
		
		
			$this->Trace("4. SET DATA1 BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_id"] = $strDataID1;
			$arrayParams["data_content"] = $strDataContent1.$strDataContent1; 
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
		
			$this->Trace("5. GET DATA1 BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_id"] = $strDataID1;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strResultData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strResultData."\"");
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			if ($strResultData != $strDataContent1.$strDataContent1)
			{
				$this->Trace("Unexpected Data!");
				return;	
			}
		
		
		
		
			$this->Trace("6. SET DATA2 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName2;
			$arrayParams["data_content"] = $strDataContent2; 
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("7. GET DATA2 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName2;
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strResultData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$this->Trace("Data: \"".$strResultData."\"");
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			if ($strResultData != $strDataContent2)
			{
				$this->Trace("Unexpected Data!");
				return;	
			}
		
		
			$this->Trace("8. SET DATA3 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName3;
			$arrayParams["data_content"] = $strDataContent3; 
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("9. GET DATA3 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName3;
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strResultData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			$strDataID3 = $consumer->GetResultValue("DATA","DATA_ID");
			$this->Trace("ID: \"".$strDataID3."\"");
			$this->Trace("Data: \"".$strResultData."\"");
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			if ($strResultData != $strDataContent3)
			{
				$this->Trace("Unexpected Data!");
				return;	
			}
		
		
		
			$this->Trace("10. DELETE DATA3 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName3;
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$this->Trace("11. GET DATA3 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName3;
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Data \"$strDataName3\" should not exist!");
				return;	
			}
			$this->Trace("");
			

		
			$this->Trace("12. SET DATA3 BY NAME AND CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_name"] = $strDataName3;
			$arrayParams["data_content"] = $strDataContent3; 
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "set";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strDataID3 = $consumer->GetResultValue("NEW_DATA_ID");
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
		
		
			$this->Trace("13. GET DATA3 BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_id"] = $strDataID3;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			$strResultData = $consumer->GetResultValue("DATA","DATA_CONTENT");
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			if ($strResultData != $strDataContent3)
			{
				$this->Trace("Unexpected Data!");
				return;	
			}
		
		
		
			$this->Trace("14. DELETE DATA3 BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_id"] = $strDataID3;
			$arrayParams["command"] = "delete";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
		
		
			$this->Trace("15. GET DATA3 BY ID");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_id"] = $strDataID3;
			$arrayParams["command"] = "get";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() == "")	
			{
				$this->Trace("Data ID=\"$strDataID3\" should not exist!");
				return;	
			}
			$this->Trace("");
			
			
		
			$this->Trace("16. LIST BY CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "1";
			$arrayParams["filter1"] = "data_context_type";
			$arrayParams["filter1_value"] = $strDataContextType;
			$arrayParams["filter2"] = "data_context_id";
			$arrayParams["filter2_value"] = $strDataContextID;
			$arrayParams["blocksize"] = "100";
			$arrayParams["offset"] = "0";
			$arrayParams["command"] = "list";
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
				$this->Trace("Unexpected result");
				return;
			}
			$this->Trace("");
		
		
			$this->Trace("17. DELETE DATA BY CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["data_context_type"] = $strDataContextType;
			$arrayParams["data_context_id"] = $strDataContextID;
			$arrayParams["command"] = "deletecontext";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$this->Trace("");
			
		
			$this->Trace("18. LIST BY CONTEXT");
			$arrayParams = array();
			$arrayParams["trace"] = "0";
			$arrayParams["filter1"] = "data_context_type";
			$arrayParams["filter1_value"] = $strDataContextType;
			$arrayParams["filter2"] = "data_context_id";
			$arrayParams["filter2_value"] = $strDataContextID;
			$arrayParams["blocksize"] = "100";
			$arrayParams["offset"] = "0";
			$arrayParams["command"] = "list";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$arrayList = $consumer->GetResultList();
			$this->Trace($arrayList);
			if (ArrayCount($arrayList) != 0)
			{
				$this->Trace("There are still data items.");
				return;	
			}
			
			$this->Trace("");
		
		
		
		
		
					
			$this->SetResult(true);	
			


			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			
			
			return true;
		}
		
		
	}
	
	

		
