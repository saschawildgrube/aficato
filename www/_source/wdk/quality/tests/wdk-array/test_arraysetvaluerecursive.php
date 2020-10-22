<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArraySetValueRecursive');
		}
		  

		function TestCase_ArraySetValueRecursive(
			$arrayInput,
			$value,
			$key,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ArraySetValueRecursive');
			$this->Trace('Value: '.$value);
			$this->Trace('Key  : '.$key);
			$this->Trace('Input: ');
			$this->Trace($arrayInput);
			$this->Trace('Expected: ');
			$this->Trace($arrayExpectedResult);
			
			$arrayResult = ArraySetValueRecursive($arrayInput,$value,$key);
			$this->Trace('Result: ');
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


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);
			
			$arrayData = array(
				'key1' => 'value1',
				'key2' => 'value2',
				'key3' => array(
					'key1' => 'value31',
					'key2' => 'value32',
					'key3' => array(
						'key1' => 'value331',
						'key2' => 'value332',
						'key3' => array(
							'key1' => 'value3331',
							'key2' => 'value3332'
							)
						)
					)
				);

			$arrayExpected = array(
				'key1' => 'test1',
				'key2' => 'value2',
				'key3' => array(
					'key1' => 'test1',
					'key2' => 'value32',
					'key3' => array(
						'key1' => 'test1',
						'key2' => 'value332',
						'key3' => array(
							'key1' => 'test1',
							'key2' => 'value3332'
							)
						)
					)
				);
			$this->TestCase_ArraySetValueRecursive($arrayData,'test1','key1',$arrayExpected);


			$arrayExpected = array(
				'key1' => 'value1',
				'key2' => 'value2',
				'key3' => ''
				);
			$this->TestCase_ArraySetValueRecursive($arrayData,'','key3',$arrayExpected);


			$arrayExpected = array(
				'key1' => 'value1',
				'key2' => 'test6',
				'key3' => 'test6'
				);
			$this->TestCase_ArraySetValueRecursive($arrayData,'test6','/key[23]/',$arrayExpected);



		}
	}
	
	

		
