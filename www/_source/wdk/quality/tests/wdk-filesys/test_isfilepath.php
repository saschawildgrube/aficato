<?php
	
	require_once(GetWDKDir().'wdk_filesys.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('IsFilePath');
		}
		

		function TestCase_IsFilePath(
			$strPath,
			$bExpectedResult)
		{ 
			$this->Trace('TestCase_IsFilePath');
	
			$this->Trace('strPath = '.$strPath);
			$this->Trace('bExpectedResult: '.RenderBool($bExpectedResult));
			
			$bResult = IsFilePath($strPath);
			$this->Trace('IsFilePath() returns: '.RenderBool($bResult));
			if ($bResult == $bExpectedResult)
			{
				$this->Trace('Testcase PASSED!');
			}
			else
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);	
			}

			$this->Trace('');
			$this->Trace('');
		}


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);

			$this->TestCase_IsFilePath(
				'file.ext',
				true);

			$this->TestCase_IsFilePath(
				'/file.ext',
				true);

			$this->TestCase_IsFilePath(
				'/file',
				true);

			$this->TestCase_IsFilePath(
				'/folder1/file.ext',
				true);

			$this->TestCase_IsFilePath(
				'/',
				false);

			$this->TestCase_IsFilePath(
				'/folder/',
				false);

			$this->TestCase_IsFilePath(
				'/folder/subfolder/',
				false);

			$this->TestCase_IsFilePath(
				'',
				false);

			$this->TestCase_IsFilePath(
				':%&$',
				false);

			
		}
		
		
	}
	
	

		
