<?php

	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test IsArray');
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsArray(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace('TestCase_IsArray');
		
			$bValue = IsArray($value);
		
			$this->Trace('Input: '.RenderValue($value));
			$this->Trace('IsArray() = '.RenderBool($bValue));
		
			if ($bValue == $bExpectedValue)
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
			
			$this->TestCase_IsArray(null,false);
			$this->TestCase_IsArray(false,false);
			$this->TestCase_IsArray(true,false);
			$this->TestCase_IsArray('',false);
			$this->TestCase_IsArray('Alpha',false);
			$this->TestCase_IsArray(42,false);
			
			$this->TestCase_IsArray(array(),true);
			$this->TestCase_IsArray(array(1),true);
			$this->TestCase_IsArray(array('Alpha'),true);
			$this->TestCase_IsArray(array('Alpha','Beta'),true);
			$this->TestCase_IsArray(array('A' => 'Alpha', 'B' => 'Beta'),true);
			
		}
		
		
	}
	
	

		
