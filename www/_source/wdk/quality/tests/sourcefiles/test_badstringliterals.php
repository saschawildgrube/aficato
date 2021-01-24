<?php
	
	require_once(GetWDKDir().'wdk_unittest_recursivefilecheck.inc');
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct('Check for broken string literals php source code');
		}
		
		function OnTestCaseCheckFile($strFilePath)
		{ 
			$strExtention = GetExtentionFromPath($strFilePath);
			if (	$strExtention == 'inc'
				||  $strExtention == 'php')
			{
				$strFileName = GetFilenameFromPath($strFilePath);
				
				// We make an exception for the mysql wrapper code
				if ($strFileName == 'wdk_mysql.inc')
				{
					return;	
				}
	
				$arrayRegExp = array();
				$arrayRegExp[] = '/\'\\\\n/';
				$arrayRegExp[] = '/\\\\n\'/';
				$arrayRegExp[] = '/\'\\\\r/';
				$arrayRegExp[] = '/\\\\r\'/';
		
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
	
	

		
