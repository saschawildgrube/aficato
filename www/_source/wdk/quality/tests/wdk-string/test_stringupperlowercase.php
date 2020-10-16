<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringUpperCase and StringLowerCase");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringUpperCase($strString,$strExpectedResult)
		{
			$this->Trace("TestCase_StringUpperCase");
			$this->Trace("Input				: \"$strString\"");
			$this->Trace("Expected Result	: \"$strExpectedResult\"");
			$strResult = StringUpperCase($strString); 
			$this->Trace("StringUpperCase returns: \"$strResult\"");
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
			
		}

		function TestCase_StringLowerCase($strString,$strExpectedResult)
		{
			$this->Trace("TestCase_StringLowerCase");
			$this->Trace("Input				: \"$strString\"");
			$this->Trace("Expected Result	: \"$strExpectedResult\"");
			$strResult = StringLowerCase($strString); 
			$this->Trace("StringLowerCase returns: \"$strResult\"");
			if ($strResult == $strExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);
			}
			$this->Trace("");
		}


		function CallbackTest()
		{
			parent::CallbackTest(); 

			$this->TestCase_StringLowerCase(
				"The brown fox jumps over the lazy Dog.",
				"the brown fox jumps over the lazy dog.");

			$this->TestCase_StringUpperCase(
				"The brown fox jumps over the lazy Dog.",
				"THE BROWN FOX JUMPS OVER THE LAZY DOG.");

			$this->TestCase_StringLowerCase(
				u('ÄäÖöÜü'),
				u('ääööüü'));

			$this->TestCase_StringUpperCase(
				u('ÄäÖöÜü'),
				u('ÄÄÖÖÜÜ'));

			$this->TestCase_StringLowerCase(
				u('AbC!"§$%&/()=?'),
				u('abc!"§$%&/()=?'));

			$this->TestCase_StringUpperCase(
				u('AbC!"§$%&/()=?'),
				u('ABC!"§$%&/()=?'));

			
		}
		

	}
	
	

		
