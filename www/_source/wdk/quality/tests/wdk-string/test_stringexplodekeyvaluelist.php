<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("StringExplodeKeyValueList");
		}
		

		function TestCase_StringExplodeKeyValueList(
			$strParam,
			$strLineSeparator,
			$strKeySeparator,
			$bTrim,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_StringExplodeKeyValueList");
	 
			$this->Trace("strParam = \"$strParam\"");
			$this->Trace("strLineSeparator = \"$strLineSeparator\"");
			$this->Trace("strKeySeparator = \"$strKeySeparator\"");
			$this->Trace("bTrim = ".RenderValue($bTrim));

			$this->Trace("Expected:");
			$this->Trace($arrayExpectedResult);


			$arrayResult = StringExplodeKeyValueList($strParam,$strLineSeparator,$strKeySeparator,$bTrim);
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


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true); 

			$strInput = "";
			$arrayExpectedResult = array();
			$this->TestCase_StringExplodeKeyValueList($strInput,"\n","=",true,$arrayExpectedResult);
			
			$strInput =
				"a=1\n".
				"b=2\n".
				"c=3";
			$arrayExpectedResult = array(
				"a" => "1",
				"b" => "2",
				"c" => "3");
			$this->TestCase_StringExplodeKeyValueList($strInput,"\n","=",false,$arrayExpectedResult);

			$strInput =
				"a=1\n".
				"b=2 \n".
				"c=3  ";
			$arrayExpectedResult = array(
				"a" => "1",
				"b" => "2 ",
				"c" => "3  ");
			$this->TestCase_StringExplodeKeyValueList($strInput,"\n","=",false,$arrayExpectedResult);

			$strInput =
				"a=1\n".
				"b=2 \n".
				"c=3  ";
			$arrayExpectedResult = array(
				"a" => "1",
				"b" => "2",
				"c" => "3");
			$this->TestCase_StringExplodeKeyValueList($strInput,"\n","=",true,$arrayExpectedResult);


		}
		
		
	}
	
	


		
