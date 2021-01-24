<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Fallback Language");
		}
		
		function OnInit()
		{
			parent::OnInit(); 
			return true;
		}
		

		
		function OnTest()
		{
			parent::OnTest();
			
			$this->SetResult(true);


			$arrayExpectedOutput = array(
				"defaultdescription");
			

			$this->TestCase_CheckURL(
				"http://".GetRootURL()."quality/testwebsite/?language=de&fallbacklanguage=en",
				array(
					"defaultdescription")
				);

			$this->TestCase_CheckURL(
				"http://".GetRootURL()."quality/testwebsite/?language=de",
				array(),
				array(
					"defaultdescription")
				);

				
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			return true;
		}
		
		
	}
	
	
		


		
