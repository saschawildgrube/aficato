<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("GetIntegerValue");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);	
			return true;
		}
	
	
		function TestCase_GetIntegerValue($value,$nExpectedResult)
		{
			$this->Trace("TestCase_GetIntegerValue");
			$this->Trace("value = $value");
			$this->Trace("Expected Result: $nExpectedResult");
			$nResult = GetIntegerValue($value);
			$strValue = (is_string($value))?("\"$value\""):($value);
			$this->Trace("GetIntegerValue($strValue) = $nResult");
			if ($nResult == $nExpectedResult)
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
			
			$this->TestCase_GetIntegerValue(0,0);
			$this->TestCase_GetIntegerValue(1,1);
			$this->TestCase_GetIntegerValue(-1,-1);
			$this->TestCase_GetIntegerValue(1.1,1);
			$this->TestCase_GetIntegerValue(1.9,2);  
			
			$this->TestCase_GetIntegerValue("",0);
			$this->TestCase_GetIntegerValue("1",1);
			$this->TestCase_GetIntegerValue("1.1",1);
			$this->TestCase_GetIntegerValue("100.1",100);
			$this->TestCase_GetIntegerValue("-1",-1);
			
			$this->TestCase_GetIntegerValue("2T",2000); 
			$this->TestCase_GetIntegerValue("2M",2000000);
			$this->TestCase_GetIntegerValue("2BN",2000000000);
			
			$this->TestCase_GetIntegerValue("2KB",2*1024);
			$this->TestCase_GetIntegerValue("2MB",2*1024*1024);			
			$this->TestCase_GetIntegerValue("4TB",4*1024*1024*1024);			

		}
		

	}
	
	

		
