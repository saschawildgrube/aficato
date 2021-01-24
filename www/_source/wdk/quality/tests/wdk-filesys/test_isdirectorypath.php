<?php
	
	require_once(GetWDKDir().'wdk_filesys.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('IsDirectoryPath');
		}
		

		function TestCase_IsDirectoryPath(
			$strPath,
			$bExpectedResult)
		{ 
			$this->Trace('TestCase_IsDirectoryPath');
	
			$this->Trace('strPath = '.$strPath);
			$this->Trace('bExpectedResult: '.RenderBool($bExpectedResult));
			
			$bResult = IsDirectoryPath($strPath);
			$this->Trace('IsDirectoryPath() returns: '.RenderBool($bResult));
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$this->TestCase_IsDirectoryPath(
				'/',
				true);

			$this->TestCase_IsDirectoryPath(
				'/folder/',
				true);

			$this->TestCase_IsDirectoryPath(
				'/folder/subfolder/',
				true);


			$this->TestCase_IsDirectoryPath(
				'file.ext',
				false);

			$this->TestCase_IsDirectoryPath(
				'/file.ext',
				false);

			$this->TestCase_IsDirectoryPath(
				'/file',
				false);

			$this->TestCase_IsDirectoryPath(
				'/folder1/file.ext',
				false);




			$this->TestCase_IsDirectoryPath(
				'',
				false);

			$this->TestCase_IsDirectoryPath(
				':%&$',
				false);

			
		}
		
		
	}
	
	

		
