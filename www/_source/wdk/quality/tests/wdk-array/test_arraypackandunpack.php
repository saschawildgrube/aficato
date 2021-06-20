<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArrayPack and ArrayUnpack');
		}
		  

		function TestCase_ArrayPack_ArrayUnpack(
			$arrayInput,
			$strExpectedResult)
		{ 
			$this->Trace('');
			$this->Trace('');
			$this->Trace('TestCase_ArrayPack_ArrayUnpack');
	
			$this->Trace('arrayInput');
			$this->Trace($arrayInput);
			$this->Trace('Expected:'); 
			$this->Trace($strExpectedResult);

			$strResult = ArrayPack($arrayInput);

			$this->Trace('ArrayPack returns:');
			$this->Trace($strResult);
			$this->Trace('StringLength = '.StringLength($strResult).'');
			
			if ($strResult != $strExpectedResult)
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);
				return;
			}
			
			$arrayResult = ArrayUnpack($strResult);
			$this->Trace('ArrayUnpack returns:');
			$this->Trace($arrayResult);

			if ($arrayResult != $arrayInput)
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);
				return;
			}
			

			$this->Trace('Testcase PASSED!');
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);
			
			$arrayInput = array(
				1 => 'a',
				2 => 'b',
				3 => 'c'
				);
			$this->TestCase_ArrayPack_ArrayUnpack(
				$arrayInput,
				'YTozOntpOjE7czoxOiJhIjtpOjI7czoxOiJiIjtpOjM7czoxOiJjIjt9');
				
			$arrayInput = array(
				array('Keys','Values'),
				array(1,'a'),
				array(2,'b'),
				array(3,'c')
				);
			$this->TestCase_ArrayPack_ArrayUnpack(
				$arrayInput,
				'YTo0OntpOjA7YToyOntpOjA7czo0OiJLZXlzIjtpOjE7czo2OiJWYWx1ZXMiO31pOjE7YToyOntpOjA7aToxO2k6MTtzOjE6ImEiO31pOjI7YToyOntpOjA7aToyO2k6MTtzOjE6ImIiO31pOjM7YToyOntpOjA7aTozO2k6MTtzOjE6ImMiO319');


			$arrayInput = array(
				10 => 'a',
				20 => 'b',
				30 => 'c'
				);
			$this->TestCase_ArrayPack_ArrayUnpack(
				$arrayInput,
				'YTozOntpOjEwO3M6MToiYSI7aToyMDtzOjE6ImIiO2k6MzA7czoxOiJjIjt9');
				
			$arrayInput = array(
				array('Keys','Values'),
				array(10,'a'),
				array(20,'b'),
				array(30,'c')
				);
			$this->TestCase_ArrayPack_ArrayUnpack(
				$arrayInput,
				'YTo0OntpOjA7YToyOntpOjA7czo0OiJLZXlzIjtpOjE7czo2OiJWYWx1ZXMiO31pOjE7YToyOntpOjA7aToxMDtpOjE7czoxOiJhIjt9aToyO2E6Mjp7aTowO2k6MjA7aToxO3M6MToiYiI7fWk6MzthOjI6e2k6MDtpOjMwO2k6MTtzOjE6ImMiO319');



		}
	}
	
	

		
