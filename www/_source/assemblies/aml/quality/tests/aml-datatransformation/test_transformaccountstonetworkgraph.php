<?php
	
	require_once(GetSourceDir().'assemblies/aml/aml.inc');
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct('AML TransformAccountsToNetworkGraph');
		}
		  

		function TestCase_TransformAccountsToNetworkGraph(
			$arrayAccounts,
			$strDateBegin,
			$strDateEnd,
			$strDefaultCurrency,
			$arrayExpectedResult)
		{ 
			$this->Trace('TestCase_TransformAccountsToNetworkGraph');
	
			$this->Trace('Accounts');
			$this->Trace($arrayAccounts);
			$this->Trace('Date begin      : '.$strDateBegin);
			$this->Trace('Date end        : '.$strDateEnd);
			$this->Trace('Default currency: '.$strDefaultCurrency);

			$this->Trace('Expected'); 
			$this->Trace($arrayExpectedResult);

			$arrayNetworkGraph = AML_TransformAccountsToNetworkGraph($arrayAccounts,$strDateBegin,$strDefaultCurrency,$strDateEnd);
			
			$this->Trace('Result');
			$this->Trace($arrayNetworkGraph);

			if ($arrayNetworkGraph == $arrayExpectedResult)
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

			$arrayAccounts = false;
			$arrayExpected = false;
			$this->TestCase_TransformAccountsToNetworkGraph($arrayAccounts,'','','EUR',$arrayExpected);

			$arrayAccounts = array();
			$arrayExpected = array( 'nodes' => array(), 'links' => array() );
			$this->TestCase_TransformAccountsToNetworkGraph($arrayAccounts,'','','EUR',$arrayExpected);

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
					),
				array(
					'ID' => 'B',
					'CURRENCY' => 'EUR',					
					'TRANSACTIONS' => array(
						array(
							'DATE' => '2019-01-01',
							'AMOUNT' => 100,
							'ID' => 'A',
							'TYPE' => '',
							'DESCRIPTION' => ''
							)
						)
					)
				);
			$arrayExpected = array(
				'nodes' => array(
					array(
						'id' => 'A',
						'name' => 'A',
						'currency' => '€',
						'text_transaction_amount_in' => '0',
						'text_transaction_amount_out' => '100',
						'text_transaction_netflow' => '-100',
						'transaction_amount' => 100,
						'transaction_amount_in' => 0,
						'transaction_amount_out' => 100,
						'transaction_netflow' => -100,
						'transaction_count' => 1,
						'dataquality' => 1,
						'transaction_amount_factor' => 0.5						
						),
					array(
						'id' => 'B',
						'name' => 'B',
						'currency' => '€',
						'text_transaction_amount_in' => '100',
						'text_transaction_amount_out' => '0',
						'text_transaction_netflow' => '100',
						'transaction_amount' => 100,				
						'transaction_amount_in' => 100,
						'transaction_amount_out' => 0,
						'transaction_netflow' => 100,
						'transaction_count' => 1,				
						'dataquality' => 0,
						'transaction_amount_factor' => 0.5						
						)
					),
				'links' => array(
					array(
						'dataquality' => 1,
						'source' => 'A',
						'target' => 'B',
						'transaction_amount_in' => 0,
						'transaction_amount_out' => 100,
						'transaction_count_in' => 0,
						'transaction_count_out' => 1,
						'transaction_direction_quotient' => 0 
						)
					)
				);
			$this->TestCase_TransformAccountsToNetworkGraph($arrayAccounts,'','','EUR',$arrayExpected);




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
				'nodes' => array(
					array(
						'id' => 'A',
						'name' => 'A',
						'currency' => '€',
						'text_transaction_amount_in' => '0',
						'text_transaction_amount_out' => '100',
						'text_transaction_netflow' => '-100',
						'transaction_amount' => 100,
						'transaction_amount_in' => 0,
						'transaction_amount_out' => 100,
						'transaction_netflow' => -100,
						'transaction_count' => 1,
						'dataquality' => 1,
						'transaction_amount_factor' => 1						
						),
					array(
						'id' => 'B',
						'name' => 'B',
						'currency' => '€',
						/*'text_transaction_amount_in' => '100',
						'text_transaction_amount_out' => '0',
						'text_transaction_netflow' => '100',
						'transaction_amount' => 100,				
						'transaction_amount_in' => 100,
						'transaction_amount_out' => 0,
						'transaction_netflow' => 100,
						'transaction_count' => 1,				
						'dataquality' => 0,
						'transaction_amount_factor' => 0.5*/
						'text_transaction_amount_in' => 0,
						'text_transaction_amount_out' => 0,
						'text_transaction_netflow' => 0,
						'transaction_amount' => 0,				
						'transaction_amount_in' => 0,
						'transaction_amount_out' => 0,
						'transaction_netflow' => 0,
						'transaction_count' => 0,				
						'dataquality' => 0,
						'transaction_amount_factor' => 0												
						)
					),
				'links' => array(
					array(
						'dataquality' => 1,
						'source' => 'A',
						'target' => 'B',
						'transaction_amount_in' => 0,
						'transaction_amount_out' => 100,
						'transaction_count_in' => 0,
						'transaction_count_out' => 1,
						'transaction_direction_quotient' => 0 
						)
					)
				);
			$this->TestCase_TransformAccountsToNetworkGraph($arrayAccounts,'','','EUR',$arrayExpected);






		}
	}
	
	

		
