<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML_RaiseRedFlags_InternationalWires');
		}
		  

		function TestCase_RaiseRedFlags_InternationalWires(
			$arrayAccount,
			$arrayTypologyDefinition,
			$strDateBegin,
			$strDateEnd,
			$arrayExpectedResult)
		{ 
			$this->Trace('RaiseRedFlags_InternationalWires');
	
			$this->Trace('Date begin : '.$strDateBegin);
			$this->Trace('Date end   : '.$strDateEnd);
			$this->Trace('Definition:');
			$this->Trace($arrayTypologyDefinition);
			$this->Trace('Account:');
			$this->Trace($arrayAccount);
			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = AML_RaiseRedFlags_InternationalWires($arrayAccount,$arrayTypologyDefinition,$strDateBegin,$strDateEnd);
			
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
			$arrayTypologyDefinition['THRESHOLD_AMOUNT']['EUR'] = 8000;

			$arrayAccount = false;
			$arrayExpected = false;
			$this->TestCase_RaiseRedFlags_InternationalWires($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_RaiseRedFlags_InternationalWires($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

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
			$this->TestCase_RaiseRedFlags_InternationalWires($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


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
						'COUNTRY' => 'USA'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'INTERNATIONALWIRES',
					'THRESHOLD_AMOUNT' => 8000.0,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01',
					'AMOUNT' => 8000.0,
					'DIRECTION' => 'WITHDRAWALS',
					'COUNTRY' => 'USA',
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
			$this->TestCase_RaiseRedFlags_InternationalWires($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'COUNTRY' => 'DEU',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 8000,
						'ID' => 'B',
						'TYPE' => 'WIRE',
						'COUNTRY' => 'USA'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'INTERNATIONALWIRES',
					'THRESHOLD_AMOUNT' => 8000.0,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01',
					'AMOUNT' => 8000.0,
					'DIRECTION' => 'DEPOSITS',
					'COUNTRY' => 'USA',
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 8000,
							'ID' => 'B',
							'TYPE' => 'WIRE',
							'COUNTRY' => 'USA'
							)
						)
					)
				);
			$this->TestCase_RaiseRedFlags_InternationalWires($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);



		}
	}
	
	

		
