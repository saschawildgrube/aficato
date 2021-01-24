<?php

	require_once(GetWDKDir()."wdk_xml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test HtmlConvertLineBreaks");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);	
			return true;
		}
		
		function TestCase_HtmlConvertLineBreaks(
			$strString,
			$strExpectedResult)
		{ 
			$this->Trace("TestCase: HtmlConvertLineBreaks");
			
			$this->Trace("Input: \"$strString\"");
			$this->Trace("Expected Result: \"$strExpectedResult\"");

			$strResult = HtmlConvertLineBreaks($strString);

			$this->Trace("Result: \"$strResult\"");
					
				
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
			$this->Trace("");
		}
		
		
		function OnTest()
		{
			parent::OnTest();

			$this->TestCase_HtmlConvertLineBreaks("A\nB\nC","A<br/>B<br/>C");
			$this->TestCase_HtmlConvertLineBreaks("A\r\nB\r\nC","A<br/>B<br/>C");
			$this->TestCase_HtmlConvertLineBreaks("A\rB\rC","A<br/>B<br/>C");
			$this->TestCase_HtmlConvertLineBreaks("A\rB\nC\r\nD","A<br/>B<br/>C<br/>D");
		}
	}
		
