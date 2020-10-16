<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringExplode");
		}
		

		function TestCase_StringExplode(
			$strParam,
			$strSeparator,
			$bTrim,
			$nLimit,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_StringExplode");
	 
			$this->Trace("strParam = \"$strParam\"");
			$this->Trace("strSeparator = \"$strSeparator\"");
			$this->Trace("bTrim = ".RenderValue($bTrim));
			$this->Trace("nLimit = $nLimit");

			$this->Trace("Expected:");
			$this->Trace($arrayExpectedResult);


			$arrayResult = StringExplode($strParam,$strSeparator,$bTrim,$nLimit);
			$this->Trace("StringExplode returns:");
			$this->Trace($arrayResult);
			
			if ($arrayResult != $arrayExpectedResult)
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
			
			$arrayExpectedResult = array(
				"a",
				"b",
				"c");
			$this->TestCase_StringExplode("a,b,c",",",false,0,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"a ",
				"b  ",
				"c   ");
			$this->TestCase_StringExplode("a ,b  ,c   ",",",false,0,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"a",
				"b",
				"c");
			$this->TestCase_StringExplode("a ,b  ,c   ",",",true,0,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"a",
				"b  ,c");
			$this->TestCase_StringExplode("a ,b  ,c   ",",",true,2,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"a",
				"b",
				"c");
			$this->TestCase_StringExplode("a \nb  \nc\n   ","\n",true,0,$arrayExpectedResult);

			$arrayExpectedResult = array(
				"a ",
				"b  ",
				"c",
				"   ");
			$this->TestCase_StringExplode("a \nb  \nc\n   ","\n",false,0,$arrayExpectedResult);


		}
		
		
	}
	
	


		
