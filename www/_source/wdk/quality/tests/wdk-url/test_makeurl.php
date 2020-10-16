<?php

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("MakeURL");
		}
		

		function TestCase_MakeURL(
			$strRootURL,
			$arrayParams,
			$bEncoded,
			$strExpectedResult)
		{ 
			$this->Trace('TestCase_MakeURL');
			$this->Trace('Root URL: '.$strRootURL);
			$this->Trace('Parameter:');
			$this->Trace($arrayParams);
			$this->Trace('bEncoded: '.RenderBool($bEncoded));
			$this->Trace('Expected Result: '.$strExpectedResult);

			$strResult = MakeURL($strRootURL,$arrayParams,$bEncoded);
			
		$this->Trace('Result: '.$strResult);			
			
			if ($strResult != $strExpectedResult)
			{
				$this->Trace('Testcase FAILED!');	
				$this->Trace('');
				$this->Trace('');
				$this->SetResult(false);	
				return;
			}
	
			$this->Trace('Testcase PASSED!');
			$this->Trace('');
			$this->Trace('');
			
		}



		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);
			
			$this->TestCase_MakeURL(
				'www.example.com',
				array(),
				false,
				'www.example.com');

			$this->TestCase_MakeURL(
				'www.example.com',
				array(
					'param1' => 'value1',
					'param2' => 'value2'
					),
				false,
				'www.example.com?param1=value1&param2=value2');

			$this->TestCase_MakeURL(
				'www.example.com?param0=value0',
				array(
					'param1' => 'value1',
					'param2' => 'value2'
					),
				false,
				'www.example.com?param0=value0&param1=value1&param2=value2');
	
		$this->TestCase_MakeURL(
				'www.example.com?param0=value0',
				array(
					'param1' => 'value1',
					'param2' => 'value2'
					),
				true,
				'www.example.com?param0=value0&amp;param1=value1&amp;param2=value2');
	
				

		}
	}
	
	

		
