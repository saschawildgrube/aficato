<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("DEMO 1");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit();
			$this->Trace("Do some init tasks here!");
			return true;
		}
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->Trace("Do the actual testing here!");
			
			$this->SetResult(true);
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			$this->Trace("Do some Cleanup here!");
			return true;
		}
		
		
	}
		
