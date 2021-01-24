<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("FindStringMultiple(IgnoreCase)");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_FindStringMultiple($strHaystack,$arrayNeedles,$nOffset,$nExpectedResultPos,$strExpectedResultNextNeedle)
		{
			$this->Trace("TestCase_FindStringMultiple");
			$this->Trace("strHaystack = \"$strHaystack\"");
			$this->Trace("Needles:");
			$this->Trace($arrayNeedles);
			$this->Trace("Offset = $nOffset");
			$this->Trace("Expected Result: return = ".$nExpectedResultPos); 
			$this->Trace("Expected Result: strNextNeedle = \"".$strExpectedResultNextNeedle."\"");
			$strNextNeedle = "";
			$nResult = FindStringMultiple($strHaystack,$arrayNeedles,$strNextNeedle,false,$nOffset);
			$this->Trace("FindString returns: ".$nResult);
			$this->Trace("strNextNeedle = \"".$strNextNeedle."\"");
			if ($nResult == $nExpectedResultPos && $strNextNeedle == $strExpectedResultNextNeedle)
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
		
		function TestCase_FindStringMultipleIgnoreCase($strHaystack,$arrayNeedles,$nOffset,$nExpectedResultPos,$strExpectedResultNextNeedle)
		{
			$this->Trace("TestCase_FindStringMultipleIgnoreCase");
			$this->Trace("strHaystack = \"$strHaystack\"");
			$this->Trace("Needles:");
			$this->Trace($arrayNeedles);
			$this->Trace("Offset = $nOffset");
			$this->Trace("Expected Result: return = ".$nExpectedResultPos);
			$this->Trace("Expected Result: strNextNeedle = \"".$strExpectedResultNextNeedle."\"");
			$strNextNeedle = "";
			$nResult = FindStringMultipleIgnoreCase($strHaystack,$arrayNeedles,$strNextNeedle,$nOffset);
			$this->Trace("FindStringIgnoreCase returns: ".$nResult);
			$this->Trace("strNextNeedle = \"".$strNextNeedle."\"");
			if ($nResult == $nExpectedResultPos && $strNextNeedle == $strExpectedResultNextNeedle)
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
		
			$this->TestCase_FindStringMultiple("abcdefghijklmnopqrstuvwxyz",array("ghi"),0,6,"ghi");
			$this->TestCase_FindStringMultiple("abcdefghijklmnopqrstuvwxyz",array("zyx"),0,-1,"");
			$this->TestCase_FindStringMultiple("abcdefghijklmnopqrstuvwxyz",array("abc"),0,0,"abc");
			$this->TestCase_FindStringMultiple("abcdefghijklmnopqrstuvwxyz",array("abc","ghi"),0,0,"abc");
			$this->TestCase_FindStringMultiple("abcdefghijklmnopqrstuvwxyz",array("ghi","uvw","xyz"),0,6,"ghi");
			$this->TestCase_FindStringMultiple("abcdefghijklmnopqrstuvwxyz",array("ghi","uvw","xyz"),7,20,"uvw");			 
			$this->TestCase_FindStringMultiple("abcdefghijklmnopqrstuvwxyz",array("ghi","uvw","xyz"),100,-1,null);

			$this->TestCase_FindStringMultipleIgnoreCase("abcdefghIjklmnopqrstuvwxyz",array("Ghi"),0,6,"Ghi");
			$this->TestCase_FindStringMultipleIgnoreCase("abcdefghIjklmnopqrstuvwxyz",array("zyx"),0,-1,"");
			$this->TestCase_FindStringMultipleIgnoreCase("abcdefghIjklmnopqrstuvwxyz",array("abC"),0,0,"abC");
			$this->TestCase_FindStringMultipleIgnoreCase("abcdefghIjklmnopqrstuvwxyz",array("abc","ghi"),0,0,"abc");
			$this->TestCase_FindStringMultipleIgnoreCase("abcdefghIjklmnopqrstuvwxyz",array("ghi","uvw","xyz"),7,20,"uvw");
		}
	}
		
