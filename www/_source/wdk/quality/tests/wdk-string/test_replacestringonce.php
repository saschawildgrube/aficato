<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("ReplaceStringOnce");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_ReplaceStringOnce($strString,$strFrom,$strTo,$strExpectedResult)
		{
			$this->Trace("TestCase_ReplaceStringOnce");
			$this->Trace("strString = \"$strString\", strFrom = \"$strFrom\", strTo = \"$strTo\"");
			$this->Trace("Expected Result:       \"$strExpectedResult\"");
			$strResult = ReplaceStringOnce($strString,$strFrom,$strTo);
			$this->Trace("ReplaceString returns: \"$strResult\"");
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

		function TestCase_ReplaceStringOnceIgnoreCase($strString,$strFrom,$strTo,$strExpectedResult)
		{
			$this->Trace("TestCase_ReplaceStringOnceIgnoreCase");
			$this->Trace("strString = \"$strString\", strFrom = \"$strFrom\", strTo = \"$strTo\"");
			$this->Trace("Expected Result:                 \"$strExpectedResult\"");
			$strResult = ReplaceStringOnceIgnoreCase($strString,$strFrom,$strTo);
			$this->Trace("ReplaceStringOnceIgnoreCase returns: \"$strResult\"");
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

		
		function OnTest()
		{
			parent::OnTest();
			
		
			$this->TestCase_ReplaceStringOnce(
				"abcdefghijklmnopqrstuvwxyz",
				"ghi",
				"GHI",
				"abcdefGHIjklmnopqrstuvwxyz");

			$this->TestCase_ReplaceStringOnce(
				"Loram ipsum dolor",
				"m",
				"mam",
				"Loramam ipsum dolor");

			$this->TestCase_ReplaceStringOnce(
				u("ÄÖÜ"),
				u("Ä"),
				"AE",
				u("AEÖÜ"));
			
			$this->TestCase_ReplaceStringOnce(
				u("ÄÖÜ"),
				u("Ä"),
				u("AE"),
				u("AEÖÜ"));
			
			$this->TestCase_ReplaceStringOnce(
				u("ß"),
				u("Ä"),
				u("AE"),
				u("ß"));
	
				
			$this->TestCase_ReplaceStringOnce(
				u("123 blubber 123 hello world"),
				u("123"),
				u("dadada"),
				u("dadada blubber 123 hello world"));
			
			
			$this->TestCase_ReplaceStringOnceIgnoreCase(
				"abcdefGhijklmnopqrstuvwxyz",
				"ghI",
				"GHI",
				"abcdefGHIjklmnopqrstuvwxyz");

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				"Loram ipsuM dolor",
				"M",
				"mam",
				"Loramam ipsuM dolor");
				
			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("äÖÜ"),
				u("Ä"),
				"AE",
				u("AEÖÜ"));

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("äÖÜÄÄ"),
				u("Ä"),
				"AE",
				u("AEÖÜÄÄ"));	
				
			$this->TestCase_ReplaceStringOnceIgnoreCase(
				"aaaaa",
				"a",
				"bb",
				u("bbaaaa"));				

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("äääää"),
				u("ä"),
				u("ÄÄ"),
				u("ÄÄääää"));				

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("äääÄÄ"),
				u("ä"),
				u("ÄÄ"),
				u("ÄÄääÄÄ"));				

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("äääÄÄ"),
				u("ä"),
				"AE",
				u("AEääÄÄ"));		

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("ääÜÄÄ"),
				u("ä"),
				"AE",
				u("AEäÜÄÄ"));	

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("äÖÜ"),
				u("Ä"),
				"&Auml;",
				u("&Auml;ÖÜ"));	
				
			$this->TestCase_ReplaceStringOnceIgnoreCase(
				u("abC blubber ABC hello world"),
				u("abc"),
				u("dadada"),
				u("dadada blubber ABC hello world"));

			$this->TestCase_ReplaceStringOnceIgnoreCase(
				"<p>This text contains a highlighted word. This is a real highlight! Highlighting is case insensitive.</p>",
				"highlight",
				"<strong>highlight</strong>",
				"<p>This text contains a <strong>highlight</strong>ed word. This is a real highlight! Highlighting is case insensitive.</p>");

		}
		

	}
	
	

		
