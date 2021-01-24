<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML_RaiseRedFlags_HighRiskGeographies');
		}
		  

		function TestCase_RaiseRedFlags_HighRiskGeographies(
			$arrayAccount,
			$arrayTypologyDefinition,
			$strDateBegin,
			$strDateEnd,
			$arrayExpectedResult)
		{ 
			$this->Trace('RaiseRedFlags_HighRiskGeographies');
	
			$this->Trace('Date begin : '.$strDateBegin);
			$this->Trace('Date end   : '.$strDateEnd);
			$this->Trace('Definition:');
			$this->Trace($arrayTypologyDefinition);
			$this->Trace('Account:');
			$this->Trace($arrayAccount);
			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = AML_RaiseRedFlags_HighRiskGeographies($arrayAccount,$arrayTypologyDefinition,$strDateBegin,$strDateEnd);
			
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
			$arrayTypologyDefinition['THRESHOLD']['SCORE'] = 0.5;
			$arrayTypologyDefinition['COUNTRIES']['AFG']['SCORE'] = 1.0;

			$arrayAccount = false;
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_HighRiskGeographies($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_HighRiskGeographies($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'COUNTRY' => 'DEU',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 7999,
						'ID' => 'B',
						'TYPE' => 'WIRE',
						'COUNTRY' => 'USA'
						)
					)
				);
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_HighRiskGeographies($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'COUNTRY' => 'DEU',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => -8000,
						'ID' => 'B',
						'TYPE' => 'WIRE',
						'COUNTRY' => 'AFG'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'HIGHRISKGEOGRAPHIES',
					'COUNTRY' => 'AFG',
					'SCORE' => 1.0,
					'THRESHOLD_SCORE' => 0.5,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01',
					'AMOUNT' => 8000.0,
					'DIRECTION' => 'WITHDRAWALS',
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => -8000,
							'ID' => 'B',
							'TYPE' => 'WIRE',
							'COUNTRY' => 'AFG'
							)
						)
					)
				);
			$this->TestCase_RaiseRedFlags_HighRiskGeographies($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

		}
	}
	
	

		
