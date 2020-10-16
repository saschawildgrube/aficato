<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringCount");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringCount($strHaystack,$strNeedle,$nExpectedResult)
		{
			$this->Trace("TestCase_StringCount");
			$this->Trace("strHaystack = \"$strHaystack\"");
			$this->Trace("strNeedle = \"$strNeedle\"");
			$this->Trace("Expected Result: ".intval($nExpectedResult)."");
			$nResult = StringCount($strHaystack,$strNeedle);
			$this->Trace("StringCount returns: $nResult");
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
		
		function TestCase_StringCountIgnoreCase($strHaystack,$strNeedle,$nExpectedResult)
		{
			$this->Trace("TestCase_StringCountIgnoreCase");
			$this->Trace("strHaystack = \"$strHaystack\"");
			$this->Trace("strNeedle = \"$strNeedle\"");
			$this->Trace("Expected Result: ".intval($nExpectedResult)."");
			$nResult = StringCountIgnoreCase($strHaystack,$strNeedle);
			$this->Trace("StringCountIgnoreCase returns: $nResult");
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
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->TestCase_StringCount("","zyx",0);
			$this->TestCase_StringCount("abcdefghijklmnopqrstuvwxyz","zyx",0);
			$this->TestCase_StringCount("abcdefghijklmnopqrstuvwxyz","ghi",1);
			$this->TestCase_StringCount("abcdefghijklmnopqrstuvwxyz","abc",1);
			$this->TestCase_StringCount("abcdefghijklmnopqrstuvwxyz","",0);
			$this->TestCase_StringCount("","test",0);
			$this->TestCase_StringCount(u("Äbcdefghijklmnopqrstüvwxyz"),u("stüvwx"),1); 
			$this->TestCase_StringCount("To be or not to be.","be",2); 
	
			$this->TestCase_StringCountIgnoreCase("","ZYX",0);
			$this->TestCase_StringCountIgnoreCase("abcdefghijklmnopqrstuvwxyz","ZYX",0);
			$this->TestCase_StringCountIgnoreCase("abcdefGhIjklmnopqrstuvwxyz","ghi",1);
			$this->TestCase_StringCountIgnoreCase("Abcdefghijklmnopqrstuvwxyz","abC",1);
			$this->TestCase_StringCountIgnoreCase("abcdefghijklmnopqrstuvwxyz","",0);
			$this->TestCase_StringCountIgnoreCase("","test",0);
			$this->TestCase_StringCountIgnoreCase(u("äüöÄÜÖ"),u("Ö"),2);
			$this->TestCase_StringCountIgnoreCase("To be or not to be.","BE",2); 

		}
		

	}
	
	

		
