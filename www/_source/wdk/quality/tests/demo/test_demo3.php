<?php
	
	
	class CTest extends CUnitTest
	{
		function __construct() 
		{
			parent::__construct("DEMO 3");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->Trace("Do some init here!");
			return true;
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			$this->Trace("Do some testing here !");
			
			$this->SetResult(true);
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			$this->Trace("Do some Cleanup...");
			return true;
		}
		
		
	}
	
	

