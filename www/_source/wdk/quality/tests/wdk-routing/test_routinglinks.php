<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test the use of aliases in wiki and html links");
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

			$strURL = $strURL_Root."test-routing-html/";
			$this->TestCase_CheckURL(
				$strURL,
				array($strURL_Root."en/Routing-with-Alias/"));

			$strURL = $strURL_Root."de/test-routing-html/";
			$this->TestCase_CheckURL(
				$strURL,
				array($strURL_Root."de/Routing-mit-Alias/"));


			// Currently the wiki element does not support the routing alias feature
			/*
			$strURL = $strURL_Root."test-routing-wiki/";
			$this->TestCase_CheckURL(
				$strURL,
				array($strURL_Root."en/Routing-with-Alias/"));
			*/
			
				
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			return true;
		}
		
		
	}
	
	
		


		
