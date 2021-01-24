<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test generic output items within textareas");
		}
		
		function OnInit()
		{
			parent::OnInit(); 
			$this->SetActive(false);
			return true;
		}
		
		function TestCase_GenericOutputItems($strContent,$arrayExpectedOutput)
		{
			$this->TestCase_CheckURL(
				$strTestWebsiteURL = "http://".GetRootURL()."quality/testwebsite/?content=test-genericoutputitem-".$strContent,
				$arrayExpectedOutput);
		}
		
		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			
			$arrayExpectedOutput[] = '<a href="http://'.GetRootURL()."quality/testwebsite/en/test-genericoutputitem-textarea/";
			$arrayExpectedOutput[] = '{URL content=""}';
		
			$this->TestCase_GenericOutputItems(
				"textarea",
				$arrayExpectedOutput);


		}
		

		 
		
	} 
	
	
		


		
