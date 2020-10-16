<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Cookie functions");
		}
		
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);


			$this->TestCase_CheckURL(
				"http://".GetRootURL()."quality/testwebsite/?content=test-cookies",
				array("testcookiepayload"), 
				array(),
				array(),
				array(),
				array("test" => "testcookiepayload"));

		}
		

		
		
	}
	
	
		


		
