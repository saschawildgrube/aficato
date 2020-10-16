<?php
	
	require_once(GetWDKDir()."wdk_parameter.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test StringCheckCharset");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		}

		function TestCase_ArrayCheckCharSet($arrayInput,$strCharset,$bRecursive,$bKeys,$bValues,$bExpectedResult)
		{
			$this->Trace("TestCase_ArrayCheckCharSet");
			$this->Trace("strCharset = \"$strCharset\"");
			$this->Trace("bRecursive = ".RenderBool($bRecursive));
			$this->Trace("bKeys = ".RenderBool($bKeys));
			$this->Trace("bValues = ".RenderBool($bValues));
			$this->Trace($arrayInput);
			$this->Trace("Expected Result: ".RenderBool($bExpectedResult)."");
			$bResult = ArrayCheckCharSet($arrayInput,$strCharset,$bRecursive,$bKeys,$bValues);
			$this->Trace("ArrayCheckCharset returns: ".RenderBool($bResult));
			if ($bResult == $bExpectedResult)
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

		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$arrayInput = array(
				"a" => "123",
				"b" => "234",
				"c" => "345",
				"d" => array(
					"aa" => "1112233",
					"bb" => "2233",
					"cc" => "3333334444"));
			
			$this->TestCase_ArrayCheckCharSet($arrayInput,"12345",true,false,true,	true);
			$this->TestCase_ArrayCheckCharSet($arrayInput,"12345",false,false,true,	true);
			$this->TestCase_ArrayCheckCharSet($arrayInput,"12345",true,true,true,	false);
			$this->TestCase_ArrayCheckCharSet($arrayInput,"12345",false,true,true,	false);
			$this->TestCase_ArrayCheckCharSet($arrayInput,"12345",true,true,false,	false);
			$this->TestCase_ArrayCheckCharSet($arrayInput,"12345",false,true,false,	false);
			
			$this->TestCase_ArrayCheckCharSet(array(),"12345",true,false,true,	true);
			$this->TestCase_ArrayCheckCharSet(array(),"12345",false,false,true,	true);
			$this->TestCase_ArrayCheckCharSet(array(),"12345",true,true,true,		true);
			$this->TestCase_ArrayCheckCharSet(array(),"12345",false,true,true,	true);
			$this->TestCase_ArrayCheckCharSet(array(),"12345",true,true,false,	true);
			$this->TestCase_ArrayCheckCharSet(array(),"12345",false,true,false,	true);

			$this->TestCase_ArrayCheckCharSet("","12345",true,false,true,	false);
			$this->TestCase_ArrayCheckCharSet("","12345",false,false,true,	false);
			$this->TestCase_ArrayCheckCharSet("","12345",true,true,true,	false);
			$this->TestCase_ArrayCheckCharSet("","12345",false,true,true,	false);
			$this->TestCase_ArrayCheckCharSet("","12345",true,true,false,	false);
			$this->TestCase_ArrayCheckCharSet("","12345",false,true,false,	false);
			
			
		}
		
	}
	
	

		
