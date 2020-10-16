<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArrayRemoveKey');
		}
		  

		function TestCase_ArrayRemoveKey(
			$arraySource,
			$key,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ArrayRemoveKey');
	
			$this->Trace('Source:');
			$this->Trace($arraySource);
			$this->Trace('Key:');
			$this->Trace($key);

			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayRemoveKey($arraySource,$key);
			
			$this->Trace('Result:');
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
			
			$arraySource = array(
				'id' 			=> 123,
				'name' 		=> 'john doe',
				'address' => '33 backstreet'
				);
			$key = 'address';
			$arrayExpected = array(
				'id' 			=> 123,
				'name' 		=> 'john doe'
				);
			$this->TestCase_ArrayRemoveKey($arraySource,$key,$arrayExpected);

			$arraySource = array(
				1	=> 123,
				2	=> 'john doe',
				3	=> '33 backstreet'
				);
			$key = 3;
			$arrayExpected = array(
				1 => 123,
				2 => 'john doe'
				);
			$this->TestCase_ArrayRemoveKey($arraySource,$key,$arrayExpected);


		}
	}
	
	

		
