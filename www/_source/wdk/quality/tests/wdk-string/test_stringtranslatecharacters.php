<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringTranslateCharacters");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringTranslateCharacters($strString,$strFrom,$strTo,$strExpectedResult)
		{
			$this->Trace("TestCase_StringTranslateCharacters");
			$this->Trace("strString = ".RenderValue($strString).", strFrom = ".RenderValue($strFrom).", strTo = ".RenderValue($strTo)."");
			$this->Trace("Expected Result: ".RenderValue($strExpectedResult));
			$strResult = StringTranslateCharacters($strString,$strFrom,$strTo);
			$this->Trace("ReplaceString returns: ".RenderValue($strResult));
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

/*
		function TestCase_ReplaceStringIgnoreCase($strString,$strFrom,$strTo,$strExpectedResult)
		{
			$this->Trace("TestCase_ReplaceStringIgnoreCase");
			$this->Trace("strString = \"$strString\", strFrom = \"$strFrom\", strTo = \"$strTo\"");
			$this->Trace("Expected Result:                 \"$strExpectedResult\"");
			$strResult = ReplaceStringIgnoreCase($strString,$strFrom,$strTo);
			$this->Trace("ReplaceStringIgnoreCase returns: \"$strResult\"");
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
*/
		
		function OnTest()
		{
			parent::OnTest();
			

			$this->TestCase_StringTranslateCharacters(
				"abcdefghijklmnopqrstuvwxyz",
				"",
				"",
				"abcdefghijklmnopqrstuvwxyz");

			$this->TestCase_StringTranslateCharacters(
				"abcdefghijklmnopqrstuvwxyz",
				3,
				array(),
				"abcdefghijklmnopqrstuvwxyz");

		
			$this->TestCase_StringTranslateCharacters(
				"abcdefghijklmnopqrstuvwxyz",
				"ghi",
				"GHI",
				"abcdefGHIjklmnopqrstuvwxyz");

			$this->TestCase_StringTranslateCharacters(
				"Loram ipsum dolor",
				"mn",
				"xx",
				"Lorax ipsux dolor");

			$this->TestCase_StringTranslateCharacters(
				u("ÄÖÜ"),
				u("Ä"),
				"AE",
				u("AÖÜ"));
			
			$this->TestCase_StringTranslateCharacters(
				u("ÄÖÜ"),
				u("Ä"),
				u("AE"),
				u("AÖÜ"));
			
			$this->TestCase_StringTranslateCharacters(
				u("ß"),
				u("Ä"),
				u("AE"),
				u("ß"));
	
				
			$this->TestCase_StringTranslateCharacters(
				u("123 blubber 123 hello world"),
				u("123"),
				u("dadada"),
				u("dad blubber dad hello world"));
			
			

		}
		

	}
	
	

		
