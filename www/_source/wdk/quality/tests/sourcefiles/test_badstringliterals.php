<?php
	
	require_once(GetWDKDir().'wdk_unittest_recursivefilecheck.inc');
	
	class CTest extends CUnitTestRecursiveFileCheck
	{
		function __construct()
		{
			parent::__construct('Check for broken string literals php source code');
		}
		
		function Callback_TestCase_CheckFile($strFilePath)
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

		function CallbackTest()
		{
			parent::CallbackTest();
			$this->SetResult(true);
			$this->CheckSourceDirectories();
		}
	}
	
	

		
