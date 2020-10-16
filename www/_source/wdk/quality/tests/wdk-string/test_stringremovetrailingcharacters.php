<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringRemoveTrailingCharacters");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringRemoveTrailingCharacters($strString,$strBlacklist,$bMultiple,$strExpectedResult)
		{
			$this->Trace("TestCase_StringRemoveTrailingCharacters");
			$this->Trace("Test String                           : \"$strString\"");
			$this->Trace("Blacklist                             : \"$strBlacklist\"");
			$this->Trace("Multiple                              : ".RenderBool($bMultiple));
			$this->Trace("Expected Result                       : \"$strExpectedResult\"");
			$strResult = StringRemoveTrailingCharacters($strString,$strBlacklist,$bMultiple); 
			$this->Trace("StringRemoveTrailingCharacters returns: \"$strResult\"");
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
			
		
			$this->TestCase_StringRemoveTrailingCharacters(
				"abc/",
				"/",
				false,
				"abc");

			$this->TestCase_StringRemoveTrailingCharacters(
				"/",
				"/",
				false,
				"");


			$this->TestCase_StringRemoveTrailingCharacters(
				"abc/",
				"",
				false,
				"abc/");

			$this->TestCase_StringRemoveTrailingCharacters(
				"abc",
				"/",
				false,
				"abc");

			$this->TestCase_StringRemoveTrailingCharacters(
				"abc/abc",
				"/",
				false,
				"abc/abc");

			$this->TestCase_StringRemoveTrailingCharacters(
				"abc/abc/",
				"/",
				false,
				"abc/abc");

			$this->TestCase_StringRemoveTrailingCharacters(
				"abc",
				false,
				false,
				"abc");

			$this->TestCase_StringRemoveTrailingCharacters(
				"abc",
				"abc",
				false,
				"ab");

			$this->TestCase_StringRemoveTrailingCharacters(
				"abcccccc",
				"c",
				true,
				"ab");
				
			$this->TestCase_StringRemoveTrailingCharacters(
				"abc",
				"abc",
				true,
				"");
				

		}
		

	}
	
	

		
