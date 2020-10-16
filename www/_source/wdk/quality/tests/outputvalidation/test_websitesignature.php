<?php

	require_once(GetWDKDir()."wdk_unittest.inc");
	
	class CTest extends CUnitTest
	{
		function __construct()
		{
			parent::__construct("Check UTF-8 Signature");
		}
		
		function CallbackInit()
		{
			parent::CallbackInit(); 
			$this->SetResult(true);
			return true;
		}
		
			
		
		function CallbackTest()
		{
			parent::CallbackTest();
			
			$this->TestCase_CheckURL(
				"http://".GetRootURL(),
				array(),
				array(),
				array("/^<\!DOCTYPE html>/"));
				
			if ($this->GetResult() != true)
			{
				$this->Trace("The tested URL returns a signature that does not directly start with:");
				$this->Trace("\"<!DOCTYPE html>\"");
				$this->Trace("Possible reasons are:");	
				$this->Trace("1. The page could not be loaded at all (this is likely to be temporary).");
				$this->Trace("2. The server has an encoding issue. Check if the UTF-8 or UTF-9 BOMs are prefixing the http result.");
				$this->Trace("3. Debug output is printed before the actual website code is printed.");
				$this->Trace("4. URL environment settings may be incorrect.");
			}
		 
		}
		
		function CallbackCleanup()
		{
			parent::CallbackCleanup();
			return true;
		}
		
		
	}
	
	
		


		
