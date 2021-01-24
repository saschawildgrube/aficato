<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("RenderHex");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_RenderHex($strString,$strExpectedResult)
		{
			$this->Trace("TestCase_RenderHex");
			$this->Trace("strString        : \"$strString\"");  
			$this->Trace("Expected result  : \"$strExpectedResult\"");
			$strResult = RenderHex($strString); 
			$this->Trace("RenderHex returns: \"$strResult\"");
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

			$this->TestCase_RenderHex(
				"",
				"");

			$this->TestCase_RenderHex(
				"abc",
				"616263");

			$this->TestCase_RenderHex(
				"ABC",
				"414243");

			$this->TestCase_RenderHex(
				u("ABC"),
				"414243");

			$this->TestCase_RenderHex(
				u("ÄÖÜ"),
				"c384c396c39c");

			$this->TestCase_RenderHex(
				u("123"),
				"313233");
		
		}
		

	}
	
	

		
