<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArrayUnique');
		}
		  

		function TestCase_ArrayUnique(
			$arrayInput,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ArrayUnique');
	
			$this->Trace('Input');
			$this->Trace($arrayInput);
			$this->Trace('Expected'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayUnique($arrayInput);
			
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
			
			$this->Trace('Index Array');
			$arrayInput = array(
				1 => 'z',
				2 => 'x',
				3 => 'a',
				4 => 'x'
				);
			$arrayExpected = array(
				1 => 'z',
				2 => 'x',
				3 => 'a'
				);
			$this->TestCase_ArrayUnique($arrayInput,$arrayExpected);


			$this->Trace('Index Array');
			$arrayInput = array();
			$arrayInput[] = 'z';
			$arrayInput[] = 'x';
			$arrayInput[] = 'a';
			$arrayInput[] = 'x';

			$arrayExpected = array();
			$arrayExpected[] = 'z';
			$arrayExpected[] = 'x';
			$arrayExpected[] = 'a'; 
			$this->TestCase_ArrayUnique($arrayInput,$arrayExpected);


			$this->Trace('Assoc Array');
			$arrayInput = array(
				'alpha' => 'z',
				'beta' => 'x',
				'gamma' => 'a',
				'delta' => 'x'
				);
			$arrayExpected = array(
				'alpha' => 'z',
				'beta' => 'x',
				'gamma' => 'a'
				); 
			$this->TestCase_ArrayUnique($arrayInput,$arrayExpected);

			$this->Trace('Array with structures');
			$arrayInput = array(
				'alpha' => array('alpha','beta'),
				'beta' => 'x',
				'gamma' => 'a',
				'delta' => 'x'
				);
			$arrayExpected = array(
				'alpha' => array('alpha','beta'),
				'beta' => 'x',
				'gamma' => 'a'
				);
			$this->TestCase_ArrayUnique($arrayInput,$arrayExpected);


		}
	}
	
	

		
