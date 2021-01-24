<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML_CalculateAccount');
		}
		  

		function TestCase_CalculateAccount(
			$arrayAccount,
			$strDateBegin,
			$strDateEnd,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_CalculateAccount');
	
			$this->Trace('Date begin : '.$strDateBegin);
			$this->Trace('Date end   : '.$strDateEnd);
			$this->Trace('Account:');
			$this->Trace($arrayAccount);
			$this->Trace('Expected:'); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = AML_CalculateAccount($arrayAccount,$strDateBegin,$strDateEnd);
			
			$this->Trace('Result:');
			$this->Trace($arrayResult);

			if ($arrayResult == $arrayExpectedResult)
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
					
			$this->SetResult(true);


			$arrayAccount = false;
			$arrayExpected = false;
			$this->TestCase_CalculateAccount($arrayAccount,'2019-01-01','2019-01-31',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_CalculateAccount($arrayAccount,'2019-01-01','2019-01-31',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_CalculateAccount($arrayAccount,'','',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_CalculateAccount($arrayAccount,'2019-01-01','2019-01-31',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_CalculateAccount($arrayAccount,'2019-01-01','2019-01-31',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_CalculateAccount($arrayAccount,'2019-01-01','',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_CalculateAccount($arrayAccount,'','2019-01-31',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_CalculateAccount($arrayAccount,'2019-12-31','2019-01-31',$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_CalculateAccount($arrayAccount,'2019-01-01','2019-01-31',$arrayExpected);

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
					),
				'CALCULATED' => true,
    		'TRANSACTION_COUNT_IN' => 1,
    		'TRANSACTION_COUNT_OUT' => 1,
    		'TRANSACTION_COUNT' => 2,
    		'TRANSACTION_AMOUNT_IN' => 200,
    		'TRANSACTION_AMOUNT_OUT' => 100,
    		'TRANSACTION_NETFLOW' => 100,
    		'COUNTERPARTIES' => array(
					array(
						'ID' => 'B',
						'TRANSACTION_COUNT_IN' => 1,
						'TRANSACTION_COUNT_OUT' => 1,
						'TRANSACTION_COUNT' => 2,
            'TRANSACTION_AMOUNT_IN' => 200.0,
            'TRANSACTION_AMOUNT_OUT' => 100.0,
            'TRANSACTION_NETFLOW' => 100.0
						)
					),
				'TRANSACTION_FIRST_DATE' => '2019-01-01',
	    	'TRANSACTION_LAST_DATE' => '2019-02-01',
	    	'BALANCE_END' => 1100,
				);
			$this->TestCase_CalculateAccount($arrayAccount,'','',$arrayExpected);
			$this->TestCase_CalculateAccount($arrayAccount,'2018-01-01','2019-03-01',$arrayExpected);
			$this->TestCase_CalculateAccount($arrayAccount,'2019-01-01','2019-02-01',$arrayExpected);

			$arrayExpected = array(
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
					),
				'CALCULATED' => true,
    		'TRANSACTION_COUNT_IN' => 0,
    		'TRANSACTION_COUNT_OUT' => 1,
    		'TRANSACTION_COUNT' => 1,
    		'TRANSACTION_AMOUNT_IN' => 0,
    		'TRANSACTION_AMOUNT_OUT' => 100,
    		'TRANSACTION_NETFLOW' => -100,
    		'COUNTERPARTIES' => array(
					array(
						'ID' => 'B',
						'TRANSACTION_COUNT_IN' => 0,
						'TRANSACTION_COUNT_OUT' => 1,
						'TRANSACTION_COUNT' => 1,
            'TRANSACTION_AMOUNT_IN' => 0.0,
            'TRANSACTION_AMOUNT_OUT' => 100.0,
            'TRANSACTION_NETFLOW' => -100.0
						)
					),
				'TRANSACTION_FIRST_DATE' => '2019-01-01',
	    	'TRANSACTION_LAST_DATE' => '2019-01-01',
	    	'BALANCE_END' => 900,
				);
			$this->TestCase_CalculateAccount($arrayAccount,'2018-01-01','2019-01-01',$arrayExpected);
			$this->TestCase_CalculateAccount($arrayAccount,'','2019-01-01',$arrayExpected);
			$this->TestCase_CalculateAccount($arrayAccount,'2019-01-01','2019-01-01',$arrayExpected);



		}
	}
	
	

		
