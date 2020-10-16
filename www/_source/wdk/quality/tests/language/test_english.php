<?php
	
	require_once(GetWDKDir().'wdk_unittest_recursivefilecheck.inc');
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct('Check for typical mistakes made in English copy text and translations.');
		}
		 
		
		
		function Callback_TestCase_CheckFile($strFilePath)
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
				// wdk.txt and content_wdkdocs-releasenotes.txt contain
				// references on the cases checked here.
				// So they must be excluded.
				if (	$strFileName == 'wdk.txt'
					||	$strFileName == 'content_wdkdocs-prologue-releasenotes.txt')
				{
					return;	
				}

				$arrayRegExp[] = '/imprint/';
					
			}
			$this->CheckFileAgainstRegExp($strFilePath,$arrayRegExp);
		}

		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);
			$this->CheckSourceDirectories();
		}
	}
	
	

		
