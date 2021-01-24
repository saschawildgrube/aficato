<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CompareString");
		}
		

		function TestCase_CompareString(
			$v1,
			$v2,
			$nExpected)
		{ 
			$this->Trace("TestCase_CompareString");
	 
			$this->Trace("String 1: ".RenderValue($v1));
			$this->Trace("String 2: ".RenderValue($v2));

			$this->Trace("Expected: $nExpected");

			$nResult = CompareString($v1,$v2);
			$this->Trace("CompareString returns: $nResult");
			
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$this->TestCase_CompareString("","",0);  
			$this->TestCase_CompareString("a","a",0);
			$this->TestCase_CompareString("a","b",-1);
			$this->TestCase_CompareString("b","a",1);
			$this->TestCase_CompareString("Hallo","hallo",0);
			$this->TestCase_CompareString("1.3","1.4",-1);
			$this->TestCase_CompareString("1.4","1.3",1);
			$this->TestCase_CompareString("5ubuntu1.3","5ubuntu1.4",-1);
			$this->TestCase_CompareString("5ubuntu1.4","5ubuntu1.3",1);
			$this->TestCase_CompareString('2019-01-01','2019-01-01',0);
			$this->TestCase_CompareString('2019-01-01','2019-01-02',-1);
			$this->TestCase_CompareString('2019-01-02','2019-01-01',1);

		}
		
		
	}
	
	


		
