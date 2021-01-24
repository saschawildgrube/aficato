<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArrayValues');
		}
		  

		function TestCase_ArrayValues(
			$arrayInput,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ArrayValues');
	
			$this->Trace('Input');
			$this->Trace($arrayInput);
			$this->Trace('Expected'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayValues($arrayInput);
			
			$this->Trace('Result');
			$this->Trace($arrayResult);

			if ($arrayResult == $arrayExpectedResult)
			{
				$this->Trace('Testcase PASSED!');
			}
			else
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);	
			}

			$this->Trace('');
			$this->Trace('');
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$arrayInput = array();
			$arrayExpected = array();
			$this->TestCase_ArrayValues($arrayInput,$arrayExpected);

			$arrayInput = null;
			$arrayExpected = array();
			$this->TestCase_ArrayValues($arrayInput,$arrayExpected);

			
			$arrayInput = array(
				'Alpha' => 'a',
				'Beta' => 'b',
				'Gamma' => 'c'
				);
			$arrayExpected = array(
				'a',
				'b',
				'c'
				);
			$this->TestCase_ArrayValues($arrayInput,$arrayExpected);

		}
	}
	
	

		
