<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayMergeRecursive");
		}
		  

		function TestCase_ArrayMergeRecursive(
			$array1,
			$array2,
			callable $CallbackArrayMerge,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayMergeRecursive");
	
			$this->Trace("Array1");
			$this->Trace($array1);
			$this->Trace("Array2");
			$this->Trace($array2);

			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayMergeRecursive($array1,$array2,$CallbackArrayMerge);
			
			$this->Trace("Result");
			$this->Trace($arrayResult);

			if ($arrayResult == $arrayExpectedResult)
			{ 
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}

			$this->Trace('');
			$this->Trace('');
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$this->Trace('Parameter corner cases');
			$array1 = array();
			$array2 = array();
			$arrayExpected = array();
			$this->TestCase_ArrayMergeRecursive($array1,$array2,'CallbackArrayMergeAdd',$arrayExpected);

			$array1 = array('a');
			$array2 = array();
			$arrayExpected = array('a');
			$this->TestCase_ArrayMergeRecursive($array1,$array2,'CallbackArrayMergeAdd',$arrayExpected);

			$array1 = array();
			$array2 = array('a');
			$arrayExpected = array('a');
			$this->TestCase_ArrayMergeRecursive($array1,$array2,'CallbackArrayMergeAdd',$arrayExpected);

			$array1 = null;
			$array2 = null;
			$arrayExpected = array();
			$this->TestCase_ArrayMergeRecursive($array1,$array2,'CallbackArrayMergeAdd',$arrayExpected);

			
			
			$this->Trace('Indexed arrays');
			$array1 = array(
				1 => 'a',
				2 => 42,
				3 => 'c'
				);
			$array2 = array( 
				1 => 'x',
				2 => 'y',
				3 => 'z'
				);
			$arrayExpected = array(
				0 => 'a',
				1 => 42,
				2 => 'c',
				3 => 'x',
				4 => 'y',
				5 => 'z'
				);
			$this->TestCase_ArrayMergeRecursive($array1,$array2,'CallbackArrayMergeAdd',$arrayExpected); 



			$this->Trace('Associative arrays');
			$array1 = array(
				'alpha' => 1,
				'beta' => 10,
				'gamma' => 100
				);
			$array2 = array(
				'alpha' => 2,
				'beta' => 20,
				'delta' => 2000
				);
			$arrayExpected = array(
				'alpha' => 3,
				'beta' => 30,
				'gamma' => 100,
				'delta' => 2000,
				);
			$this->TestCase_ArrayMergeRecursive($array1,$array2,'CallbackArrayMergeAdd',$arrayExpected);


			$this->Trace('Associative arrays');
			$array1 = array(
				'alpha' => 1,
				'beta' => 10,
				'gamma' => array(
					'gamma.alpha' => 1,
					'gamma.beta' => 11)
				);
			$array2 = array(
				'alpha' => 2,
				'beta' => 20,
				'gamma' => array(
					'gamma.beta' => 22,
					'gamma.delta' => 2222
					),
				'delta' => 2000
				);
			$arrayExpected = array(
				'alpha' => 3,
				'beta' => 30,
				'gamma' => array(
					'gamma.alpha' => 1,
					'gamma.beta' => 33,
					'gamma.delta' => 2222),
				'delta' => 2000
				);
			$this->TestCase_ArrayMergeRecursive($array1,$array2,'CallbackArrayMergeAdd',$arrayExpected);

		}
	}
	
	

		
