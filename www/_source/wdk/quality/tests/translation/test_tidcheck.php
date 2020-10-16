<?php
	
	require_once(GetWDKDir()."wdk_unittest.inc");
		
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("TID_ check");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->SetResult(true);
			return true;
		}
		
		
		function TestCase_TIDCheck(
			$strURL)
		{ 
			$this->Trace("TestCase_TIDcheck");
	
			$this->TestCase_CheckURL($strURL,array(),array("?TID_"));
		}
		
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->TestCase_TIDCheck(
				"http://".GetRootURL());
		}

		
		
	}
	
	

		
