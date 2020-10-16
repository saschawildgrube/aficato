<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("SLICE AND GLUE");
		}
		

		function TestCase_SliceAndGlue(
			$strParam,
			$nSlices,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_SliceAndGlue");
	 
			$this->Trace("strParam          = \"$strParam\"");

			$arraySlices = StringSlice($strParam,$nSlices);
			
			$this->Trace($arraySlices);
			
			if ($arraySlices != $arrayExpectedResult)
			{
				$this->Trace("Expected result does not match.");	
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
				$this->Trace("");
				$this->Trace("");
				return;
			}
			
			$strResult = StringGlue($arraySlices);
			
			$this->Trace("strResult = \"$strResult\"");
	
			if ($strResult != $strParam)
			{
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
			
			$this->TestCase_SliceAndGlue("a1b2c3d4e5",2,array("abcde","12345"));
			$this->TestCase_SliceAndGlue("abc",3,array("a","b","c"));
			$this->TestCase_SliceAndGlue("123123123123123123",3,array("111111","222222","333333"));
			$this->TestCase_SliceAndGlue(u("ÄÖÜÄÖÜ"),3,array(u("ÄÄ"),u("ÖÖ"),u("ÜÜ")));
		}
		
		
	}
	
	


		
