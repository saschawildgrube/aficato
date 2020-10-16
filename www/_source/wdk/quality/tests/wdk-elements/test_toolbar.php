<?php
	

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test Element Toolbar");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			$this->SetResult(true);			
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$strURL_Root = "http://".GetRootURL()."quality/testwebsite/";
			$strURL = $strURL_Root."?content=test-element-toolbar";
			$strURL_Image = $strURL_Root."?layout=default&amp;command=image&amp;";


			

			
			$this->TestCase_CheckURL(
				$strURL,
				array('<div><a href="http://www.example.com"><img src="'.$strURL_Image.'id=icon_link" alt="" title="Example.com" style="vertical-align:middle;"/>Example.com</a>&nbsp;<a href="http://www.example.com"><img src="'.$strURL_Image.'id=icon_link" alt="" title="" style="vertical-align:middle;"/></a>&nbsp;<a href="http://www.example.com" target="_blank">Example.com</a></div>'));
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
		
		
	}
	
	
		


		
