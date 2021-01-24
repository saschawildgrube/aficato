<?php
	
	require_once(GetWDKDir().'wdk_filesys.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('IsFile');
		}
		

		function TestCase_IsFile(
			$strFilePath,
			$bExpectedResult)
		{ 
			$this->Trace('TestCase_IsFile');
	
			$this->Trace('strFilePath = '.$strFilePath);
			$this->Trace('bExpectedResult: '.RenderBool($bExpectedResult));
			
			$bResult = IsFile($strFilePath,false);
			$this->Trace('IsFile(,false) returns: '.RenderBool($bResult));
			if ($bResult == $bExpectedResult)
			{
				$this->Trace('Testcase PASSED!');
			}
			else
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);	
			}
			
			$bResult = IsFile($strFilePath,true);
			$this->Trace('IsFile(,true) returns: '.RenderBool($bResult));
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

			$this->TestCase_IsFile(
				GetWDKDir().'wdk.txt',
				true);
			
			$this->TestCase_IsFile(
				GetWDKDir().'quality/testfiles/helloworld.txt',
				true);

			$this->TestCase_IsFile(
				GetWDKDir().'quality/testfiles/empty.txt',
				true);

			$this->TestCase_IsFile(
				GetWDKDir().'quality/testfiles/doesnotexist.txt',
				false);
			
			
		}
		
		
	}
	
	

		
