<?php
	
	require_once(GetWDKDir()."wdk_list.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK CList::Join");
		}
		

		function TestCase_ListJoin(
			$list,
			$listJoin,
			$arrayJoinConditions,
			$arrayJoinFields,
			$bExpectedResult,
			$listExpected)
		{ 
			
			$this->Trace("TestCase_ListJoin");
	
			$this->Trace("List:");
			$this->Trace($list->GetListArray());
			$this->Trace("List to be joined:");
			$this->Trace($listJoin->GetListArray());
			$this->Trace("Join Conditions:");
			$this->Trace($arrayJoinConditions);			
			$this->Trace("Fields to be joined:");
			$this->Trace($arrayJoinFields);			
			$this->Trace("Expected List:");
			$this->Trace($listExpected->GetListArray());
	
	
			$bResult = $list->Join(
				$listJoin,
				$arrayJoinConditions,
				$arrayJoinFields);
			
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
					"ALPHA" => "Alpha",
					"NUMBER" => "1",
					"TEXT" => "Alpha is the first one"
					),
				array(
					"ALPHA" => "Beta",
					"NUMBER" => "2",
					"TEXT" => "Beta is the second one"
					),
				array(
					"ALPHA" => "Gamma",
					"NUMBER" => "3",
					"TEXT" => "Gamma is neither the first nor the second one"
					),
				array(
					"ALPHA" => "Delta",
					"NUMBER" => "4",
					"TEXT" => "Delta is also a name for an airline"
					),
				array(
					"ALPHA" => "Epsilon",
					"NUMBER" => "5",
					"TEXT" => "Epsilon starts with an 'e'"
					)
				);
				
			$arrayListJoin = array(
				array(
					"ALPHA" => "Alpha",
					"NUMBER" => "11",
					"TEXT" => "A"
					),
				array(
					"ALPHA" => "Beta",
					"NUMBER" => "22",
					"TEXT" => "B"
					),
				array(
					"ALPHA" => "Gamma",
					"NUMBER" => "33",
					"TEXT" => "C"
					),
				array(
					"ALPHA" => "Delta",
					"NUMBER" => "44",
					"TEXT" => "D"
					),
				array(
					"ALPHA" => "Epsilon",
					"NUMBER" => "55",
					"TEXT" => "E"
					)
				);				
				
			$arrayJoinConditions = array("ALPHA");
			$arrayJoinFields = array("TEXT");
		
			$arrayListExpected = array(
				array(
					"ALPHA" => "Alpha",
					"NUMBER" => "1",
					"TEXT" => "A"
					),
				array(
					"ALPHA" => "Beta",
					"NUMBER" => "2",
					"TEXT" => "B"
					),
				array(
					"ALPHA" => "Gamma",
					"NUMBER" => "3",
					"TEXT" => "C"
					),
				array(
					"ALPHA" => "Delta",
					"NUMBER" => "4",
					"TEXT" => "D"
					),
				array(
					"ALPHA" => "Epsilon",
					"NUMBER" => "5",
					"TEXT" => "E"
					)
				);

				
			$list = new CList();
			$list->SetListArray($arrayList);
			$listJoin = new CList();
			$listJoin->SetListArray($arrayListJoin);
			$listExpected = new CList();
			$listExpected->SetListArray($arrayListExpected);
			$this->TestCase_ListJoin( 
				$list,
				$listJoin,
				$arrayJoinConditions,
				$arrayJoinFields,
				true,
				$listExpected);				

				
				
				
				
				
				
				
		}
	}
	
	

		
