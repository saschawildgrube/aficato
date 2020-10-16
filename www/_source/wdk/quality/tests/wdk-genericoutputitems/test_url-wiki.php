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
			
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/en/test-genericoutputitem-url-wiki/";
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/en/";
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/en/";
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/de/";

			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/en/only-en/";
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/de/only-en/";
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/en/only-de/";
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/de/only-de/";

			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/en/test-routing1/";
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/de/test-routing1/";
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/en/Routing-with-Alias/";
			$arrayExpectedOutput[] = '<li><a href="http://'.GetRootURL()."quality/testwebsite/de/Routing-mit-Alias/";

		
			$this->TestCase_GenericOutputItems(
				"url-wiki",
				$arrayExpectedOutput);


		}
		

		 
		
	} 
	
	
		


		
