<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML_IsRoundAmount');
		}
		  

		function TestCase_IsRoundAmount(
			$fAmount,
			$arrayDefinition,
			$bExpectedResult)
		{ 
			$this->Trace('IsRoundAmount');
	
			$this->Trace('Amount: '.RenderValue($fAmount));
			$this->Trace('Definition:');
			$this->Trace($arrayDefinition);
			$this->Trace('Expected: '.RenderBool($bExpectedResult)); 


			$bResult = AML_IsRoundAmount($fAmount,$arrayDefinition);
			
			$this->Trace('Result: '.RenderBool($bResult));

			if ($bResult === $bExpectedResult)
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


		function OnInit()
		{
			//$this->SetActive(false);	
			return parent::OnInit();
		}

		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$arrayDefinition = array();
			$arrayDefinition['1000000'] = 4;
			$arrayDefinition['100000'] = 3;
			$arrayDefinition['10000'] = 2;
			$arrayDefinition['2000'] = 2;

			$this->TestCase_IsRoundAmount(0,$arrayDefinition,false);
			$this->TestCase_IsRoundAmount(1,$arrayDefinition,false);
			$this->TestCase_IsRoundAmount(-1,$arrayDefinition,false);
			$this->TestCase_IsRoundAmount(1000,$arrayDefinition,false);
			$this->TestCase_IsRoundAmount(-1000,$arrayDefinition,false);
			$this->TestCase_IsRoundAmount(1999,$arrayDefinition,false);
			$this->TestCase_IsRoundAmount(-1999,$arrayDefinition,false);
			$this->TestCase_IsRoundAmount(100001,$arrayDefinition,false);
			$this->TestCase_IsRoundAmount(-100001,$arrayDefinition,false);

			$this->TestCase_IsRoundAmount(2000,$arrayDefinition,true);
			$this->TestCase_IsRoundAmount(2100,$arrayDefinition,true);
			$this->TestCase_IsRoundAmount(7000,$arrayDefinition,true);
			$this->TestCase_IsRoundAmount(-7000,$arrayDefinition,true);
			$this->TestCase_IsRoundAmount(10000,$arrayDefinition,true);
			$this->TestCase_IsRoundAmount(-10000,$arrayDefinition,true);
			$this->TestCase_IsRoundAmount(1000000,$arrayDefinition,true);
			$this->TestCase_IsRoundAmount(-1000000,$arrayDefinition,true);


		}
	}
	
	

		
