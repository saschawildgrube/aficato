<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('StringFilter');
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringFilter($strString,$strFilter,$strExpectedResult)
		{
			$this->Trace('TestCase_StringFilter');
			$this->Trace('Test String         : '.$strString);
			$this->Trace('Filter              : '.$strFilter);
			$this->Trace('Expected Result     : '.$strExpectedResult);
			$strResult = StringFilter($strString,$strFilter); 
			$this->Trace('StringFilter returns: '.$strResult);
			if ($strResult == $strExpectedResult)
			{
				$this->Trace('Testcase PASSED!');
			}
			else
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);
			}
			$this->Trace('');
			
		}


		function OnTest()
		{
			parent::OnTest(); 
			
		
			$this->TestCase_StringFilter(
				'abc',
				'ab',
				'ab');

			$this->TestCase_StringFilter(
				'abc',
				'',
				'');

			$this->TestCase_StringFilter(
				'abc',
				'c',
				'c');

			$this->TestCase_StringFilter(
				'This is a test',
				'abcdefghijklmnopqrstuvwxyz',
				'hisisatest');
		}
		

	}
	
	

		
