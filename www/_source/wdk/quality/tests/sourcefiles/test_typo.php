<?php
	
	require_once(GetWDKDir().'wdk_unittest_recursivefilecheck.inc');
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct('Check for typical typos in source code files');
		}

		function OnTestCaseCheckFile($strFilePath)
		{ 
			$arrayRegExp = array();
			$strExtention = GetExtentionFromPath($strFilePath);
			$strFileName = GetFilenameFromPath($strFilePath);
			
			// We don't want to fail the test because of THIS file!
			if ($strFileName == 'test_typo.php')
			{
				return;	
			}
			
			// We don't check minified 3rd party js files
			if (FindString($strFileName,'.min.js') != -1)
			{
				return;	
			}

			if (	$strExtention == 'inc'
				||  $strExtention == 'php'
				||  $strExtention == 'js'  
				||  $strExtention == 'txt'
				||  $strExtention == 'md'
				||  $strExtention == 'htm')
			{
				$arrayRegExp = array();
				$arrayRegExp[] = '/retrun/i';
				$arrayRegExp[] = '/provacy/i';
				$arrayRegExp[] = '/ teh /i';
				$arrayRegExp[] = '/plgin/i';
				$arrayRegExp[] = '/steepphp/i';
				$arrayRegExp[] = '/scallfolder/i';
				$arrayRegExp[] = '/licence/i';
				
				$this->CheckFileAgainstRegExp($strFilePath,$arrayRegExp);
			}
			
			return;
		}

		function OnTest()
		{
			parent::OnTest();
			$this->SetResult(true);
			$this->CheckSourceDirectories();
		}
	}
	
	

		
