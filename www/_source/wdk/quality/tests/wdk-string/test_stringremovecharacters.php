<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('StringRemoveCharacters');
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringRemoveCharacters($strString,$strBlacklist,$strExpectedResult)
		{
			$this->Trace('TestCase_StringRemoveCharacters');
			$this->Trace('Test String                   : "'.$strString.'"');
			$this->Trace('Blacklist                     : "'.$strBlacklist.'"');   
			$this->Trace('Expected Result               : "'.$strExpectedResult.'"');
			$strResult = StringRemoveCharacters($strString,$strBlacklist); 
			$this->Trace('StringRemoveCharacters returns: "'.$strResult.'"');
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
			
		
			$this->TestCase_StringRemoveCharacters(
				'abc',
				'ab',
				'c');

			$this->TestCase_StringRemoveCharacters(
				'abc',
				'',
				'abc');

			$this->TestCase_StringRemoveCharacters(
				'abc',
				'c',
				'ab');

			$this->TestCase_StringRemoveCharacters(
				'This is a test',
				'abcdefghijklmnopqrstuvwxyz',
				'T   ');

			$this->TestCase_StringRemoveCharacters(
				'',
				'',
				'');

			$this->TestCase_StringRemoveCharacters(
				'abc',
				'def',
				'abc');

			$this->TestCase_StringRemoveCharacters(
				'=���=',
				'=',
				u('���'));

			$this->TestCase_StringRemoveCharacters(
				u('=���='),
				'=',
				u('���'));


		}
		

	}
	
	

		
