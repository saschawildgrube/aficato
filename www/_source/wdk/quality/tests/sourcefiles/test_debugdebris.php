<?php
	
	require_once(GetWDKDir()."wdk_unittest_recursivefilecheck.inc");
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct("Check for unintended debug output in php source code");
		}
		
		function OnTestCaseCheckFile($strFilePath)
		{ 
			$strExtention = GetExtentionFromPath($strFilePath);
			if (	$strExtention == "inc"
				||  $strExtention == "php")
			{
				$strFileName = GetFilenameFromPath($strFilePath);
				// we don't want to fail the test because of THIS file!
				if ($strFileName == "test_debugdebris.php")
				{
					return;	
				}
				$arrayRegExp = array();
				$arrayRegExp[] = '/error_log\(/';
				$arrayRegExp[] = '/var_dump\(/';
				$arrayRegExp[] = '/this\-\>Debug\(/';
				$arrayRegExp[] = '/flush\(\)/';

				if ($strFileName != "wdk_arrayprint.inc") 
				{
					$arrayRegExp[] = '/print_r\(/';
				}
				if ($strFileName != "wdk_websitesatellite.inc") 
				{
					$arrayRegExp[] = '/this\-\>GetWebsite\(\)\-\>Debug\(/';
				}
					
				$this->CheckFileAgainstRegExp($strFilePath,$arrayRegExp);
			}
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$this->CheckSourceDirectories();
		}
	}
	
	

		
