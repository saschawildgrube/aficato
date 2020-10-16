<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML_RaiseRedFlags_IntensiveCash');
		}
		  

		function TestCase_RaiseRedFlags_IntensiveCash(
			$arrayAccount,
			$arrayTypologyDefinition,
			$strDateBegin,
			$strDateEnd,
			$arrayExpectedResult)
		{ 
			$this->Trace('RaiseRedFlags_IntensiveCash');
	
			$this->Trace('Date begin : '.$strDateBegin);
			$this->Trace('Date end   : '.$strDateEnd);
			$this->Trace('Definition:');
			$this->Trace($arrayTypologyDefinition);
			$this->Trace('Account:');
			$this->Trace($arrayAccount);
			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = AML_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,$strDateBegin,$strDateEnd);
			
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

			$arrayDefinition = array();


			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 1;
			$arrayVariant['THRESHOLD_TRANSACTION_COUNT'] = 1;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['EUR'] = 3000;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['USD'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['USD'] = 3000;
			$arrayTypologyDefinition['VARIANTS'][] = $arrayVariant;

			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 7;
			$arrayVariant['THRESHOLD_TRANSACTION_COUNT'] = 3;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['EUR'] = 1500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['EUR'] = 1500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['USD'] = 1500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['USD'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['USD'] = 1500;
			$arrayTypologyDefinition['VARIANTS'][] = $arrayVariant;		


			$arrayAccount = false;
			$arrayExpected = false; // because no CURRENCY is defined in the account
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false; // because no CURRENCY is defined in the account
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// Two amounts on the same day, but they do not reach the threshold
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 750,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 749,
						'ID' => 'B',
						'TYPE' => 'CASH'
						)
					)
				);
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);





			// Two amounts that do not sum up above the total amount threshold and hence do not trigger any scenario
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 1450, 
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 1500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						)
					)
				);
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// Two negative amounts that do not sum up above the total amount threshold and hence do not trigger any scenario
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 10000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => -1400,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => -1500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						)
					)
				);
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


/*

			// Two amounts below the single amount threshold that sum up above the total amount threshold within 7 days
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 5000,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => 5000,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'INTENSIVECASH',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_TRANSACTION_COUNT' => 2,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-02',
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 2,
					'TOTAL_AMOUNT' => 10000.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 5000,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-02',
							'AMOUNT' => 5000,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							)
						)
					)
				);
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// More transactions that should not trigger the rule and two withdrawals below the single amount threshold that sum up above the total amount threshold within 7 days
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => -2500,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => 500,
						'ID' => 'C',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-03',
						'AMOUNT' => 100,
						'ID' => 'C',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-05',
						'AMOUNT' => 10000,
						'ID' => 'C',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => -3000,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-10',
						'AMOUNT' => -100,
						'ID' => 'C',
						'TYPE' => 'WIRE'
						)					
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'INTENSIVECASH',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_TRANSACTION_COUNT' => 2,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-07',
					'DIRECTION' => 'WITHDRAWALS',
					'TRANSACTION_COUNT' => 2,
					'TOTAL_AMOUNT' => 5500.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => -2500,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => -3000,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							)
						)
					)
				);
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

*/



			// One amount that qualifies for the one day case
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 3000,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 1500,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
				
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'INTENSIVECASH',
					'THRESHOLD_DAYS' => 1,
					'THRESHOLD_AMOUNT' => 3000.0,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01',
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 1,
					'TOTAL_AMOUNT' => 3000.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 3000,
							'ID' => 'B',
							'TYPE' => 'CASH'
							)
						)					
					)					
				);
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// Three amounts that qualifies for the 7 days case
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-08',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						)					
					)
				);
				
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'INTENSIVECASH',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_AMOUNT' => 1500.0,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-07',
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 3,
					'TOTAL_AMOUNT' => 1500.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							),
						array(
							'DATE' => '2019-01-02',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							)
						)					
					)					
				);
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);



			// Two 7 days cases in one go
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-07',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						)																					
					)
				);
				
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'INTENSIVECASH',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_AMOUNT' => 1500.0,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01',
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 3,
					'TOTAL_AMOUNT' => 1500.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							),
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							),
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							)
						)					
					),					
				array(
					'TYPOLOGY' => 'INTENSIVECASH',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_AMOUNT' => 1500.0,
					'START_DATE' => '2019-01-07',
					'END_DATE' => '2019-01-07',
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 3,
					'TOTAL_AMOUNT' => 1500.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							),
						array(
							'DATE' => '2019-01-07',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							)
						)					
					)					
				);
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);





			// The one day and 7 days case in one go - checks if the overlap filter works
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 1500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 1500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => 500,
						'ID' => 'B',
						'TYPE' => 'CASH'
						)																				
					)
				);
				
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'INTENSIVECASH',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_AMOUNT' => 1500.0,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-02',
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 3,
					'TOTAL_AMOUNT' => 3500.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 1500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							),
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 1500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							),
						array(
							'DATE' => '2019-01-02',
							'AMOUNT' => 500,
							'ID' => 'B',
							'TYPE' => 'CASH'
							)
						)							
					)					
				);
			$this->TestCase_RaiseRedFlags_IntensiveCash($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);




		}
	}
	
	

		
