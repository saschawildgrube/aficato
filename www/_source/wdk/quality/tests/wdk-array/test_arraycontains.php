<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArrayContains');
		}
		  

		function TestCase_ArrayContains(
			$array1,
			$array2,
			$vExpectedResult)
		{ 
			$this->Trace('TestCase_ArrayContains');
	
			$this->Trace('Array1');
			$this->Trace($array1);
			$this->Trace('Array2');
			$this->Trace($array2);

			$this->Trace('Expected'); 
			$this->Trace($vExpectedResult);

			$vResult = ArrayContains($array1,$array2);
			
			$this->Trace('Result');
			$this->Trace($vResult);   

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



			$this->Trace('Parameter corner cases: an empty array is contained in an empty array by definition');

			$array1 = array();
			$array2 = array();
			$vExpected = true;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);

			$array1 = array('a');
			$array2 = array();
			$vExpected = true;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);

			$array1 = array();
			$array2 = array('a');
			$vExpected = false;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);

			$array1 = null;
			$array2 = null;
			$vExpected = true;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);

			
			
			$this->Trace('Indexed arrays');

			// One array is exactly part of another
			$array1 = array(
				1 => 'a',
				2 => 'b',
				3 => 'c'
				);
			$array2 = array( 
				1 => 'a',
				2 => 'b'
				);
			$vExpected = true;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);


			// order in the second array does not matter
			$array1 = array(
				1 => 'a',
				2 => 'b',
				3 => 'c'
				);
			$array2 = array( 
				1 => 'b',
				2 => 'a'
				);
			$vExpected = true;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);


			// The second array is NOT part of the first
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
			$vExpected = false;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);




			$array1 = array();
			$array1[] = 'a';
			$array1[] = 'b';
			$array1[] = 'c';
			$array2 = array();
			$array2[] = 'x';
			$array2[] = 'y';
			$array2[] = 'z';
			$vExpected = false;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);


			$array1 = array();
			$array1[] = 'a';
			$array1[] = 'b';
			$array1[] = 'c';
			$array2 = array();
			$array2[] = 'a';
			$array2[] = 'c';
			$vExpected = true;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);

			$array1 = array();
			$array1[] = 'a';
			$array1[] = 'b';
			$array1[] = array('c' => 'd');
			$array2 = array();
			$array2[] = 'b';
			$array2[] = array('c' => 'd');
			$vExpected = true;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);



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
			$vExpected = false;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);


			$this->Trace('Associative arrays');
			$array1 = array(
				'alpha' => 'a',
				'beta' => 'b',
				'gamma' => array(
					'gamma.alpha' => 'ca',
					'gamma.beta' => 'cb')
				);
			$array2 = array(
				'alpha' => 'x',
				'beta' => 'y',
				'gamma' => array(
					'gamma.beta' => 'cb2',
					'gamma.delta' => 'cd2'
					),
				'delta' => 'd'
				);
			$vExpected = false;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);



			$array1 = array(
				'alpha' => 'a',
				'beta' => 'b',
				'gamma' => 'c'
				);
			$array2 = array(
				'alpha' => 'a',
				'beta' => 'b',
				);
			$vExpected = true;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);

			$this->Trace('Associative arrays');
			$array1 = array(
				'alpha' => 'a',
				'beta' => 'b',
				'gamma' => array(
					'gamma.alpha' => 'ca',
					'gamma.beta' => 'cb')
				);
			$array2 = array(
				'alpha' => 'a',
				'gamma' => array(
					'gamma.beta' => 'cb')
				);
			$vExpected = true;
			$this->TestCase_ArrayContains($array1,$array2,$vExpected);



		}
	}
	
	

		
