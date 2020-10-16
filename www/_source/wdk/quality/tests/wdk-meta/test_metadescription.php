<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Meta Description");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			//$this->SetActive(false);
			return true;
		}
		
		function TestCase_MetaDescription(
			$strURL,
			$strExpected)
		{ 
			$this->Trace("");
			$this->Trace("TestCase_MetaDescription");
			$this->TestCase_CheckURL($strURL,array("<meta name=\"description\" content=\"$strExpected\"/>"));
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);

			$strExpected = "defaultdescription";
			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-meta1";
			$this->TestCase_MetaDescription(
				$strURL,
				$strExpected);

			$strExpected = "specificdescription";
			$strURL = "http://".GetRootURL()."quality/testwebsite/?content=test-meta2";
			$this->TestCase_MetaDescription(
				$strURL,
				$strExpected);


		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
		 
		
	} 
	
	
		


		
