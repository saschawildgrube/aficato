<?php
	
	require_once(GetWDKDir()."wdk_list.inc");

	// This is part of the code injection test.
	$strCodeInjection="";
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK CList::Sort");
		}
		

		function TestCase_ListSort(
			$list,
			$arraySortOptions,
			$bExpectedResult,
			$listExpected)
		{ 
			
			$this->Trace("TestCase_ListSort");
	
			$this->Trace("List:");
			$this->Trace($list->GetListArray());
			$this->Trace("Sort Options:");
			$this->Trace($arraySortOptions);
			$this->Trace("Expected List:");
			$this->Trace($listExpected->GetListArray());
	
	
			// Test some function here
			$bResult = $list->Sort($arraySortOptions);
			
			$this->Trace("bResult = ".RenderBool($bResult)."");
			
			if ($bResult != $bExpectedResult)
			{
				$this->Trace("Testcase FAILED!");	
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
					"COL3" => "A"
					),
				array(
					"ID" => 2,
					"COL1" => "Beta",
					"COL2" => "10",
					"COL3" => "B"
					),
				array(
					"ID" => 3,
					"COL1" => "Alpha",
					"COL2" => "10",
					"COL3" => "B"
					),
				array(
					"ID" => 4,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "B"
					),
				array(
					"ID" => 5,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "A"
					),
				array(
					"ID" => 6,
					"COL1" => "Beta",
					"COL2" => "10",
					"COL3" => "A"
					)
				);
		
			
			
			
			
			
			
			$arraySortOptions = array(
				"COL1" => "asc",
				"COL2" => "asc",
				"COL3" => "desc");
			$arrayListExpected = array(
				array(
					"ID" => 3,
					"COL1" => "Alpha",
					"COL2" => "10",
					"COL3" => "B"
					),
				array(
					"ID" => 1,
					"COL1" => "Alpha",
					"COL2" => "10",
					"COL3" => "A"
					),
				array(
					"ID" => 4,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "B"
					),
				array(
					"ID" => 5,
					"COL1" => "Beta",
					"COL2" => "1",
					"COL3" => "A"
					),
				array(
					"ID" => 2,
					"COL1" => "Beta",
					"COL2" => "10",
					"COL3" => "B"
					),
				array(
					"ID" => 6,
					"COL1" => "Beta",
					"COL2" => "10",
					"COL3" => "A"
					)
				);
			$list = new CList();
			$list->SetListArray($arrayList);
			$listExpected = new CList();
			$listExpected->SetListArray($arrayListExpected);
			$this->TestCase_ListSort(
				$list,
				$arraySortOptions,
				true,
				$listExpected);
				
				
				
				
				
				
				
			$arraySortOptions = array(
				"COL4" => "asc"); // does not exist
			$list = new CList();
			$list->SetListArray($arrayList);
			$listExpected = new CList();
			$listExpected->SetListArray($arrayList);
			$this->TestCase_ListSort(
				$list,
				$arraySortOptions,
				true,
				$listExpected);				

				
			/*
			
			This test case emulates a functional code injection attack against a non-protected version of CList::Sort().
			If CList::Sort() was defused, the sorting would take place and $strCodeInjection would be set to "TEST".
			
			*/
				  
			global $strCodeInjection;
			$arrayList1 = array(
				array(
					"ID" => 1,
					"COL1" => "Beta",
					'COL1"],SORT_ASC,$arrayKeys); global $strCodeInjection; $strCodeInjection="TEST"; array_multisort($arrayColumns["COL1' => "BBB"
					),
				array(
					"ID" => 2,
					"COL1" => "Alpha",
					'COL1"],SORT_ASC,$arrayKeys); global $strCodeInjection; $strCodeInjection="TEST"; array_multisort($arrayColumns["COL1' => "AAA"
					)
				);
			$arrayList2 = array(
				array(
					"ID" => 2,
					"COL1" => "Alpha",
					'COL1"],SORT_ASC,$arrayKeys); global $strCodeInjection; $strCodeInjection="TEST"; array_multisort($arrayColumns["COL1' => "AAA"
					),
				array(
					"ID" => 1,
					"COL1" => "Beta",
					'COL1"],SORT_ASC,$arrayKeys); global $strCodeInjection; $strCodeInjection="TEST"; array_multisort($arrayColumns["COL1' => "BBB"
					)
				);
			$arraySortOptions = array(
				'COL1"],SORT_ASC,$arrayKeys); global $strCodeInjection; $strCodeInjection="TEST"; array_multisort($arrayColumns["COL1' => "asc",
				'COL1' => "asc");
			$list = new CList();
			$list->SetListArray($arrayList1);
			$listExpected = new CList();
			$listExpected->SetListArray($arrayList1); // If CList::Sort() was defused, $arrayList2 would be expected.
			$this->TestCase_ListSort(
				$list,
				$arraySortOptions,
				false, // because of the " and the ; in the column names.
				$listExpected);
			
			if ($strCodeInjection=="TEST")
			{
				$this->Trace("CList::Sort() is vulnerable to a code injection attack!");
				$this->SetResult(false);	
			}
			else
			{
				$this->Trace("CList::Sort() is NOT vulnerable to a code injection attack!");
			}
				
				
		}
	}
	
	

		
