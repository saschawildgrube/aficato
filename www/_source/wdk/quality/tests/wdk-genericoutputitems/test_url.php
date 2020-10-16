<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test generic output item URL");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			return true;
		}
		
		function TestCase_GenericOutputItems($strContent,$arrayExpectedOutput)
		{
			$this->TestCase_CheckURL(
				$strTestWebsiteURL = "http://".GetRootURL()."quality/testwebsite/?content=test-genericoutputitem-".$strContent,
				$arrayExpectedOutput);
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);
			
			$arrayExpectedOutput[] = "1:http://".GetRootURL()."quality/testwebsite/en/test-genericoutputitem-url/";
			$arrayExpectedOutput[] = "2:http://".GetRootURL()."quality/testwebsite/en/";
			$arrayExpectedOutput[] = "3:http://".GetRootURL()."quality/testwebsite/en/";
			$arrayExpectedOutput[] = "4:http://".GetRootURL()."quality/testwebsite/de/";

			$arrayExpectedOutput[] = "5:http://".GetRootURL()."quality/testwebsite/en/only-en/";
			$arrayExpectedOutput[] = "6:http://".GetRootURL()."quality/testwebsite/de/only-en/";
			$arrayExpectedOutput[] = "7:http://".GetRootURL()."quality/testwebsite/en/only-de/";
			$arrayExpectedOutput[] = "8:http://".GetRootURL()."quality/testwebsite/de/only-de/";

			$arrayExpectedOutput[] = "9:http://".GetRootURL()."quality/testwebsite/en/test-routing1/";
			$arrayExpectedOutput[] = "10:http://".GetRootURL()."quality/testwebsite/de/test-routing1/";
			$arrayExpectedOutput[] = "11:http://".GetRootURL()."quality/testwebsite/en/Routing-with-Alias/";
			$arrayExpectedOutput[] = "12:http://".GetRootURL()."quality/testwebsite/de/Routing-mit-Alias/";

			$this->TestCase_GenericOutputItems(
				"url",
				$arrayExpectedOutput);
			
		}
		

		 
		
	} 
	
	
		


		
