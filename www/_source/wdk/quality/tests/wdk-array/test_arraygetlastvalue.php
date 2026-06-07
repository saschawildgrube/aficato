<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArrayGetLastValue');
		}
		

		function TestCase_ArrayGetLastValue(
			$arrayInput,
			$vExpectedResult)
		{ 
			$this->Trace('TestCase_ArrayGetLastValue');
			$this->Trace('Input: '.RenderValue($arrayInput));
	
			$vResult = ArrayGetLastValue($arrayInput);
	
			$this->Trace('Result: '.RenderValue($vResult));
	
			if ($vResult == $vExpectedResult)
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
			
			$this->TestCase_ArrayGetLastValue(array(),false);
			$this->TestCase_ArrayGetLastValue(null,false);
			
			$arrayInput = array(
				'key1' => 'value1',
				'key2' => 'value2',
				'key3' => array(
					'key31' => 'value31',
					'key32' => 'value32',
					'key33' => 'value33'
					)
				);
					
			$this->TestCase_ArrayGetLastValue($arrayInput,
				array(
					'key31' => 'value31',
					'key32' => 'value32',
					'key33' => 'value33'
					)
				);
					

			$arrayInput = array(
				0 => 'value1',
				1 => 'value2',
				2 => array(
					'key31' => 'value31',
					'key32' => 'value32',
					'key33' => 'value33'
					)
				);
			$this->TestCase_ArrayGetLastValue($arrayInput,
				array(
					'key31' => 'value31',
					'key32' => 'value32',
					'key33' => 'value33'
					)
				);

			$arrayInput = array(
				10 => 'value1',
				20 => 'value2'
				);
			$this->TestCase_ArrayGetLastValue($arrayInput,'value2');			
	
		}
		
		
	}
	
	

		
