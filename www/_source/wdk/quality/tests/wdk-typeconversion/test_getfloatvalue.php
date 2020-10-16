<?php
	
	require_once(GetWDKDir().'wdk_typeconversion.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK GetFloatValue');
		}
		  

		

		function TestCase_GetFloatValue(
			$value,
			$fExpected)
		{ 
			$this->Trace('TestCase_GetFloatValue');
			$this->Trace('value     = '.$value);
			$this->Trace('fExpected = '.$fExpected);
		
			$fResult = GetFloatValue(
				$value);
			
			$this->Trace('fResult   = '.$fResult);
			
			if ($fResult == $fExpected)
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
			
			
			$this->TestCase_GetFloatValue(
				floatval(1.5),
				1.5);

			$this->TestCase_GetFloatValue(
				'1,5',
				1.5);

			$this->TestCase_GetFloatValue(
				'bogus',
				0);

			$this->TestCase_GetFloatValue(
				'',
				0);

			$this->TestCase_GetFloatValue(
				0,
				0);

			$this->TestCase_GetFloatValue(
				'1,000.05',
				1000.05);

			$this->TestCase_GetFloatValue(
				"1'000,05",
				1000.05);

			$this->TestCase_GetFloatValue(
				'1000,05',
				1000.05);

			$this->TestCase_GetFloatValue(
				'1.000,05',
				1000.05);



			$this->TestCase_GetFloatValue(
				floatval(-1.5),
				-1.5);

			$this->TestCase_GetFloatValue(
				'-1,5',
				-1.5);

			$this->TestCase_GetFloatValue(
				'-',
				0);

			$this->TestCase_GetFloatValue(
				-0,
				0);

			$this->TestCase_GetFloatValue(
				'-1,000.05',
				-1000.05);

			$this->TestCase_GetFloatValue(
				"-1'000,05",
				-1000.05);

			$this->TestCase_GetFloatValue(
				'-1000,05',
				-1000.05);

			$this->TestCase_GetFloatValue(
				'-1.000,05',
				-1000.05);




		}
	}
	
	

		
