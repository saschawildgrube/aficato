<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Toolbar");
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
			$strURL = $strURL_Root."?content=test-element-toolbar";
			$strURL_Image = $strURL_Root."?layout=default&amp;command=image&amp;";


			

			
			$this->TestCase_CheckURL(
				$strURL,
				array('<div><a href="http://www.example.com"><i class="fa fa-link fa-fw" aria-hidden="true"></i>Example.com</a>&nbsp;<a href="http://www.example.com"><i class="fa fa-link fa-fw" aria-hidden="true"></i></a>&nbsp;<a href="http://www.example.com" target="_blank">Example.com</a></div>'));
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			return true;
		}
		
		
	}
	
	
		


		
