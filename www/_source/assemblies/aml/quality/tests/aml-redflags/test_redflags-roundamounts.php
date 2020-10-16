<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML_RaiseRedFlags_RoundAmounts');
		}
		  

		function TestCase_RaiseRedFlags_RoundAmounts(
			$arrayAccount,
			$arrayTypologyDefinition,
			$strDateBegin,
			$strDateEnd,
			$arrayExpectedResult)
		{ 
			$this->Trace('RaiseRedFlags_RoundAmounts');
	
			$this->Trace('Date begin : '.$strDateBegin);
			$this->Trace('Date end   : '.$strDateEnd);
			$this->Trace('Definition:');
			$this->Trace($arrayTypologyDefinition);
			$this->Trace('Account:');
			$this->Trace($arrayAccount);
			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = AML_RaiseRedFlags_RoundAmounts($arrayAccount,$arrayTypologyDefinition,$strDateBegin,$strDateEnd);
			
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


		function CallbackInit()
		{
			//$this->SetActive(false);	
			return parent::CallbackInit();
		}

		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);

			$arrayTypologyDefinition = array();
			$arrayTypologyDefinition['EUR']['1000000'] = 4;
			$arrayTypologyDefinition['EUR']['100000'] = 3;
			$arrayTypologyDefinition['EUR']['2000'] = 2;

			$arrayAccount = false;
			$arrayExpected = false;
			$this->TestCase_RaiseRedFlags_RoundAmounts($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_RaiseRedFlags_RoundAmounts($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
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
			$this->TestCase_RaiseRedFlags_RoundAmounts($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => -8000,
						'ID' => 'B',
						'TYPE' => 'WIRE',
						'COUNTRY' => 'USA'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'ROUNDAMOUNTS',
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
							'COUNTRY' => 'USA'
							)
						)
					)
				);
			$this->TestCase_RaiseRedFlags_RoundAmounts($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'COUNTRY' => 'DEU',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 2500,
						'ID' => 'B',
						'TYPE' => 'WIRE',
						'COUNTRY' => 'USA'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => 2501,
						'ID' => 'B',
						'TYPE' => 'WIRE',
						'COUNTRY' => 'USA'
						),
					array(
						'DATE' => '2019-01-03',
						'AMOUNT' => 9999,
						'ID' => 'B',
						'TYPE' => 'WIRE',
						'COUNTRY' => 'USA'
						)
					)				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'ROUNDAMOUNTS',
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01',
					'AMOUNT' => 2500.0,
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 2500,
							'ID' => 'B',
							'TYPE' => 'WIRE',
							'COUNTRY' => 'USA'
							)
						)
					)
				);
			$this->TestCase_RaiseRedFlags_RoundAmounts($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

		}
	}
	
	

		
