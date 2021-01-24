<?php

	require_once(GetWDKDir()."wdk_list.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK CList::GetStatistics");
		}
		

		function TestCase_GetStatistics(
			$list,
			$arrayExpectedResult)
		{ 
			
			$this->Trace("TestCase_ListFilter");
	
			$this->Trace("List:");
			$this->Trace($list->GetListArray());
			$this->Trace("Expected Result:");
			$this->Trace($arrayExpectedResult);
	
			$arrayResult = $list->GetStatistics();
			
			$this->Trace("Result:");
			$this->Trace($arrayResult);
			
			if ($arrayResult != $arrayExpectedResult)
			{
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
					"NAME" => "Peter",
					"AGE" => 20
					),
				array(
					"NAME" => "Paul",
					"AGE" => 22
					),
				array(
					"NAME" => "Mary",
					"AGE" => 24
					)
				);
			$list = new CList();
			$list->SetListArray($arrayList);
			
			$arrayExpected = array(
				"AGE" => array(
					"count" => 3,
					"sum" => 66,
					"max" => 24,
					"min" => 20,
					"dif" => 4,
					"avg" => 22
					)
				);
			
			$this->TestCase_GetStatistics(
				$list,
				$arrayExpected);
				
				
				
				
				
				
		}
	}
	
	

		
