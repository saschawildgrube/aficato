<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("TestWebsite self check");
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

			$strURL = "http://".GetRootURL()."quality/testwebsite/";
			$this->TestCase_CheckURL($strURL,array("This is an instance of a WDK test website."));
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			
			if ($this->GetResult() == false)
			{
				$this->Trace("PLEASE NOTE:
Some tests in the wdk assembly require the setup of a test website.
E.g. all component tests that require to be embedded into a CWebsite derived class.
The source code of the test website is in the wdk/quality/testwebsite/ directory.
In order to make the test website accessible a proxy must be set up which includes the index.php file from the test website directory.
				
Please do the following to setup the test website:
1. Create a directory underneath your root url \"/quality/testwebsite/\". E.g. http://www.example.com/quality/testwebsite/.
2. Create an index.php in this directory which includes the file /wdk/quality/testwebsite/index.php. You can find templates in /templates/testwebsite/.
3. Adapt the env.inc and .htaccess files.
				");	
			}
			
			return true;
		}
		
		
	}
	
	
		


		
