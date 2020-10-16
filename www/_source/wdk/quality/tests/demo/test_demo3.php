<?php
	
	
	class CTest extends CUnitTest
	{
		function __construct() 
		{
			parent::__construct("DEMO 3");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->Trace("Do some init here!");
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->Trace("Do some testing here !");
			
			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			$this->Trace("Do some Cleanup...");
			return true;
		}
		
		
	}
	
	

