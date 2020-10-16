<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Generic Output Items");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			return true;
		}
		
		function TestCase_GenericOutputItem($strContent,$arrayExpectedOutput)
		{
			$this->TestCase_CheckURL(
				$strTestWebsiteURL = "http://".GetRootURL()."quality/testwebsite/?content=".$strContent,
				$arrayExpectedOutput);
			
			$this->Trace("");	
			$this->Trace("");	
			
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);
			
			$this->TestCase_GenericOutputItem(
				"test-genericoutputitem",
				array(
					//HtmlEncode("http://".GetRootURL()."quality/testwebsite/?content=index&language=EN"),
					HtmlEncode("http://".GetRootURL()."quality/testwebsite/en/"),
					HtmlEncode("http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=icon_test"),
					HtmlEncode("http://".GetRootURL()."quality/testwebsite/?layout=default&command=image&id=test")
				)
				);

				
				
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
		
		
	}
	
	
		


		
