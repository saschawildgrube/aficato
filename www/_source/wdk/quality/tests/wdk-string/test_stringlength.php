<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringLength");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}
		
		function TestCase_StringLength($strString,$nExpectedResult)
		{
			$this->Trace("StringLength");
			$this->Trace("strString: ".RenderValue($strString));
			$this->Trace("Expected Result: ".RenderValue($nExpectedResult)."");
			$nResult = StringLength($strString);
			$this->Trace("StringLength returns: ".RenderValue($nResult));
			if ($nResult === $nExpectedResult)
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
			
			$this->TestCase_StringLength("",0);
			$this->TestCase_StringLength("a",1);
			$this->TestCase_StringLength(u("ä"),1);
			$this->TestCase_StringLength("123",3);
			$this->TestCase_StringLength(u("ÄÜÖ"),3);
			$this->TestCase_StringLength(u("ÄÜÖ"),3);
			
			$this->TestCase_StringLength(array(),0);
			$this->TestCase_StringLength(23.4,0);
			$this->TestCase_StringLength(2,0);
			$this->TestCase_StringLength(array("a"),0);
		}
		

	}
	
	

		
