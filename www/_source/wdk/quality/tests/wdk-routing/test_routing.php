<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Routing");
		}
		
		function OnInit()
		{
			parent::OnInit(); 
			$this->SetResult(true);			
			return true;
		}
		
		function OnTest()
		{
			parent::OnTest();

			$strURL_Root = "http://".GetRootURL()."quality/testwebsite/";

			$strURL = $strURL_Root."test-routing1/";
			$this->TestCase_CheckURL(
				$strURL,
				array("ROUTING1"));
				
			$strURL = $strURL_Root."Routing-with-Alias/";
			$this->TestCase_CheckURL(
				$strURL,
				array("ROUTING2"));				

			$strURL = $strURL_Root."en/";
			$this->TestCase_CheckURL(
				$strURL,
				array("This is an instance of a WDK test website."));				

			$strURL = $strURL_Root."de/";
			$this->TestCase_CheckURL(
				$strURL,
				array("Dies ist eine WDK-Test-Website."));				

				
			$strURL = $strURL_Root."en/test-language/";
			$this->TestCase_CheckURL(
				$strURL,
				array("LANGUAGE-EN"));				

			$strURL = $strURL_Root."de/test-language/";
			$this->TestCase_CheckURL(
				$strURL,
				array("LANGUAGE-DE"));				

			$strURL = $strURL_Root."en/Language-with-Alias/";
			$this->TestCase_CheckURL(
				$strURL,
				array("LANGUAGE-ALIAS-EN"));				

			$strURL = $strURL_Root."de/Sprache-mit-Alias/";
			$this->TestCase_CheckURL(
				$strURL,
				array("LANGUAGE-ALIAS-DE"));				


				
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			return true;
		}
		
		
	}
	
	
		


		
