<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML_RaiseRedFlags_RapidMovement');
		}
		  

		function TestCase_RaiseRedFlags_RapidMovement(
			$arrayAccount,
			$arrayTypologyDefinition,
			$strDateBegin,
			$strDateEnd,
			$arrayExpectedResult)
		{ 
			$this->Trace('RaiseRedFlags_RapidMovement');
	
			$this->Trace('Date begin : '.$strDateBegin);
			$this->Trace('Date end   : '.$strDateEnd);
			$this->Trace('Definition:');
			$this->Trace($arrayTypologyDefinition);
			$this->Trace('Account:');
			$this->Trace($arrayAccount);
			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = AML_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,$strDateBegin,$strDateEnd);
			
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
			
			$arrayIgnoreTypes = array('DEBITCARD','DIRECTDEBIT','SALARY','OTHER');

			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 7;
			$arrayVariant['THRESHOLD_RATIO'] = 0.45;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['IGNORE_TYPES'] = $arrayIgnoreTypes;
			$arrayTypologyDefinition['VARIANTS'][] = $arrayVariant;			


			// Two transactions within 8 days, balacing to zero
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 0,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 10000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-08',
						'AMOUNT' => -10000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// Two transactions within 7 days, balacing to zero
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 0,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 10000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => -10000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'RAPIDMOVEMENT',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_RATIO' => 0.45,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-07',
					'RATIO' => 1.0,
					'DEPOSITS_AMOUNT' => 10000.0,
					'DEPOSITS_COUNT' => 1,
					'WITHDRAWALS_AMOUNT' => 10000.0,
					'WITHDRAWALS_COUNT' => 1,
					'NETFLOW' => 0.0,			
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 10000.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => -10000.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							)
						)					
					)
				);
	
			$this->TestCase_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// Two transactions within 7 days, balacing to +50% (above the threshold ratio), and after that another transaction that would NOT trigger to test if it was within 7 days.
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 0,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 10000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => -5000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-08',
						'AMOUNT' => 5000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'RAPIDMOVEMENT',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_RATIO' => 0.45,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-07',
					'RATIO' => 0.5,
					'DEPOSITS_AMOUNT' => 10000.0,
					'DEPOSITS_COUNT' => 1,
					'WITHDRAWALS_AMOUNT' => 5000.0,
					'WITHDRAWALS_COUNT' => 1,
					'NETFLOW' => 5000.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 10000.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => -5000.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							)
						)					
					)
				);
	
			$this->TestCase_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);



			// The 'Monthly' data case
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 0,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 100,
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => -1000,
						),
					array(
						'DATE' => '2019-01-03',
						'AMOUNT' => -1000,
						),
					array(
						'DATE' => '2019-01-05',
						'AMOUNT' => -45,
						),
					array(
						'DATE' => '2019-01-30',
						'AMOUNT' => 3000,
						),
					array(
						'DATE' => '2019-02-01',
						'AMOUNT' => 200,
						),
					array(
						'DATE' => '2019-02-02',
						'AMOUNT' => -100,
						),
					array(
						'DATE' => '2019-02-05',
						'AMOUNT' => -500,
						),
					array(
						'DATE' => '2019-02-05',
						'AMOUNT' => -24.5,
						),
					array(
						'DATE' => '2019-03-15',
						'AMOUNT' => 1300,
						),
					array(
						'DATE' => '2019-03-20',
						'AMOUNT' => 300,
						),
					array(
						'DATE' => '2019-03-21',
						'AMOUNT' => 1300,
						),
					array(
						'DATE' => '2019-04-15',
						'AMOUNT' => 4000,
						),
					array(
						'DATE' => '2019-04-20',
						'AMOUNT' => -5000,
						),
					array(
						'DATE' => '2019-05-01',
						'AMOUNT' => -6000,
						),
					array(
						'DATE' => '2019-05-03',
						'AMOUNT' => -1200,
						),
					array(
						'DATE' => '2019-05-05',
						'AMOUNT' => 120,
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'RAPIDMOVEMENT',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_RATIO' => 0.45,
					'START_DATE' => '2019-04-15',
					'END_DATE' => '2019-04-20',
					'RATIO' => 0.8,
					'DEPOSITS_AMOUNT' => 4000.0,
					'DEPOSITS_COUNT' => 1,
					'WITHDRAWALS_AMOUNT' => 5000.0,
					'WITHDRAWALS_COUNT' => 1,
					'NETFLOW' => -1000.0, 
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-04-15',
							'AMOUNT' => 4000
							),
						array(
							'DATE' => '2019-04-20',
							'AMOUNT' => -5000
							)
						)					
					)
				);
			$this->TestCase_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);
						

			// Two transactions within 7 days, balacing to +50% (above the threshold ratio), and after that another transaction that would NOT trigger to test if it was within 7 days. However the transactions are ignored due to their type.
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 0,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 10000.0,
						'ID' => 'B',
						'TYPE' => 'SALARY'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => -5000.0,
						'ID' => 'B',
						'TYPE' => 'DEBITCARD'
						),
					array(
						'DATE' => '2019-01-08',
						'AMOUNT' => 5000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array();
	
			$this->TestCase_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);




			// Two transactions within 7 days - close to the minimum threshold
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 0,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 150.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => -160.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-08',
						'AMOUNT' => 5000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'RAPIDMOVEMENT',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_RATIO' => 0.45,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-07',
					'RATIO' => 0.9375,
					'DEPOSITS_AMOUNT' => 150.0,
					'DEPOSITS_COUNT' => 1,
					'WITHDRAWALS_AMOUNT' => 160.0,
					'WITHDRAWALS_COUNT' => 1,
					'NETFLOW' => -10.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 150.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => -160.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							)
						)					
					)
				);
	
			$this->TestCase_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

			// Two transactions within 7 days - below the minimum threshold
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 0,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 149.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => -139.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-08',
						'AMOUNT' => 5000.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array();
	
			$this->TestCase_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);



			// 4 transactions that shoukd trigger the 1 day variant twice:

			$arrayTypologyDefinition = array();
			
			$arrayIgnoreTypes = array('DEBITCARD','DIRECTDEBIT','SALARY','OTHER');

			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 1;
			$arrayVariant['THRESHOLD_RATIO'] = 0.45;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['IGNORE_TYPES'] = $arrayIgnoreTypes;
			$arrayTypologyDefinition['VARIANTS'][] = $arrayVariant;			

			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 0,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 150.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',  
						'AMOUNT' => -160.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => 150.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-07',  
						'AMOUNT' => -160.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'RAPIDMOVEMENT',
					'THRESHOLD_DAYS' => 1,
					'THRESHOLD_RATIO' => 0.45,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01',
					'RATIO' => 0.9375,
					'DEPOSITS_AMOUNT' => 150.0,
					'DEPOSITS_COUNT' => 1,
					'WITHDRAWALS_AMOUNT' => 160.0,
					'WITHDRAWALS_COUNT' => 1,
					'NETFLOW' => -10.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 150.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => -160.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							)
						)					
					),
				array(
					'TYPOLOGY' => 'RAPIDMOVEMENT',
					'THRESHOLD_DAYS' => 1,
					'THRESHOLD_RATIO' => 0.45,
					'START_DATE' => '2019-01-07',
					'END_DATE' => '2019-01-07',
					'RATIO' => 0.9375,
					'DEPOSITS_AMOUNT' => 150.0,
					'DEPOSITS_COUNT' => 1,
					'WITHDRAWALS_AMOUNT' => 160.0,
					'WITHDRAWALS_COUNT' => 1,
					'NETFLOW' => -10.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => 150.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => -160.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							)
						)					
					)
				);
	
			$this->TestCase_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// Two variants with overlapping red flags - check if the filtering works
			// Two transactions on one day and another two within 7 days - both variants would be triggered
			
			$arrayTypologyDefinition = array();
			
			$arrayIgnoreTypes = array('DEBITCARD','DIRECTDEBIT','SALARY','OTHER');

			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 1;
			$arrayVariant['THRESHOLD_RATIO'] = 0.45;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['IGNORE_TYPES'] = $arrayIgnoreTypes;
			$arrayTypologyDefinition['VARIANTS'][] = $arrayVariant;			

			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 7;        
			$arrayVariant['THRESHOLD_RATIO'] = 0.45;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['IGNORE_TYPES'] = $arrayIgnoreTypes;
			$arrayTypologyDefinition['VARIANTS'][] = $arrayVariant;			

			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 0,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 150.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',  
						'AMOUNT' => -160.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => 150.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-07',  
						'AMOUNT' => -160.0,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'RAPIDMOVEMENT',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_RATIO' => 0.45,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-07',
					'RATIO' => 0.9375,
					'DEPOSITS_AMOUNT' => 300.0,
					'DEPOSITS_COUNT' => 2,
					'WITHDRAWALS_AMOUNT' => 320.0,
					'WITHDRAWALS_COUNT' => 2,
					'NETFLOW' => -20.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 150.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => -160.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => 150.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => -160.0,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							)					
						)					
					)
				);
			$this->TestCase_RaiseRedFlags_RapidMovement($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);



		}
	}
	
	

		
