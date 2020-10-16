<?php
	
	require_once(GetWDKDir()."wdk_unittest_recursivefilecheck.inc");
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct("Check for google-analytics.js in public js folder");
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);
			
			$strFilePath = GetDocumentRootDir() . 'js/google-analytics.js';
			if (IsFile($strFilePath) == true)
			{
				$this->Trace("The google-analytics.js file was found in the public js folder. This will not work as the google analytics javascript file requires code preprocessing.\n$strFilePath");
				$this->SetResult(false);				
			}
			
			
		}
	}
	
	

		
