<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML_RaiseRedFlags_Inactivity');
		}
		  

		function TestCase_RaiseRedFlags_Inactivity(
			$arrayAccount,
			$arrayTypologyDefinition,
			$strDateBegin,
			$strDateEnd,
			$arrayExpectedResult)
		{ 
			$this->Trace('RaiseRedFlags_Inactivity');
	
			$this->Trace('Date begin : '.$strDateBegin);
			$this->Trace('Date end   : '.$strDateEnd);
			$this->Trace('Definition:');
			$this->Trace($arrayTypologyDefinition);
			$this->Trace('Account:');
			$this->Trace($arrayAccount);
			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = AML_RaiseRedFlags_Inactivity($arrayAccount,$arrayTypologyDefinition,$strDateBegin,$strDateEnd);
			
			$this->Trace('Result:');
			$this->Trace($arrayResult);

			if ($arrayResult === $arrayExpectedResult)
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

			$arrayTypologyDefinition = array();
			$arrayTypologyDefinition['THRESHOLD_DAYS'] = 21;

			$arrayAccount = false;
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_Inactivity($arrayAccount,$arrayTypologyDefinition,'2019-01-01','2019-01-31',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_Inactivity($arrayAccount,$arrayTypologyDefinition,'2019-01-01','2019-01-31',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_Inactivity($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => -100,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => 200,
						'ID' => 'B',
						'TYPE' => 'CASH'
						)
						
					)
				);
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_Inactivity($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);




			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => -100,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-02-01',
						'AMOUNT' => 200,
						'ID' => 'B',
						'TYPE' => 'CASH'
						)
						
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'INACTIVITY',
					'THRESHOLD_DAYS' => 21,
					'START_DATE' => '2019-01-02',
					'END_DATE' => '2019-01-31',
					'INACTIVITY_DAYS' => 30
					)
				);
			$this->TestCase_RaiseRedFlags_Inactivity($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


		}
	}
	
	

		
