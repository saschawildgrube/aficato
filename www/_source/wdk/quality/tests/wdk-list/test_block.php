<?php

	require_once(GetWDKDir()."wdk_list.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK CList::Block");
		}
		

		function TestCase_ListBlock(
			$list,
			$nBlockSize,
			$nOffset,
			$bExpectedResult,
			$listExpected)
		{ 
			
			$this->Trace("TestCase_ListBlock");
	
			$this->Trace("List:");
			$this->Trace($list->GetListArray());
			$this->Trace("nBlockSize: $nBlockSize");
			$this->Trace("nOffset: $nOffset");
			$this->Trace("Expected List:");
			$this->Trace($listExpected->GetListArray());
	
	
			// Test some function here
			$bResult = $list->Block($nBlockSize,$nOffset);
			
			$this->Trace("bResult = \"bResult\"");
			
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

			$list = new CList();
			$list->SetListArray($arrayList);


			$arrayListExpected = array(
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
					)
				);


			$listExpected = new CList();
			$listExpected->SetListArray($arrayListExpected);
			
			$this->TestCase_ListBlock(
				$list,
				2,
				1,
				true,
				$listExpected);
				
				
				
				
				
				
				
				
				
				
			$arrayListExpected = array();

			$listExpected = new CList();
			$listExpected->SetListArray($arrayListExpected);
			
			$this->TestCase_ListBlock(
				$list,
				10,
				10,
				true,
				$listExpected);				
		
				
		}
	}
	
	

		
