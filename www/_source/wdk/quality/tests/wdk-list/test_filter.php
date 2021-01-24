<?php

	require_once(GetWDKDir()."wdk_list.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK CList::Filter");
		}
		

		function TestCase_ListFilter(
			$list,
			$arrayFilterOptions,
			$bExpectedResult,
			$listExpected)
		{ 
			
			$this->Trace("TestCase_ListFilter");
	
			$this->Trace("List:");
			$this->Trace($list->GetListArray());
			$this->Trace("Filter Options:");
			$this->Trace($arrayFilterOptions);
			$this->Trace("Expected List:");
			$this->Trace($listExpected->GetListArray());
	
	
			// Test function here
			$bResult = $list->Filter($arrayFilterOptions);
			
			$this->Trace("bResult = \"bResult\"");
			
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



		function OnTest()
		{
			parent::OnTest();
			
			$this->SetResult(true);
			
			$arrayList = array(
				array(
					"ID" => 1,
					"COL1" => "Alpha",
					"COL2" => "10",
					"COL3" => "Obelix"
					),
				array(
					"ID" => 2,
					"COL1" => "Beta",
					"COL2" => "10",
					"COL3" => "Asterix"
					),
				array(
					"ID" => 3,
					"COL1" => "Alpha",
					"COL2" => "10",
					"COL3" => "Caesar"
					),
				array(
					"ID" => 4,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "Nero"
					),
				array(
					"ID" => 5,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "Idefix"
					),
				array(
					"ID" => 6,
					"COL1" => "Beta",
					"COL2" => "10",
					"COL3" => "Miraculix"
					)
				);






			// TESTCASE 1

			$list = new CList();
			$list->SetListArray($arrayList);


			$arrayFilterOptions = array(
				array(
					"field" => "COL3",
					"operator" => "contains",
					"value" => "er"
					)
				);
			
			$arrayListExpected = array(
				array(
					"ID" => 2,
					"COL1" => "Beta",
					"COL2" => "10",
					"COL3" => "Asterix"
					)
					,
				array(
					"ID" => 4,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "Nero"
					)
				);
			$listExpected = new CList();
			$listExpected->SetListArray($arrayListExpected);
			
			$this->TestCase_ListFilter(
				$list,
				$arrayFilterOptions,
				true,
				$listExpected);
				
				
				
				
				
			

			// TESTCASE 2


			$list = new CList();
			$list->SetListArray($arrayList);


			$arrayFilterOptions = array(
				array(
					"field" => "COL1",
					"operator" => "=",
					"value" => "Beta")
					,
				array(
					"field" => "COL2",
					"operator" => "<",
					"value" => "10")
				);
			
			$arrayListExpected = array(
				array(
					"ID" => 4,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "Nero"
					),
				array(
					"ID" => 5,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "Idefix"
					)
				);
			$listExpected = new CList();
			$listExpected->SetListArray($arrayListExpected);
			
			$this->TestCase_ListFilter(
				$list,
				$arrayFilterOptions,
				true,
				$listExpected);



			// TESTCASE 3


			$list = new CList();
			$list->SetListArray($arrayList);

			$arrayFilterOptions = array(
				array(
					"field" => "COL3",
					"operator" => "in",
					"value" => "Nero,Idefix")
				);
			
			$arrayListExpected = array(
				array(
					"ID" => 4,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "Nero"
					),
				array(
					"ID" => 5,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "Idefix"
					)
				);
			$listExpected = new CList();
			$listExpected->SetListArray($arrayListExpected);
			
			$this->TestCase_ListFilter(
				$list,
				$arrayFilterOptions,
				true,
				$listExpected);
				
				
				
		}
	}
	
	

		
