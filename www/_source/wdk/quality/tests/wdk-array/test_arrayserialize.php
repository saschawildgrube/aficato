<?php
	
	require_once(GetWDKDir().'wdk_array.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('WDK ArraySerialize and ArrayUnserialize');
		}
		  

		function TestCase_ArraySerialize_ArrayUnserialize(
			$arrayInput,
			$vExpectedResult)
		{ 
			$this->Trace('');
			$this->Trace('');
			$this->Trace('TestCase_ArraySerialize_ArrayUnserialize');
	
			$this->Trace('arrayInput');
			$this->Trace($arrayInput);
			$this->Trace('Expected:'); 
			$this->Trace(RenderValue($vExpectedResult));

			$vResult = ArraySerialize($arrayInput);

			$this->Trace('ArraySerialize returned:');
			$this->Trace(RenderValue($vResult));
			//$this->Trace('StringLength = '.StringLength($strResult).'');
			
			if ($vResult != $vExpectedResult)
			{
				$this->Trace('Testcase FAILED!');	
				$this->SetResult(false);
				return;
			}
			
			$arrayResult = ArrayUnserialize($vResult);
			$this->Trace('ArrayUnserialize returned:');
			$this->Trace(RenderValue($arrayResult));
			if (ArrayStrictCompare($arrayResult,$arrayInput) == false)
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
			$this->TestCase_ArraySerialize_ArrayUnserialize(
				$arrayInput,
				'a:3:{i:1;s:1:"a";i:2;s:1:"b";i:3;s:1:"c";}');
				
			$arrayInput = array(
				array('Keys','Values'),
				array(1,'a'),
				array(2,'b'),
				array(3,'c')
				);
			$this->TestCase_ArraySerialize_ArrayUnserialize(
				$arrayInput,
				'a:4:{i:0;a:2:{i:0;s:4:"Keys";i:1;s:6:"Values";}i:1;a:2:{i:0;i:1;i:1;s:1:"a";}i:2;a:2:{i:0;i:2;i:1;s:1:"b";}i:3;a:2:{i:0;i:3;i:1;s:1:"c";}}');


			$arrayInput = array(
				10 => 'a',
				20 => 'b',
				30 => 'c'
				);
			$this->TestCase_ArraySerialize_ArrayUnserialize(
				$arrayInput,
				'a:3:{i:10;s:1:"a";i:20;s:1:"b";i:30;s:1:"c";}');
				
			$arrayInput = array(
				array('Keys','Values'),
				array(10,'a'),
				array(20,'b'),
				array(30,'c')
				);
			$this->TestCase_ArraySerialize_ArrayUnserialize(
				$arrayInput,
				'a:4:{i:0;a:2:{i:0;s:4:"Keys";i:1;s:6:"Values";}i:1;a:2:{i:0;i:10;i:1;s:1:"a";}i:2;a:2:{i:0;i:20;i:1;s:1:"b";}i:3;a:2:{i:0;i:30;i:1;s:1:"c";}}');



		}
	}
	
	

		
