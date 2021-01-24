<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArrayRemoveKeyRecursive');
		}
		  

		function TestCase_ArrayRemoveKeyRecursive(
			$arraySource,
			$key,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ArrayRemoveKeyRecursive');
	
			$this->Trace('Source:');
			$this->Trace($arraySource);
			$this->Trace('Key:');
			$this->Trace($key);

			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayRemoveKeyRecursive($arraySource,$key);
			
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


		function OnTest()
		{
			parent::OnTest();
					
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
			$this->TestCase_ArrayRemoveKeyRecursive($arraySource,$key,$arrayExpected);
			
			$arraySource = array(
				'name' 		=> 'John',
				'address' => '33 Backstreet',
				'siblings' => array(
					array(
						'name' => 'Lisa',
						'address' => '1 Baker street'
						),
					array(
						'name' => 'Jane',
						'address' => '1 Google Drive'
						)
					)
				);
			$key = 'address';
			$arrayExpected = array(
				'name' 		=> 'John',
				'siblings' => array(
					array(
						'name' => 'Lisa'
						),
					array(
						'name' => 'Jane'
						)
					)
				);
			$this->TestCase_ArrayRemoveKeyRecursive($arraySource,$key,$arrayExpected);

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
			$this->TestCase_ArrayRemoveKeyRecursive($arraySource,$key,$arrayExpected);


		}
	}
	
	

		
