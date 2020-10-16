<?php
	
	require_once(GetWDKDir()."wdk_array.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("WDK ArrayCheckKeysByWhiteList");
		}
		  

		function TestCase_ArrayCheckKeysByWhiteList(
			$arraySource,
			$arrayWhiteList,
			$bExpectedResult,
			$arrayExpectedForbiddenKeys)
		{ 
			$this->Trace("TestCase_ArrayCheckKeysByWhiteList");
	
			$this->Trace("Source");
			$this->Trace($arraySource);
			$this->Trace("White List");
			$this->Trace($arrayWhiteList);

			$this->Trace("Expected Result"); 
			$this->Trace(RenderBool($bExpectedResult));
			$this->Trace("Expected Forbidden Keys"); 
			$this->Trace($arrayExpectedForbiddenKeys);

			
			$arrayForbiddenKeys = array();
			$bResult = ArrayCheckKeysByWhiteList($arraySource,$arrayWhiteList,$arrayForbiddenKeys);

			$this->Trace("Result");
			$this->Trace(RenderBool($bResult));

			
			$this->Trace("Forbidden Keys");
			$this->Trace($arrayForbiddenKeys);

			if ( ($bResult == $bExpectedResult) && ($arrayForbiddenKeys == $arrayExpectedForbiddenKeys))
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
			
			$arraySource = array(
				"id" 			=> 123,
				"name" 		=> "john doe",
				"address" 	=> "33 backstreet"
				);
			$arrayWhiteList = array("id","name","address");
			$arrayExpectedForbiddenKeys = array();
			$this->TestCase_ArrayCheckKeysByWhiteList($arraySource,$arrayWhiteList,true,$arrayExpectedForbiddenKeys);

			$arraySource = array(
				"id" 			=> 123,
				"name" 		=> "john doe",
				"address" 	=> "33 backstreet",
				"bogus" 	=> "Hello World"
				);
			$arrayWhiteList = array("id","name","address");
			$arrayExpectedForbiddenKeys = array("bogus");
			$this->TestCase_ArrayCheckKeysByWhiteList($arraySource,$arrayWhiteList,false,$arrayExpectedForbiddenKeys);

			$arraySource = array();
			$arrayWhiteList = array("id","name","address");
			$arrayExpectedForbiddenKeys = array();
			$this->TestCase_ArrayCheckKeysByWhiteList($arraySource,$arrayWhiteList,true,$arrayExpectedForbiddenKeys);



		}
	}
	
	

		
