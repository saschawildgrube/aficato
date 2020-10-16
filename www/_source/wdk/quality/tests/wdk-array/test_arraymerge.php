<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayMerge");
		}
		  

		function TestCase_ArrayMerge(
			$array1,
			$array2,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_ArrayMerge");
	
			$this->Trace("Array1");
			$this->Trace($array1);
			$this->Trace("Array2");
			$this->Trace($array2);

			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayMerge($array1,$array2);
			
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


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);

			$this->Trace('Parameter corner cases');
			$array1 = array();
			$array2 = array();
			$arrayExpected = array();
			$this->TestCase_ArrayMerge($array1,$array2,$arrayExpected);

			$array1 = array('a');
			$array2 = array();
			$arrayExpected = array('a');
			$this->TestCase_ArrayMerge($array1,$array2,$arrayExpected);

			$array1 = array();
			$array2 = array('a');
			$arrayExpected = array('a');
			$this->TestCase_ArrayMerge($array1,$array2,$arrayExpected);

			$array1 = null;
			$array2 = null;
			$arrayExpected = array();
			$this->TestCase_ArrayMerge($array1,$array2,$arrayExpected);

			
			
			$this->Trace('Indexed arrays');
			$array1 = array(
				1 => 'a',
				2 => 'b',
				3 => 'c'
				);
			$array2 = array( 
				1 => 'x',
				2 => 'y',
				3 => 'z'
				);
			$arrayExpected = array(
				0 => 'a',
				1 => 'b',
				2 => 'c',
				3 => 'x',
				4 => 'y',
				5 => 'z'
				);
			$this->TestCase_ArrayMerge($array1,$array2,$arrayExpected); 



			$array1 = array();
			$array1[] = 'a';
			$array1[] = 'b';
			$array1[] = 'c';

			$array2 = array();
			$array2[] = 'x';
			$array2[] = 'y';
			$array2[] = 'z';
			
			$arrayExpected = array();
			$arrayExpected[] = 'a';
			$arrayExpected[] = 'b';
			$arrayExpected[] = 'c';
			$arrayExpected[] = 'x';
			$arrayExpected[] = 'y';
			$arrayExpected[] = 'z';
			$this->TestCase_ArrayMerge($array1,$array2,$arrayExpected);


			$array1 = array();
			$array1[] = 'a';
			$array1[] = 'b';
			$array1[] = array('c' => 'd');

			$array2 = array();
			$array2[] = 'x';
			$array2[] = 'y';
			$array2[] = 'z';
			
			$arrayExpected = array();
			$arrayExpected[] = 'a';
			$arrayExpected[] = 'b';
			$arrayExpected[] = array('c' => 'd');
			$arrayExpected[] = 'x';
			$arrayExpected[] = 'y';
			$arrayExpected[] = 'z';
			$this->TestCase_ArrayMerge($array1,$array2,$arrayExpected);



			$this->Trace('Associative arrays');
			$array1 = array(
				'alpha' => 'a',
				'beta' => 'b',
				'gamma' => 'c'
				);
			$array2 = array(
				'alpha' => 'x',
				'beta' => 'y',
				'delta' => 'd'
				);
			$arrayExpected = array(
				'alpha' => 'x',
				'beta' => 'y',
				'gamma' => 'c',
				'delta' => 'd',
				);
			$this->TestCase_ArrayMerge($array1,$array2,$arrayExpected);




		}
	}
	
	

		
