<?php
	
	require_once(GetSourceDir()."assemblies/aml/aml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("AML CreateMissingCounterpartyAccounts");
		}
		  

		function TestCase_CreateMissingCounterpartyAccounts(
			$arrayAccounts,
			$strDateBegin,
			$strDateEnd,
			$strDefaultCurrency,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_CreateMissingCounterpartyAccounts");
	
			$this->Trace("Accounts");
			$this->Trace($arrayAccounts);
			$this->Trace("Date begin      : ".$strDateBegin);
			$this->Trace("Date end        : ".$strDateEnd);
			$this->Trace("Default currency: ".$strDefaultCurrency);


			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayResult = AML_CreateMissingCounterpartyAccounts($arrayAccounts,$strDateBegin,$strDateEnd,$strDefaultCurrency);
			
			$this->Trace("Result");
			$this->Trace($arrayResult);

			if ($arrayResult == $arrayExpectedResult)
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

			$arrayAccounts = false;
			$arrayExpected = false;
			$this->TestCase_CreateMissingCounterpartyAccounts($arrayAccounts,'','','EUR',$arrayExpected);

			$arrayAccounts = array();
			$arrayExpected = array();
			$this->TestCase_CreateMissingCounterpartyAccounts($arrayAccounts,'','','EUR',$arrayExpected);

			$arrayAccounts = array(
				array(
					'ID' => 'A',
					'CURRENCY' => 'EUR',
					'DATAQUALITY' => 1,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => -100,
							'ID' => 'B',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						)
					)
				);

			$arrayExpected = array(
				array(
					'ID' => 'A',
					'CURRENCY' => 'EUR',
					'DATAQUALITY' => 1,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => -100,
							'ID' => 'B',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						)
					),
				array(
					'ID' => 'B',
					'CURRENCY' => 'EUR',
					'DATAQUALITY' => 0/*,
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 100,
							'ID' => 'A',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						)*/
					)					
				);
				
			$this->TestCase_CreateMissingCounterpartyAccounts($arrayAccounts,'','','EUR',$arrayExpected);





		}
	}
	
	

		
