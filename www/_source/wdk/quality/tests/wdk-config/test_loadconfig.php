<?php

	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CConfig::LoadConfig");
		}
		

		function TestCase_LoadConfig(
			$arrayConfigStoreLocations,
			$arrayConfigIDs,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_LoadConfig");
	
			$this->Trace("arrayConfigIDs");
			$this->Trace($arrayConfigIDs);
			$this->Trace("arrayExpectedResult");
			$this->Trace($arrayExpectedResult);

	
			$config = new CConfig();
			
			foreach ($arrayConfigStoreLocations as $strConfigStoreLocation)
			{
				$config->AddConfigStoreLocation($strConfigStoreLocation);	
			}
			
			foreach ($arrayConfigIDs as $strConfigID)
			{
				$bResult = $config->LoadConfig($strConfigID);	
				if ($bResult == false)
				{
					$this->Trace("config->LoadConfig(\"$strConfigID\") returned false");	
				}
			}
	
			$arrayResult = $config->GetDataArray();
			
			$this->Trace("config->GetDataArray():");
			$this->Trace($arrayResult);
	
			if ($arrayResult == $arrayExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}
			$this->Trace("");
			$this->Trace("");
		}


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);

			$arrayConfigStoreLocations = array(GetWDKDir() . "quality/testfiles/");
			$arrayConfigIDs = array("test1");
			$arrayExpected = array(
				"tag1" => "value11",
				"tag2" => "value12",
				"tag3" => "value13");
			$this->TestCase_LoadConfig(
				$arrayConfigStoreLocations,
				$arrayConfigIDs,
				$arrayExpected);




			$arrayConfigStoreLocations = array(GetWDKDir() . "quality/testfiles/");
			$arrayConfigIDs = array("test1","test2","test3");
			$arrayExpected = array(
				"tag1" => "value21",
				"tag2" => "value32",
				"tag3" => "value23",
				"tag4" => "value34");
			
			$this->TestCase_LoadConfig(
				$arrayConfigStoreLocations,
				$arrayConfigIDs,
				$arrayExpected);





		}
		
		
	}
	
	

		
