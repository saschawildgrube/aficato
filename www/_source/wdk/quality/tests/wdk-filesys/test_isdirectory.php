<?php
	
	require_once(GetWDKDir().'wdk_filesys.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('IsDirectory');
		}
		

		function TestCase_IsDirectory(
			$strDirPath,
			$bExpectedResult)
		{ 
			$this->Trace('TestCase_IsDirectory');
	
			$this->Trace('strDirPath = '.$strDirPath);
			$this->Trace('bExpectedResult: '.RenderBool($bExpectedResult));
			$bResult = IsDirectory($strDirPath);
	
			$this->Trace('IsDirectory() returns: '.RenderBool($bResult));
	
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

			$this->TestCase_IsDirectory(
				'/',
				true);

			
			$this->TestCase_IsDirectory(
				GetWDKDir().'quality/testdir/dir1/',
				true);

			$this->TestCase_IsDirectory(
				GetWDKDir().'quality/testdir/dir1',
				false);

			$this->TestCase_IsDirectory(
				GetWDKDir().'quality/testdir/dir1/file1.ext',
				false);

			$this->TestCase_IsDirectory(
				GetWDKDir().'quality/testdir/dir1//',
				false);

			$this->TestCase_IsDirectory(
				'',
				false);

			$this->TestCase_IsDirectory(
				StringSection(GetWDKDir(),1).'quality/testdir/dir1/',
				false);
		
			
			
		}
		
		
	}
	
	

		
