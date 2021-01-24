<?php
	
	require_once(GetSourceDir()."assemblies/aml/aml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("AML TransformTransactionsToAccounts");
		}
		  

		function TestCase_TransformTransactionsToAccounts(
			$arrayTransactions,
			$arrayMapping,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_TransformTransactionsToAccounts");
	
			$this->Trace("Transactions");
			$this->Trace($arrayTransactions);
			$this->Trace("Mapping");
			$this->Trace($arrayMapping);

			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayAccounts = AML_TransformTransactionsToAccounts($arrayTransactions,$arrayMapping);
			
			$this->Trace("Result");
			$this->Trace($arrayAccounts);

			if ($arrayAccounts == $arrayExpectedResult)
			{
				$this->Trace("Testcase PASSED!");
			}
			else
			{
				$this->Trace("Testcase FAILED!");	
				$this->SetResult(false);	
			}

			$this->Trace("");
			$this->Trace("");
		}


		function OnTest()
		{
			parent::OnTest();
					
			$this->SetResult(true);

			$arrayTransactions = false;
			$arrayMapping = array();
			$arrayExpected = false;
			$this->TestCase_TransformTransactionsToAccounts($arrayTransactions,$arrayMapping,$arrayExpected);


			$arrayTransactions = array();
			$arrayMapping = array();
			$arrayExpected = array();
			$this->TestCase_TransformTransactionsToAccounts($arrayTransactions,$arrayMapping,$arrayExpected);


			
			$arrayTransactions = array(
				array(
					'DATE' => '2019-01-01',
					'AMOUNT' => '100',
					'FROM' => 'A',
					'TO' => 'B')
				);
			$arrayMapping = array();
			$arrayExpected = array(
				array(
					'ID' => 'A',
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => -100,
							'ID' => 'B',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						),
					'DATAQUALITY' => 1
					),
				array(
					'ID' => 'B',
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 100,
							'ID' => 'A',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						),
					'DATAQUALITY' => 1
					)
				);
			$this->TestCase_TransformTransactionsToAccounts($arrayTransactions,$arrayMapping,$arrayExpected);


		$arrayTransactions = array(
				array(
					'TRANSACTION_DATE' => '2019-01-01',
					'TRANSACTION_AMOUNT' => '100',
					'SOURCE' => 'A',
					'BENEFICIARY' => 'B')
				);
			$arrayMapping = array(
				'DATE' => 'TRANSACTION_DATE',
				'AMOUNT' => 'TRANSACTION_AMOUNT',
				'FROM' => 'SOURCE',
				'TO' => 'BENEFICIARY');
			$arrayExpected = array(
				array(
					'ID' => 'A',
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => -100,
							'ID' => 'B',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						),
					'DATAQUALITY' => 1
					),
				array(
					'ID' => 'B',
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 100,
							'ID' => 'A',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						),
					'DATAQUALITY' => 1
					)
				);
			$this->TestCase_TransformTransactionsToAccounts($arrayTransactions,$arrayMapping,$arrayExpected);



		$arrayTransactions = array(
				array(
					'TRANSACTION_DATE' => '12/24/2019',
					'TRANSACTION_AMOUNT' => '1,000.99',
					'SOURCE' => 'A',
					'BENEFICIARY' => 'B')
				);
			$arrayMapping = array(
				'DATE' => 'TRANSACTION_DATE',
				'AMOUNT' => 'TRANSACTION_AMOUNT',
				'FROM' => 'SOURCE',
				'TO' => 'BENEFICIARY');
			$arrayExpected = array(
				array(
					'ID' => 'A',
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-12-24',
							'AMOUNT' => -1000.99,
							'ID' => 'B',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						),
					'DATAQUALITY' => 1
					),
				array(
					'ID' => 'B',
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-12-24',
							'AMOUNT' => 1000.99,
							'ID' => 'A',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						),
					'DATAQUALITY' => 1
					)
				);
			$this->TestCase_TransformTransactionsToAccounts($arrayTransactions,$arrayMapping,$arrayExpected);



		}
	}
	
	

		
