<?php
	
	require_once(GetWDKDir().'wdk_unittest_recursivefilecheck.inc');
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct('Check for typical mistakes made in English copy text and translations.');
		}
		 
		
		
		function OnTestCaseCheckFile($strFilePath)
		{ 
			$arrayRegExp = array();
			$strExtention = GetExtentionFromPath($strFilePath);
			if (	$strExtention == 'htm'
				||  $strExtention == 'txt')
			{
				$strFileName = GetFilenameFromPath($strFilePath);
				// we don't want to fail the test because of THIS file!
				if ($strFileName == 'test_english.php')
				{
					return;	
				}
				// content_wdkdocs-releasenotes.txt contains
				// references to some of the cases checked here.
				if ($strFileName != 'content_wdkdocs-releasenotes.txt')
				{
					$arrayRegExp[] = '/imprint/';	
				}
					
			}
			$this->CheckFileAgainstRegExp($strFilePath,$arrayRegExp);
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$this->CheckSourceDirectories();
		}
	}
	
	

		
