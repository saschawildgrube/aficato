<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Test CUnitTest");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();

			$this->Trace("This is a successful test.");
			$this->SetResult(true);	
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
	}
	
			
