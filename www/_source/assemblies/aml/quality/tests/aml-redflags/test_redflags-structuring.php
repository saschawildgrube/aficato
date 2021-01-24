<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML_RaiseRedFlags_Structuring');
		}
		  

		function TestCase_RaiseRedFlags_Structuring(
			$arrayAccount,
			$arrayTypologyDefinition,
			$strDateBegin,
			$strDateEnd,
			$arrayExpectedResult)
		{ 
			$this->Trace('RaiseRedFlags_Structuring');
	
			$this->Trace('Date begin : '.$strDateBegin);
			$this->Trace('Date end   : '.$strDateEnd);
			$this->Trace('Definition:');
			$this->Trace($arrayTypologyDefinition);
			$this->Trace('Account:');
			$this->Trace($arrayAccount);
			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = AML_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,$strDateBegin,$strDateEnd);
			
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

			$arrayDefinition = array();

			$arrayIgnoreTypes = array('DEBITCARD','DIRECTDEBIT','SALARY','OTHER');

			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 1;
			$arrayVariant['THRESHOLD_TRANSACTION_COUNT'] = 2;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['EUR'] = 9500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['EUR'] = 3000;
			$arrayVariant['IGNORE_TYPES'] = $arrayIgnoreTypes;
			$arrayTypologyDefinition['VARIANTS'][] = $arrayVariant;

			$arrayVariant = array();
			$arrayVariant['THRESHOLD_DAYS'] = 7;
			$arrayVariant['THRESHOLD_TRANSACTION_COUNT'] = 2;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MAX']['EUR'] = 9500;
			$arrayVariant['THRESHOLD_TRANSACTION_AMOUNT_MIN']['EUR'] = 150;
			$arrayVariant['THRESHOLD_AMOUNT_TOTAL']['EUR'] = 5000;
			$arrayVariant['IGNORE_TYPES'] = $arrayIgnoreTypes;
			$arrayTypologyDefinition['VARIANTS'][] = $arrayVariant;			


			$arrayAccount = false;
			$arrayExpected = false; // because no CURRENCY is defined in the account
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false; // because no CURRENCY is defined in the account
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// Two amounts on the same day, but each of them above the threshold for a single amount
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 10000,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 10000,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// Two amounts on the same day, but the sum is below the total amount threshold
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 1200,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 1200,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


			// Two amounts below the single amount threshold that sum up above the total amount threshold, but the time between is longer than 8 days
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
						'DATE' => '2019-01-09',
						'AMOUNT' => 5000,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array();
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);




			// Two amounts below the single amount threshold that sum up above the total amount threshold
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 2000,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 2000,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						)
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'STRUCTURING',
					'THRESHOLD_DAYS' => 1,
					'THRESHOLD_TRANSACTION_COUNT' => 2,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01',
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 2,
					'TOTAL_AMOUNT' => 4000.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 2000,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 2000,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							)
						)					
					)
				);
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);





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
					'TYPOLOGY' => 'STRUCTURING',
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
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);


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
					'TYPOLOGY' => 'STRUCTURING',
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
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);




			// This setup triggers the multple red flag issue
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 2500,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 500,
						'ID' => 'C',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',
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
					'TYPOLOGY' => 'STRUCTURING',
					'THRESHOLD_DAYS' => 1,
					'THRESHOLD_TRANSACTION_COUNT' => 2,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01', 
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 2,
					'TOTAL_AMOUNT' => 3000.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 2500,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 500,
							'ID' => 'C',
							'TYPE' => 'WIRE'
							)
						)
					)
				);
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);





			// This checks if amounts below the min threshold and some transaction types are ignored properly
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 2500,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 100, // should be ignored
						'ID' => 'D',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 4500,
						'ID' => 'D',
						'TYPE' => 'SALARY' // should be ignored
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 500,
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
					'TYPOLOGY' => 'STRUCTURING',
					'THRESHOLD_DAYS' => 1,
					'THRESHOLD_TRANSACTION_COUNT' => 2,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-01', 
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 2,
					'TOTAL_AMOUNT' => 3000.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 2500,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 500,
							'ID' => 'C',
							'TYPE' => 'WIRE'
							)
						)
					)
				);
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);







			// This triggers the one day and 7 day scenario at the same time. But only the 7 days scenario red flag should be reported
			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 1500,
						'ID' => 'A',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => 1500,
						'ID' => 'B',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => 2000,
						'ID' => 'C',
						'TYPE' => 'WIRE'
						)				
					)
				);
			$arrayExpected = array(
				array(
					'TYPOLOGY' => 'STRUCTURING',
					'THRESHOLD_DAYS' => 7,
					'THRESHOLD_TRANSACTION_COUNT' => 2,
					'START_DATE' => '2019-01-01',
					'END_DATE' => '2019-01-02', 
					'DIRECTION' => 'DEPOSITS',
					'TRANSACTION_COUNT' => 3,
					'TOTAL_AMOUNT' => 5000.0,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 1500,
							'ID' => 'A',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 1500,
							'ID' => 'B',
							'TYPE' => 'WIRE'
							),
						array(
							'DATE' => '2019-01-02',
							'AMOUNT' => 2000,
							'ID' => 'C',
							'TYPE' => 'WIRE'
							)
						)
					)
				);
			$this->TestCase_RaiseRedFlags_Structuring($arrayAccount,$arrayTypologyDefinition,'','',$arrayExpected);





		}
	}
	
	

		
