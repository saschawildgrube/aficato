<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("FindString");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_FindString($strHaystack,$strNeedle,$nExpectedResult)
		{
			$this->Trace("TestCase_FindString");
			$this->Trace("strHaystack = \"$strHaystack\", strNeedle = \"$strNeedle\"");
			$this->Trace("Expected Result: ".intval($nExpectedResult)."");
			$nResult = FindString($strHaystack,$strNeedle);
			$this->Trace("FindString returns: $nResult");
			if ($nResult == $nExpectedResult)
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
		
		function TestCase_FindStringIgnoreCase($strHaystack,$strNeedle,$nExpectedResult)
		{
			$this->Trace("TestCase_FindString");
			$this->Trace("strHaystack = \"$strHaystack\", strNeedle = \"$strNeedle\"");
			$this->Trace("Expected Result: ".intval($nExpectedResult)."");
			$nResult = FindStringIgnoreCase($strHaystack,$strNeedle);
			$this->Trace("FindStringIgnoreCase returns: $nResult");
			if ($nResult == $nExpectedResult)
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
			
			$this->TestCase_FindString("abcdefghijklmnopqrstuvwxyz","ghi",6);
			$this->TestCase_FindString("abcdefghijklmnopqrstuvwxyz","zyx",-1);
			$this->TestCase_FindString("abcdefghijklmnopqrstuvwxyz","abc",0);
			$this->TestCase_FindString("abcdefghijklmnopqrstuvwxyz","",-1);
			$this->TestCase_FindString("","test",-1);
			$this->TestCase_FindString(u("Äbcdefghijklmnopqrstüvwxyz"),u("stüvwx"),18); 
	
			$this->TestCase_FindStringIgnoreCase("abcdefGhIjklmnopqrstuvwxyz","ghi",6);
			$this->TestCase_FindStringIgnoreCase("abcdefghijklmnopqrstuvwxyz","ZYX",-1);
			$this->TestCase_FindStringIgnoreCase("Abcdefghijklmnopqrstuvwxyz","abC",0);
			$this->TestCase_FindStringIgnoreCase("abcdefghijklmnopqrstuvwxyz","",-1);
			$this->TestCase_FindStringIgnoreCase("","test",-1);
			$this->TestCase_FindStringIgnoreCase(u("äüöÄÜÖ"),u("Ö"),2);
			

		}
		

	}
	
	

		
