<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArrayMap');
		}
		  

		function TestCase_ArrayMap(
			$array,
			$CallbackArrayMap,
			$arrayParams,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_ArrayMap');
	
			$this->Trace('Array:');
			$this->Trace($array);
			$this->Trace('CallbackArrayMap: ');
			$this->Trace($CallbackArrayMap);
			$this->Trace('arrayParams: ');
			$this->Trace($arrayParams);

			$this->Trace('Expected'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = ArrayMap($array,$CallbackArrayMap,$arrayParams);
			
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


			function CallbackArrayMapTest($key,$value,$arrayParams)
			{
				if ($key === 'haystack')
				{
					return ArrayGetValue($arrayParams,'result');	
				}	
				return $value;
			}


			$this->Trace('Empty array');
			$array = array();
			$CallbackArrayMap = 'CallbackArrayMapTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array();
			$this->TestCase_ArrayMap($array,$CallbackArrayMap,$arrayParams,$arrayExpected);

			$this->Trace('Null value');
			$array = null;
			$CallbackArrayMap = 'CallbackArrayMapTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array();
			$this->TestCase_ArrayMap($array,$CallbackArrayMap,$arrayParams,$arrayExpected);


			$this->Trace('Associative array stays the same');
			$array = array(
				'A' => 'Alpha'
				);
			$CallbackArrayMap = 'CallbackArrayMapTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array(
				'A' => 'Alpha'
				);
			$this->TestCase_ArrayMap($array,$CallbackArrayMap,$arrayParams,$arrayExpected);

			$this->Trace('Indexed array stays the same');
			$array = array(
				'Alpha'
				);
			$CallbackArrayMap = 'CallbackArrayMapTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array(
				'Alpha'
				);
			$this->TestCase_ArrayMap($array,$CallbackArrayMap,$arrayParams,$arrayExpected);

			$this->Trace('Associative array is altered');
			$array = array(
				'A' => 'Alpha',
				'haystack' => 'Searching for the needle!'
				);
			$CallbackArrayMap = 'CallbackArrayMapTest';
			$arrayParams = array( 'result' => 'needle' );
			$arrayExpected = array(
				'A' => 'Alpha',
				'haystack' => 'needle'
				);
			$this->TestCase_ArrayMap($array,$CallbackArrayMap,$arrayParams,$arrayExpected);

		}
	}
	
	

		
