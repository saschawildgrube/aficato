<?php
	
	require_once(GetSourceDir()."webservices_directory.inc");

	/*
	
	1. Check "passed" test
	2. Check "failed" test
	3. Check "inactive" test
	
	*/
	
	class CTest extends CUnitTest
	{
		private $m_strWebservice;
		
		function __construct()
		{
			$arrayConfig = array();
			$arrayConfig["webservices"] = GetWebServicesDirectory();
			parent::__construct("Web service system/test",$arrayConfig);
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			
			$this->m_strWebservice = "system/test";
			$this->RequireWebservice($this->m_strWebservice);
			$this->SetVerbose(true);
			
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			
			
			$consumer = new CWebServiceConsumerWebApplication($this);


		
			$this->Trace("1. TEST PASSED TEST:");
			$arrayParams = array();
			$arrayParams["test_path"] = "wdk/test/passed";
			$arrayParams["command"] = "test";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strTestResult = $consumer->GetResultValue("LASTEXECUTION_STATUS");
			$this->Trace("LASTEXECUTION_STATUS = ".$strTestResult);
			if ($strTestResult != "PASSED")	
			{
				$this->Trace("Error: Unexpected test result!");		
				return;	
			}
			$this->Trace("");
		
			$this->Trace("2. TEST FAILED TEST:");
			$arrayParams = array();
			$arrayParams["test_path"] = "wdk/test/failed";
			$arrayParams["command"] = "test";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strTestResult = $consumer->GetResultValue("LASTEXECUTION_STATUS");
			$this->Trace("LASTEXECUTION_STATUS = ".$strTestResult);
			if ($strTestResult != "FAILED")	
			{
				$this->Trace("Error: Unexpected test result!");		
				return;	
			}
			$this->Trace("");
			
			$this->Trace("3. TEST INACTIVE TEST:");
			$arrayParams = array();
			$arrayParams["test_path"] = "wdk/test/inactive";
			$arrayParams["command"] = "test";
			$consumer->ConsumeWebService($this->m_strWebservice,$arrayParams);
			if ($consumer->GetError() != "")	
			{
				$this->Trace("Error: \"".$consumer->GetError()."\"");		
				return;	
			}
			$strTestResult = $consumer->GetResultValue("LASTEXECUTION_STATUS");
			$this->Trace("LASTEXECUTION_STATUS = ".$strTestResult);
			if ($strTestResult != "INACTIVE")	
			{
				$this->Trace("Error: Unexpected test result!");		
				return;	
			}
			$this->Trace("");

			$this->SetResult(true);
		}
		

		
	}
	
	

		
