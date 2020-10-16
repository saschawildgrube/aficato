<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("CompareStringIgnoreHyphen");
		}
		

		function TestCase_CompareStringIgnoreHyphen(
			$v1,
			$v2,
			$nExpected)
		{ 
			$this->Trace("TestCase_CompareStringIgnoreHyphen");
	 
			$this->Trace("String 1: ".RenderValue($v1));
			$this->Trace("String 2: ".RenderValue($v2));

			$this->Trace("Expected: $nExpected");

			$nResult = CompareStringIgnoreHyphen($v1,$v2);
			$this->Trace("CompareStringIgnoreHyphen returns: $nResult");
			
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

			$this->TestCase_CompareStringIgnoreHyphen("","",0);  
			$this->TestCase_CompareStringIgnoreHyphen("a","a",0);
			$this->TestCase_CompareStringIgnoreHyphen("a","b",-1);
			$this->TestCase_CompareStringIgnoreHyphen("b","a",1);
			$this->TestCase_CompareStringIgnoreHyphen("aa","a-a",-1);
			$this->TestCase_CompareStringIgnoreHyphen("a-a","aa",1);
			$this->TestCase_CompareStringIgnoreHyphen("-","-",0);
			$this->TestCase_CompareStringIgnoreHyphen("","-",-1);
			$this->TestCase_CompareStringIgnoreHyphen("-","",1);
			$this->TestCase_CompareStringIgnoreHyphen("hallowelt","hallo-welt",-1);
			$this->TestCase_CompareStringIgnoreHyphen("hallo-welt","hallowelt",1);
			$this->TestCase_CompareStringIgnoreHyphen("hallo-welt","hallo-welt",0);


		}
		
		
	}
	
	


		
