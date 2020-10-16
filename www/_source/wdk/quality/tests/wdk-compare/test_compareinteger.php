<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CompareInteger");
		}
		

		function TestCase_CompareInteger(
			$v1,
			$v2,
			$nExpected)
		{ 
			$this->Trace("TestCase_CompareInteger");
	 
			$this->Trace("Integer 1: ".RenderValue($v1));
			$this->Trace("Integer 2: ".RenderValue($v2));

			$this->Trace("Expected: $nExpected");

			$nResult = CompareInteger($v1,$v2);
			$this->Trace("CompareInteger returns: $nResult");
			
			if ($nResult != $nExpected)
			{
				$this->Trace("Expected result does not match.");	
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
				$this->Trace("");
				$this->Trace("");
				return;
			}
			
			$this->Trace("Testcase PASSED!");
			$this->Trace("");
			$this->Trace("");
		}


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);

			$this->TestCase_CompareInteger(0,2,-1);  
			$this->TestCase_CompareInteger(1,1,0);
			$this->TestCase_CompareInteger(3,1,1);


		}
		
		
	}
	
	


		
