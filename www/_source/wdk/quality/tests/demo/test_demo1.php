<?php
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("DEMO 1");
		}
		
		function OnInit()
		{
			parent::OnInit();
			$this->Trace("Do some init tasks here!");
			return true;
		}
		
		function OnTest()
		{
			parent::OnTest();
			
			$this->Trace("Do the actual testing here!");
			
			$this->SetResult(true);
		}
		
		function OnCleanup()
		{
			parent::OnCleanup();
			$this->Trace("Do some Cleanup here!");
			return true;
		}
		
		
	}
		
