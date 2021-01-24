<?php

	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	
	*/
	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		
		function __construct()
		{
			$this->m_strWebservice = "system/test";
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/test",$arrayConfig);
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


		
		 
			$this->Trace("GET TEST LIST:");
			$arrayParams = array();
			$arrayParams["test_filter1"] = "site_path";
			$arrayParams["test_filter1_value"] = "local";
			$arrayParams["test_filter1_operator"] = "=";
			$arrayParams["test_filter2"] = "assembly_id";
			$arrayParams["test_filter2_value"] = "wdk";
			$arrayParams["test_filter2_operator"] = "=";
			$arrayParams["test_filter3"] = "group_id";
			$arrayParams["test_filter3_value"] = "test";
			$arrayParams["test_filter3_operator"] = "=";
			$arrayParams["command"] = "testlist";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			
			$arrayTestList = $consumer->GetResultList();
			
			if (ArrayCount($arrayTestList) != 3)
			{
				$this->Trace("There should be three tests in the \"test\" group.");
				return;
			}
			
			foreach ($arrayTestList as $arrayTest)
			{
				$strGroupID = $arrayTest["GROUP_ID"];
				if ($strGroupID != "test")
				{
					$this->Trace("Unexpected test group: \"$strGroupID\"");
					return;	
				}
				$strTestID = $arrayTest["TEST_ID"];
				
					
			}
			
			$this->Trace($arrayTestList);
			
			$this->Trace("");
		

			$this->SetResult(true);
		}
		

		
	}
	
	

		
