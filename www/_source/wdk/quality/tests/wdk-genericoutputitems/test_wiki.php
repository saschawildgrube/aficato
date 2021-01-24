<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test generic output items with WIKI content");
		}
		
		function OnInit()
		{
			parent::OnInit(); 
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
			
			for ($nCount = 1; $nCount <= 15; $nCount++)
			{
				$arrayExpectedOutput[] = '<a href="http://'.GetRootURL().'quality/testwebsite/en/test'.$nCount;
			}
			
			$this->TestCase_GenericOutputItems(
				"wiki",
				$arrayExpectedOutput);
			
		}
		

		 
		
	} 
	
	
		


		
