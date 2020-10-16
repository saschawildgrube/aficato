<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringRemoveLeadingCharacters");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringRemoveLeadingCharacters($strString,$strBlacklist,$bMultiple,$strExpectedResult)
		{
			$this->Trace("TestCase_StringRemoveLeadingCharacters");
			$this->Trace("Test String                          : \"$strString\"");
			$this->Trace("Blacklist                            : \"$strBlacklist\"");
			$this->Trace("Multiple                             : ".RenderBool($bMultiple));
			$this->Trace("Expected Result                      : \"$strExpectedResult\"");
			$strResult = StringRemoveLeadingCharacters($strString,$strBlacklist,$bMultiple); 
			$this->Trace("StringRemoveLeadingCharacters returns: \"$strResult\"");
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
			
		
			$this->TestCase_StringRemoveLeadingCharacters(
				"abc/",
				"/",
				false,
				"abc/");

			$this->TestCase_StringRemoveLeadingCharacters(
				"/",
				"/",
				false,
				"");


			$this->TestCase_StringRemoveLeadingCharacters(
				"abc/",
				"",
				false,
				"abc/");

			$this->TestCase_StringRemoveLeadingCharacters(
				"abc",
				"/",
				false,
				"abc");

			$this->TestCase_StringRemoveLeadingCharacters(
				"abc/abc",
				"/",
				false,
				"abc/abc");

			$this->TestCase_StringRemoveLeadingCharacters(
				"/abc/abc/",
				"/",
				false,
				"abc/abc/");

			$this->TestCase_StringRemoveLeadingCharacters(
				"abc",
				false,
				false,
				"abc");

			$this->TestCase_StringRemoveLeadingCharacters(
				"abc",
				"abc",
				false,
				"bc");

			$this->TestCase_StringRemoveLeadingCharacters(
				"abcccccc",
				"c",
				true,
				"abcccccc");

			$this->TestCase_StringRemoveLeadingCharacters(
				"aaaaaaabcccccc",
				"a",
				true,
				"bcccccc");
				
			$this->TestCase_StringRemoveLeadingCharacters(
				"abc",
				"abc",
				true,
				"");
				

		}
		

	}
	
	

		
