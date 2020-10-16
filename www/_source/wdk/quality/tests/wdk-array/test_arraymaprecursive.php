<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArrayMapRecursive');
		}
		  

		function TestCase_ArrayMapRecursive(
			$array,
			$CallbackArrayMapRecursive,
			$arrayParams,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ArrayMapRecursive');
	
			$this->Trace('Array:');
			$this->Trace($array);
			$this->Trace('CallbackArrayMapRecursive: ');
			$this->Trace($CallbackArrayMapRecursive);
			$this->Trace('arrayParams: ');
			$this->Trace($arrayParams);

			$this->Trace('Expected'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayMapRecursive($array,$CallbackArrayMapRecursive,$arrayParams);
			
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


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);


			function CallbackArrayMapRecursiveTest($key,$value,$arrayParams)
			{
				if ($key === 'haystack')
				{
					return ArrayGetValue($arrayParams,'result');	
				}	
				return $value;
			}


			$this->Trace('Empty array');
			$array = array();
			$CallbackArrayMapRecursive = 'CallbackArrayMapRecursiveTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array();
			$this->TestCase_ArrayMapRecursive($array,$CallbackArrayMapRecursive,$arrayParams,$arrayExpected);

			$this->Trace('Null value');
			$array = null;
			$CallbackArrayMapRecursive = 'CallbackArrayMapRecursiveTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array();
			$this->TestCase_ArrayMapRecursive($array,$CallbackArrayMapRecursive,$arrayParams,$arrayExpected);


			$this->Trace('Associative array stays the same');
			$array = array(
				'A' => 'Alpha'
				);
			$CallbackArrayMapRecursive = 'CallbackArrayMapRecursiveTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array(
				'A' => 'Alpha'
				);
			$this->TestCase_ArrayMapRecursive($array,$CallbackArrayMapRecursive,$arrayParams,$arrayExpected);

			$this->Trace('Indexed array stays the same');
			$array = array(
				'Alpha'
				);
			$CallbackArrayMapRecursive = 'CallbackArrayMapRecursiveTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array(
				'Alpha'
				);
			$this->TestCase_ArrayMapRecursive($array,$CallbackArrayMapRecursive,$arrayParams,$arrayExpected);

			$this->Trace('Associative array is altered');
			$array = array(
				'A' => 'Alpha',
				'haystack' => 'Searching for the needle!'
				);
			$CallbackArrayMapRecursive = 'CallbackArrayMapRecursiveTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array(
				'A' => 'Alpha',
				'haystack' => 'needle'
				);
			$this->TestCase_ArrayMapRecursive($array,$CallbackArrayMapRecursive,$arrayParams,$arrayExpected);


		$this->Trace('Associative array with nested array is altered');
			$array = array(
				'A' => 'Alpha',
				'haystack' => 'Searching for the needle!',
				'array1' => array(
					'haystack' => 'Searching for the needle!'
					)
				);
			$CallbackArrayMapRecursive = 'CallbackArrayMapRecursiveTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array(
				'A' => 'Alpha',
				'haystack' => 'needle',
				'array1' => array(
					'haystack' => 'needle'
					)
				);
			$this->TestCase_ArrayMapRecursive($array,$CallbackArrayMapRecursive,$arrayParams,$arrayExpected);



		}
	}
	
	

		
