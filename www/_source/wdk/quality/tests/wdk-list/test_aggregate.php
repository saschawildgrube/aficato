<?php

	require_once(GetWDKDir()."wdk_list.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK CList::Aggregate");
		}
		

		function TestCase_ListAggregate(
			$list,
			$arrayColumns,
			$strResultColumn,
			$bExpectedResult,
			$listExpected)
		{ 
			
			$this->Trace("TestCase_ListAggregate");
	
			$this->Trace("List:");
			$this->Trace($list->GetListArray());
			$this->Trace("Columns:");
			$this->Trace($arrayColumns);
			$this->Trace("Result column:");
			$this->Trace($strResultColumn);
			$this->Trace("Expected List:");
			$this->Trace($listExpected->GetListArray());
	
	
			// Test function here
			$bResult = $list->Aggregate($arrayColumns,$strResultColumn);
			
			$this->Trace('bResult = '.RenderBool($bResult));
			
			if ($bResult != $bExpectedResult)
			{
				$this->Trace("Testcase FAILED!");	
				$this->Trace("");
				$this->Trace("");
				$this->SetResult(false);	
				return;
			}
	
			if ($list->GetListArray() != $listExpected->GetListArray())
			{
				$this->Trace("Resulting List does not match expectation!");	
				$this->Trace($list->GetListArray());
				$this->Trace("Testcase FAILED!");
				$this->Trace("");
				$this->Trace("");
				$this->SetResult(false);	
				return;
			}

			$this->Trace("Testcase PASSED!");
			$this->Trace("");
			$this->Trace("");
			
		}



		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->SetResult(true);
			
			$arrayList = array(
				array(
					"ID" => 1,
					"NAME" => "John",
					"SEX" => "male",
					"AGE" => "adult"
					),
				array(
					"ID" => 2,
					"NAME" => "Jake",
					"SEX" => "male",
					"AGE" => "adult"
					),
				array(
					"ID" => 3,
					"NAME" => "Mary",
					"SEX" => "female",
					"AGE" => "adult"
					),
				array(
					"ID" => 4,
					"NAME" => "Susan",
					"SEX" => "female",
					"AGE" => "adult"
					),
				array(
					"ID" => 5,
					"NAME" => "Lisa",
					"SEX" => "female",
					"AGE" => "adult"
					),
				array(
					"ID" => 6,
					"NAME" => "Louise",
					"SEX" => "female",
					"AGE" => "child"
					)
				);






			// TESTCASE 1

			$list = new CList();
			$list->SetListArray($arrayList);


			$arrayColumns = array('SEX','AGE');
			$strResultColumn = 'COUNT';
			
			$arrayListExpected = array(
				array(
					"SEX" => "male",
					"AGE" => "adult",
					"COUNT" => 2
					),
				array(
					"SEX" => "female",
					"AGE" => "adult",
					"COUNT" => 3
					),
				array(
					"SEX" => "female",
					"AGE" => "child",
					"COUNT" => 1
					)
				);
			$listExpected = new CList();
			$listExpected->SetListArray($arrayListExpected);
			
			$this->TestCase_ListAggregate(
				$list,
				$arrayColumns,
				$strResultColumn,
				true,
				$listExpected);
				
				
				
		}
	}
	
	

		
