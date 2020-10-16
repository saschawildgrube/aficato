<?php
	
	require_once(GetSourceDir()."assemblies/aml/aml.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("AML_TransformAccountsToAccountChart");
		}
		  

		function TestCase_TransformAccountToAccountChart(
			$arrayAccount,
			$strDateBegin,
			$strDateEnd,
			$strTimePeriod,
			$arrayTypes,
			$arrayExpectedResult)
		{ 
			$this->Trace("TestCase_TransformAccountToAccountChart");
	
			$this->Trace("Account:");
			$this->Trace($arrayAccount);
			$this->Trace("Date Begin : ".$strDateBegin);
			$this->Trace("Date End   : ".$strDateEnd);
			$this->Trace("Time Period: ".$strTimePeriod);
			$this->Trace("Types:");
			$this->Trace($arrayTypes);


			$this->Trace("Expected"); 
			$this->Trace($arrayExpectedResult);

			$arrayAccountChart = AML_TransformAccountToAccountChart($arrayAccount,$strDateBegin,$strDateEnd,$strTimePeriod,$arrayTypes);
			
			$this->Trace("Result");
			$this->Trace($arrayAccountChart);

			if ($arrayAccountChart == $arrayExpectedResult)
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


		function CallbackTest()
		{
			parent::CallbackTest();
					
			$this->SetResult(true);


			$arrayAccount = false;
			$arrayExpected = false;
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-01-01','2019-01-31','',null,$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-01-01','2019-01-31','QUARTERLY',null,$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'','','MONTHLY',null,$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-01-01','2019-01-31','QUARTERLY',null,$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-01-01','2019-01-31','ANNUALLY',null,$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-01-01','','MONTHLY',null,$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'','2019-01-31','MONTHLY',null,$arrayExpected);

			$arrayAccount = array();
			$arrayExpected = false;
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-12-31','2019-01-31','MONTHLY',null,$arrayExpected);

			// MONTHLY with empty input array

			$arrayAccount = array();
			$arrayExpected = array(
				'metric' => '¤',
				'bars' => array( 				
					array(
						'axis_text' => '2019-01',
						'line_value_left' => 0,
						'line_value_right' => 0,
						'bar_positive_amount' => 0,
						'bar_negative_amount' => 0,
						'bar_positive_count' => '0',
						'bar_negative_count' => '0'
						)
					)
				);
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-01-01','2019-01-31','MONTHLY',null,$arrayExpected);

			// MONTHLY 1

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
				'metric' => '€',
				'bars' => array( 				
					array(
						'axis_text' => '2019-01',
						'line_value_left' => 1000,
						'line_value_right' => 900,
						'bar_positive_amount' => 0,
						'bar_negative_amount' => 100,
						'bar_positive_count' => 0,
						'bar_negative_count' => 1
						),
					array(
						'axis_text' => '2019-02',
						'line_value_left' => 900,
						'line_value_right' => 1100,
						'bar_positive_amount' => 200,
						'bar_negative_amount' => 0,
						'bar_positive_count' => 1,
						'bar_negative_count' => 0
						)
					)
				);
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-01-01','2019-02-28','MONTHLY',null,$arrayExpected);


			// MONTHLY 2

			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => -100,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => -1000,
						'ID' => 'C',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-03',
						'AMOUNT' => 500,
						'ID' => 'D',
						'TYPE' => ''
						)
						
					)
				);
			$arrayTypes = array(
				'WIRE',
				'CASH',
				'*'); // Asterix for 'everything else'
			$arrayExpected = array(
				'metric' => '€',
				'bars' => array( 				
					array(
						'axis_text' => '2019-01',
						'line_value_left' => 1000,
						'line_value_right' => 400,
						'bar_positive_amount' => 500,
						'bar_negative_amount' => 1100,
						'bar_positive_count' => 1,
						'bar_negative_count' => 2,
						'bar_positive_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => 'CASH',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => '*',
								'amount' => 500,
								'count' => 1)
							),
						'bar_negative_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 1000,
								'count' => 1),
							array(
								'name' => 'CASH',
								'amount' => 100,
								'count' => 1),
							array(
								'name' => '*',
								'amount' => 0,
								'count' => 0)
							)
						)
					)
				);
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-01-01','2019-01-31','MONTHLY',$arrayTypes,$arrayExpected);


			// WEEKLY

			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2018-12-29',   
						'AMOUNT' => -100,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => -1000,
						'ID' => 'C',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-03',
						'AMOUNT' => 500,
						'ID' => 'D',
						'TYPE' => ''
						)
						
					)
				);
			$arrayTypes = array(
				'WIRE',
				'CASH',
				'*'); // Asterix for 'everything else'
			$arrayExpected = array(
				'metric' => '€',
				'bars' => array( 				
					array(
						'axis_text' => '2018-12-24',
						'line_value_left' => 1000,
						'line_value_right' => 900,
						'bar_positive_amount' => 0,
						'bar_negative_amount' => 100,
						'bar_positive_count' => 0,
						'bar_negative_count' => 1,
						'bar_positive_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => 'CASH',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => '*',
								'amount' => 0,
								'count' => 0)
							),
						'bar_negative_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => 'CASH',
								'amount' => 100,
								'count' => 1),
							array(
								'name' => '*',
								'amount' => 0,
								'count' => 0)
							)
						),
					array(
						'axis_text' => '2018-12-31',  
						'line_value_left' => 900,
						'line_value_right' => 400,
						'bar_positive_amount' => 500,
						'bar_negative_amount' => 1000,
						'bar_positive_count' => 1,
						'bar_negative_count' => 1,
						'bar_positive_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => 'CASH',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => '*',
								'amount' => 500,
								'count' => 1)
							),
						'bar_negative_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 1000,
								'count' => 1),
							array(
								'name' => 'CASH',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => '*',
								'amount' => 0,
								'count' => 0)
							)
						)
					)
				);
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2018-12-28','2019-01-06','WEEKLY',$arrayTypes,$arrayExpected);

			
			// DAILY

			$arrayAccount = array(
				'ID' => 'A',
				'BALANCE_BEGIN' => 1000,
				'CURRENCY' => 'EUR',
				'TRANSACTIONS' => array(
					array(
						'DATE' => '2019-01-01',
						'AMOUNT' => -100,
						'ID' => 'B',
						'TYPE' => 'CASH'
						),
					array(
						'DATE' => '2019-01-02',
						'AMOUNT' => -1000,
						'ID' => 'C',
						'TYPE' => 'WIRE'
						),
					array(
						'DATE' => '2019-01-03',
						'AMOUNT' => 500,
						'ID' => 'D',
						'TYPE' => ''
						)
						
					)
				);
			$arrayTypes = array(
				'WIRE',
				'CASH',
				'*'); // Asterix for 'everything else'
			$arrayExpected = array(
				'metric' => '€',
				'bars' => array( 				
					array(
						'axis_text' => '2019-01-01',
						'line_value_left' => 1000,
						'line_value_right' => 900,
						'bar_positive_amount' => 0,
						'bar_negative_amount' => 100,
						'bar_positive_count' => 0,
						'bar_negative_count' => 1,
						'bar_positive_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => 'CASH',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => '*',
								'amount' => 0,
								'count' => 0)
							),
						'bar_negative_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => 'CASH',
								'amount' => 100,
								'count' => 1),
							array(
								'name' => '*',
								'amount' => 0,
								'count' => 0)
							)
						),
					array(
						'axis_text' => '2019-01-02',
						'line_value_left' => 900,
						'line_value_right' => -100,
						'bar_positive_amount' => 0,
						'bar_negative_amount' => 1000,
						'bar_positive_count' => 0,
						'bar_negative_count' => 1,
						'bar_positive_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => 'CASH',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => '*',
								'amount' => 0,
								'count' => 0)
							),
						'bar_negative_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 1000,
								'count' => 1),
							array(
								'name' => 'CASH',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => '*',
								'amount' => 0,
								'count' => 0)
							)
						),
					array(
						'axis_text' => '2019-01-03',
						'line_value_left' => -100,
						'line_value_right' => 400,
						'bar_positive_amount' => 500,
						'bar_negative_amount' => 00,
						'bar_positive_count' => 1,
						'bar_negative_count' => 0,
						'bar_positive_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => 'CASH',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => '*',
								'amount' => 500,
								'count' => 1)
							),
						'bar_negative_fractions' => array(
							array(
								'name' => 'WIRE',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => 'CASH',
								'amount' => 0,
								'count' => 0),
							array(
								'name' => '*',
								'amount' => 0,
								'count' => 0)
							)
						)
					)
				);
			$this->TestCase_TransformAccountToAccountChart($arrayAccount,'2019-01-01','2019-01-03','DAILY',$arrayTypes,$arrayExpected);


		}
	}
	
	

		
