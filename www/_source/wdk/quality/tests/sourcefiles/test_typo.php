<?php
	
	require_once(GetWDKDir().'wdk_unittest_recursivefilecheck.inc');
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct('Check for typical typos in source code files');
		}

		function Callback_TestCase_CheckFile($strFilePath)
		{ 
			$arrayRegExp = array();
			$strExtention = GetExtentionFromPath($strFilePath);
			$strFileName = GetFilenameFromPath($strFilePath);
			
			// we don't want to fail the test because of THIS file!
			if ($strFileName == 'test_typo.php')
			{
				return;	
			}

			if (	$strExtention == 'inc'
				||  $strExtention == 'php'
				||  $strExtention == 'txt'
				||  $strExtention == 'htm')
			{
				$arrayRegExp = array();
				$arrayRegExp[] = '/retrun/';
				$arrayRegExp[] = '/provacy/';
				$arrayRegExp[] = '/ teh /';
				$this->CheckFileAgainstRegExp($strFilePath,$arrayRegExp);
			}
			
			return;
		}

		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);
			$this->CheckSourceDirectories();
		}
	}
	
	

		
