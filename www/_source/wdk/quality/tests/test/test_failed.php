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
					
			$this->Trace("This is a failing test!");

			$this->SetResult(false);	
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}

	}
	
	
	
