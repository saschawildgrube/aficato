<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayStrictCompare");
		}
		

		function TestCase_ArrayStrictCompare(
			$array1,
			$array2,
			$bExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayStrictCompare");
			$this->Trace("array1:");
			$this->Trace($array1);
			$this->Trace("array2:");
			$this->Trace($array2);

			$this->Trace("bExpectedResult = ".RenderBool($bExpectedResult));
	
			$bResult = ArrayStrictCompare($array1,$array2);
	
			$this->Trace("bResult = ".RenderBool($bResult));
	
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
			$this->Trace("");
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			$this->TestCase_ArrayStrictCompare("bogus","bogus",false);
			$this->TestCase_ArrayStrictCompare(false,false,false);
			$this->TestCase_ArrayStrictCompare(array(),array(),true);
			
			$array1 = array(
				1 => 1,
				2 => 2,
				3 => 3
				);
			$array2 = array(
				1 => 1,
				2 => 2,
				3 => 3
				);
			$this->TestCase_ArrayStrictCompare($array1,$array2,true);

			$array2 = array(
				0 => 1,
				1 => 2,
				2 => 3
				);
			$this->TestCase_ArrayStrictCompare($array1,$array2,false);				
			
			$array1 = array(
				"alpha" => 1,
				"beta" => 2,
				"gamma" => 3
				);
			$array2 = array(
				"alpha" => 1,
				"beta" => 2,
				"gamma" => 3
				);
			$this->TestCase_ArrayStrictCompare($array1,$array2,true);

			$array2 = array(
				"alpha" => 1,
				"gamma" => 3,
				"beta" => 2
				);
			$this->TestCase_ArrayStrictCompare($array1,$array2,false);				



		}
		
		
	}
	
	

		
