<?php
	
	require_once(GetWDKDir().'wdk_unittest_recursivefilecheck.inc');
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct('Check for problems in xhtml files');
		}

		function OnTestCaseCheckFile($strFilePath)
		{ 
			$arrayRegExp = array();
			$strExtention = GetExtentionFromPath($strFilePath);
			$strFileName = GetFilenameFromPath($strFilePath);
			
			// we don't want to fail the test because of THIS file!

			if (	$strExtention == 'htm'
				||  $strExtention == 'txt')
			{
				$arrayRegExp = array();
				$arrayRegExp[] = '/--->/';
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
	
	

		
