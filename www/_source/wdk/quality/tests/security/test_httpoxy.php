<?php
	
	require_once(GetWDKDir()."wdk_unittest_recursivefilecheck.inc");
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct("Httpoxy Vulnerability CVE-2016-5385 (PHP) and CVE-2016-5387 (Apache HTTP Server)");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		
		
				
		function Callback_TestCase_CheckFile($strFilePath)
		{ 
			$arrayRegExp = array();
			$strExtention = GetExtentionFromPath($strFilePath);
			if (	$strExtention == "inc"
				||  $strExtention == "php")
			{
				$strFileName = GetFilenameFromPath($strFilePath);
				// we don't want to fail the test because of THIS file!
				if ($strFileName == "test_httpoxy.php")
				{
					return;	
				}
				// and we don't want to fail the test because of the remote part of the test!
				if ($strFileName == "content_test-httpoxy.inc")
				{
					return;		
				}
				$arrayRegExp = array
					(
						"/HTTP_PROXY/"
					);
			}
			$this->CheckFileAgainstRegExp($strFilePath,$arrayRegExp);
		}

		function CallbackTest()
		{
			parent::CallbackTest();

			$this->Trace("Checking for the Httpoxy vulnerability (CVE-2016-5385):");
			$this->Trace("For more information go to:");
			$this->Trace("https://httpoxy.org/");
			$this->Trace("");
			$this->Trace("");
		
			$this->Trace("Checking web server behaviour:");
		
			$this->TestCase_CheckURL(
				"http://".GetRootURL()."quality/testwebsite/?content=test-httpoxy",
				array(),
				array(
					'$_SERVER[&quot;HTTP_PROXY&quot;]: 1.1.1.1',
					'getenv(&quot;HTTP_PROXY&quot;): 1.1.1.1'),
				array(),
				array(), // Params
				array(), // Cookies
				array(
					"Proxy" => "1.1.1.1"),
				"get",
				5,
				true,
				false);

			$this->Trace("");
			$this->Trace("");

		
			$this->Trace("If the test case fails, add this to the .htaccess file:");
			$this->Trace("<IfModule mod_headers.c>");
   		$this->Trace("   RequestHeader unset Proxy");
			$this->Trace("</IfModule>");
		
			$this->Trace("");
			$this->Trace("");
	
			$this->Trace("Checking the source code for active usage of the HTTP_PROXY environment value:");
			$this->CheckSourceDirectories();
	
			
		}
	}
		
