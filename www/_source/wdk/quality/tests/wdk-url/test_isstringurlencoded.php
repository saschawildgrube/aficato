<?php

	require_once(GetWDKDir().'wdk_url.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('Test IsStringURLEncoded');
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->SetResult(true);
			return true;
		}

		function TestCase_IsStringURLEncoded(
			$value,
			$bExpectedValue)
		{ 
			$this->Trace('TestCase_IsStringURLEncoded');
		
			$bValue = IsStringURLEncoded($value);
		
			$this->Trace('IsStringURLEncoded('.RenderValue($value).') = '.RenderBool($bValue));
		
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
			
			$this->Trace('');

			$this->TestCase_IsStringURLEncoded('Hello+World',true);
			$this->TestCase_IsStringURLEncoded('Hello%0D%0AWorld',true);
 			
			$this->TestCase_IsStringURLEncoded(false,false);
			$this->TestCase_IsStringURLEncoded('',false);
			$this->TestCase_IsStringURLEncoded('Hello World',false);
 			$this->TestCase_IsStringURLEncoded("Hello\r\nWorld",false);

		
			
		}
		
		
	}
	
	

		
