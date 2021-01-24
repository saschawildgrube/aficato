<?php
	
	require_once(GetWDKDir().'wdk_filesys.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('IsValidPath');
		}
		

		function TestCase_IsValidPath(
			$strPath,
			$bExpectedResult)
		{ 
			$this->Trace('TestCase_IsValidPath');
	
			$this->Trace('strPath = '.$strPath);
			$this->Trace('bExpectedResult: '.RenderBool($bExpectedResult));
			
			$bResult = IsValidPath($strPath);
			$this->Trace('IsValidPath() returns: '.RenderBool($bResult));
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

			$this->TestCase_IsValidPath(
				'',
				false);

			$this->TestCase_IsValidPath(
				':%&$',
				false);

			$this->TestCase_IsValidPath(
				'file.ext',
				true);

			$this->TestCase_IsValidPath(
				'/file.ext',
				true);

			$this->TestCase_IsValidPath(
				'/file',
				true);

			$this->TestCase_IsValidPath(
				'/folder1/file.ext',
				true);

			$this->TestCase_IsValidPath(
				'/',
				true);

			$this->TestCase_IsValidPath(
				'/folder/',
				true);

			$this->TestCase_IsValidPath(
				'/folder/subfolder/',
				true);

			$this->TestCase_IsValidPath(
				'www/example/_source/wdk/scaffolds/components/entity/templates/www/_source/assemblies/$$$a$$$/content/content_$$$entityname$$$manager.htm',
				true);

			
		}
		
		
	}
	
	

		
